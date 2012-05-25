/* Copyright wangchw(wangox@gmail.com). All right reserved.
 * Use of this source code is governed by BSD-style license
 * that can be found in the LICENSE file.
 */

#include <wrpc/handler.h>
#include <wrpc/ctrl.h>
#include <wrpc/wrpc.pb.h>
#include <google/protobuf/descriptor.h>

namespace wrpc {

Handler::Handler() {
}

int Handler::RegService(google::protobuf::Service* s) {
  svcs_.push_back(s);
}

void Handler::Done() {
}

void Handler::EncodeResponse(google::protobuf::RpcController* controller,
                             const RpcRequest* req, 
                             RpcResponse* res, 
                             google::protobuf::Message* m) const {
  res->set_id(req->id());

  RpcError e;
  e.set_code(0);
  if (controller->Failed()) {
    e.set_code(-1);
    e.set_reason(controller->ErrorText());
  }
  res->mutable_err()->CopyFrom(e);

  const int len = m->ByteSize();
  char* p = new char[len];
  if (!m->SerializeToArray(p, len)) {
    delete [] p;
    return;
  }
  res->set_res(p, len);
  delete [] p;
}

int Handler::HandleRequest(const RpcRequest* req, 
                           RpcResponse* res) const {
  for (auto& s : svcs_) {
    auto sd = s->GetDescriptor();
    if (sd->full_name() != req->service()) {
      continue;
    }

    for (int i = 0; i < sd->method_count(); ++i) {
      auto md = sd->method(i);
      if (md->full_name() == req->method()) {
        auto req_msg = s->GetRequestPrototype(md).New();
        auto res_msg = s->GetResponsePrototype(md).New();

        const std::string& r = req->req();
        req_msg->ParseFromArray(r.c_str(), r.size());

        google::protobuf::RpcController* controller = new Controller;

        // call your service method
        s->CallMethod(md, controller, req_msg, res_msg, NULL);
        EncodeResponse(controller, req, res, res_msg);

        delete controller;
        delete req_msg;
        delete res_msg;

        return 0;
      }
    }
  }
  return -1;
}

} // namespace wrpc
