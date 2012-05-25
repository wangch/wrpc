/* Copyright wangchw(wangox@gmail.com). All right reserved.
 * Use of this source code is governed by BSD-style license
 * that can be found in the LICENSE file.
 */


#include <wrpc/channel.h>
#include <wrpc/client.h>
#include <wrpc/wrpc.pb.h>
#include <google/protobuf/descriptor.h>
#include <istream>
#include <ostream>

namespace wrpc {

unsigned long long Channel::seq_id_ = 0;

Channel::Channel(Client* client) : client_(client) {
}

Channel::~Channel() {
}

void Channel::CallMethod(const google::protobuf::MethodDescriptor* method,
                         google::protobuf::RpcController* controller,
                         const google::protobuf::Message* request,
                         google::protobuf::Message* response,
                         google::protobuf::Closure* done) {
  RpcRequest rpc_req;
  rpc_req.set_id(++seq_id_);
  rpc_req.set_service(method->service()->full_name());
  rpc_req.set_method(method->full_name());

  int len = request->ByteSize();
  char* p = new char[len];
  if (!request->SerializeToArray(p, len)) {
    delete [] p;
    return;
  }
  rpc_req.set_req(p, len);
  delete [] p;

  RpcResponse rpc_res;
  int r = client_->Call(controller, &rpc_req, &rpc_res);
  if (r != 0) {
    if (done) {
      done->Run();
    }
    return -1;
  }

  std::string e(controller->ErrorText());
  auto err = rpc_res.err();
  if (err.code() != 0) {
    e += err.reason();
    controller->SetFailed(e);
  }

  const std::string& r = rpc_res.res();
  if (!response->ParseFromArray(r.c_str(), r.size())) {
    controller->SetFailed(e + "Error: google::protobuf::Message::ParseFromArray error.");
  }

  if (done) {
    done->Run();
  }
}

}
