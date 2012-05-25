/* Copyright wangchw(wangox@gmail.com). All right reserved.
 * Use of this source code is governed by BSD-style license
 * that can be found in the LICENSE file.
 */


#ifndef WRPC_CHANNEL_H_
#define WRPC_CHANNEL_H_


#include <google/protobuf/service.h>
#include <string>

namespace wrpc {

class Client;
class RpcResponse;

class Channel : public google::protobuf::RpcChannel {
 public:
  Channel(Client* client);
  virtual ~Channel();
  virtual void CallMethod(const google::protobuf::MethodDescriptor* method,
                          google::protobuf::RpcController* controller,
                          const google::protobuf::Message* request,
                          google::protobuf::Message* response,
                          google::protobuf::Closure* done = 0);
 private:
  Client* client_;
  static unsigned long long seq_id_;
};

} // namespace wrpc

#endif //WRPC_CHANNEL_H_ 
