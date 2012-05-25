/* Copyright wangchw(wangox@gmail.com). All right reserved.
 * Use of this source code is governed by BSD-style license
 * that can be found in the LICENSE file.
 */


#ifndef WRPC_CONNECT_H_
#define WRPC_CONNECT_H_

//#include <handler.h>
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <array>

namespace wrpc {

class Handler;
/// Represents a single connection from a client.
class Conn : public boost::enable_shared_from_this<Conn>, private boost::noncopyable {
 public:
  /// Construct a connection with the given io_service.
  explicit Conn(boost::asio::io_service& io_service,
                      Handler& handler);

  /// Get the socket associated with the connection.
  boost::asio::ip::tcp::socket& socket();

  /// Start the first asynchronous operation for the connection.
  void Start();
 private:
  /// Handle completion of a read operation.
  void HandleRead(const boost::system::error_code& e);
  void HandleRequest(const boost::system::error_code& e);

  /// Handle completion of a write operation.
  void HandleWrite(const boost::system::error_code& e);


  /// Strand to ensure the connection's handlers are not called concurrently.
  boost::asio::io_service::strand strand_;

  /// Socket for the connection.
  boost::asio::ip::tcp::socket socket_;

  /// The handler used to process the incoming request.
  Handler& request_handler_;

  /// Buffer for incoming data.
  std::array<int, 1> buffer_;

  /// The incoming request.
  boost::asio::streambuf request_;

  /// The reply to be sent back to the client.
  boost::asio::streambuf response_;
};

typedef boost::shared_ptr<Conn> ConnPtr;

} // namespace wrpc 

#endif //WRPC_CONNECT_H_

