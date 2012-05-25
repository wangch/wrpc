/* Copyright wangchw(wangox@gmail.com). All right reserved.
 * Use of this source code is governed by BSD-style license
 * that can be found in the LICENSE file.
 */

#include <wrpc/conn.h>
#include <wrpc/handler.h>
#include <wrpc/ctrl.h>
#include <wrpc/wrpc.pb.h>
#include <boost/bind.hpp>
#include <istream>
#include <ostream>

namespace wrpc {

Conn::Conn(boost::asio::io_service& io_service,
           Handler& handler) : 
    strand_(io_service), 
    socket_(io_service),
    request_handler_(handler) {
    }

boost::asio::ip::tcp::socket& Conn::socket() {
  return socket_;
}

void Conn::Start() {
  boost::asio::async_read(socket_, 
                          boost::asio::buffer(buffer_),
                          boost::asio::transfer_exactly(sizeof(int)), 
                          strand_.wrap(boost::bind(&Conn::HandleRead,
                                                   shared_from_this(),
                                                   boost::asio::placeholders::error)));
}

void Conn::HandleRead(const boost::system::error_code& e) {
  if (!e) {
    int len = buffer_[0];
    boost::asio::async_read(socket_, 
                            request_,
                            boost::asio::transfer_exactly(len), 
                            strand_.wrap(boost::bind(&Conn::HandleRequest, 
                                                     shared_from_this(),
                                                     boost::asio::placeholders::error)));
  }
}

void Conn::HandleRequest(const boost::system::error_code& e) {
  if (!e) {
    std::istream is(&request_);
    RpcRequest req;
    if (!req.ParseFromIstream(&is)) {
      return;
    }
    RpcResponse res;
    int r = request_handler_.HandleRequest(&req, &res);
    if (r != 0) {
      return;
    }

    buffer_[0] = res.ByteSize();
    boost::system::error_code ec;
    boost::asio::write(socket_, 
                       boost::asio::buffer(buffer_),
                       ec);
    if (ec) {
      return;
    }

    std::ostream os(&response_);
    if (!res.SerializeToOstream(&os)) {
      return;
    }
    boost::asio::async_write(socket_, 
                             response_, 
                             strand_.wrap(boost::bind(&Conn::HandleWrite, 
                                                      shared_from_this(),
                                                      boost::asio::placeholders::error)));
  }
}

void Conn::HandleWrite(const boost::system::error_code& e) {
  if (!e) {
    boost::system::error_code ignored_ec;
    socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
  }
}

} // namespace wrpc
