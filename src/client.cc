/* Copyright wangchw(wangox@gmail.com). All right reserved.
 * Use of this source code is governed by BSD-style license
 * that can be found in the LICENSE file.
 */

#include <wrpc/client.h>
#include <wrpc/wrpc.pb.h>
#include <istream>
#include <ostream>

namespace wrpc {


Client::Client(const std::string& host, 
               const std::string& svc) : resolver_(io_service_), socket_(io_service_) {
  Resolve(host, svc);
}

Client::~Client() {
}

void Client::Resolve(const std::string& host, const std::string& svc) {
  tcp::resolver::query query(host, svc);
  endpoint_ = resolver_.resolve(query);
}

int Client::Call(google::protobuf::RpcController* controller, 
                 const RpcRequest* req,
                 RpcResponse* res) {
  std::lock_guard<std::mutex> lock(mutex_); // for thread safe
  if (Connect(controller) != 0) {
    return -1;
  }
  if (WriteRequest(controller, req) != 0) {
    return -1;
  }
  if (ReadResponse(controller, res) != 0) {
    return -1;
  }
  if (req->id() != res->id()) {
    controller->SetFailed("the response is not for the request.");
    return -1;
  }
  return 0;
}

int Client::Connect(google::protobuf::RpcController* controller) {
  boost::system::error_code ec;
  boost::asio::connect(socket_, endpoint_, ec);
  if (ec) {
    std::string e("Error:");
    e += ec.message();
    controller->SetFailed(e);
    return -1;
  }
  return 0;
}

int Client::WriteRequest(google::protobuf::RpcController* controller, const RpcRequest* req) {
  int len = req->ByteSize();
  char* data = (char*)(&len);
  boost::system::error_code ec;
  boost::asio::write(socket_, boost::asio::buffer(data, sizeof(len)), ec);
  if (ec) {
    std::string e("Error:");
    e += ec.message();
    controller->SetFailed(e);
    return -1;
  }

  boost::asio::streambuf req_buf;
  std::ostream os(&req_buf);
  req->SerializeToOstream(&os);

  boost::asio::write(socket_, req_buf, ec);
  if (ec) {
    std::string e("Error:");
    e += ec.message();
    controller->SetFailed(e);
    return -1;
  }

  return 0;
}

int Client::ReadResponse(google::protobuf::RpcController* controller, RpcResponse* res) {
  int len = 0;
  char* data = (char*)(&len);
  boost::system::error_code ec;
  boost::asio::read(socket_, 
                    boost::asio::buffer(data, sizeof(len)), 
                    boost::asio::transfer_exactly(sizeof(int)), 
                    ec);
  if (ec) {
    std::string e("Error:");
    e += ec.message();
    controller->SetFailed(e);
    return -1;
  }

  boost::asio::streambuf res_buf;
  boost::asio::read(socket_, 
                    res_buf, 
                    boost::asio::transfer_exactly(len),
                    ec);
  if (ec) {
    std::string e("Error:");
    e += ec.message();
    controller->SetFailed(e);
    return -1;
  }

  std::istream is(&res_buf);
  if (!res->ParseFromIstream(&is)) {
    std::string e("Error: google::protobuf::Message::ParseFromIstream error.");
    controller->SetFailed(e);
    return -1;
  }
  return 0;
}

} // namespace wrpc
