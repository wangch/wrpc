/* Copyright wangchw(wangox@gmail.com). All right reserved.
 * Use of this source code is governed by BSD-style license
 * that can be found in the LICENSE file.
 */


#ifndef WRPC_CLIENT_H_
#define WRPC_CLIENT_H_

#include <google/protobuf/service.h>
#include <boost/asio.hpp>
#include <string>
#include <mutex>
#include <condition_variable>

namespace wrpc {

using namespace boost;
using namespace boost::asio::ip;

class RpcRequest;
class RpcResponse;

class Client {
 public:
  Client(const std::string& host, const std::string& svc);
  ~Client();
  int Call(google::protobuf::RpcController* controller, 
           const RpcRequest* req, 
           RpcResponse* res);

 private:
  void Resolve(const std::string& host, const std::string& svc);
  int Connect(google::protobuf::RpcController* controller);
  int WriteRequest(google::protobuf::RpcController* controller, const RpcRequest* req);
  int ReadResponse(google::protobuf::RpcController* controller, RpcResponse* res);

 private:
  boost::asio::io_service io_service_;
  tcp::resolver resolver_;
  tcp::socket socket_;
  tcp::resolver::iterator endpoint_;
  std::mutex mutex_;
};

} // namespace wrpc

#endif //WRPC_CLIENT_H_
