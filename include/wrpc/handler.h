/* Copyright wangchw(wangox@gmail.com). All right reserved.
 * Use of this source code is governed by BSD-style license
 * that can be found in the LICENSE file.
 */


#ifndef WRPC_HANDLER_H_
#define WRPC_HANDLER_H_


#include <google/protobuf/service.h>
#include <vector>

namespace wrpc {

class RpcRequest;
class RpcResponse;

class Handler {
 public:
  Handler();
  int RegService(google::protobuf::Service* s);
  int HandleRequest(const RpcRequest* req, RpcResponse* res) const;

 private:
  void Done(); 
  void EncodeResponse(google::protobuf::RpcController* controller,
                      const RpcRequest* req, 
                      RpcResponse* res, 
                      google::protobuf::Message* m) const; 

  std::vector<google::protobuf::Service*> svcs_;
};


} // namespace wrpc

#endif //WRPC_HANDLER_H_

