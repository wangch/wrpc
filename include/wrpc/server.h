/* Copyright wangchw(wangox@gmail.com). All right reserved.
 * Use of this source code is governed by BSD-style license
 * that can be found in the LICENSE file.
 */


#ifndef WRPC_SERVER_H_
#define WRPC_SERVER_H_

#include "conn.h"
#include "handler.h"
#include <boost/asio.hpp>
#include <map>

namespace wrpc {

class Server {
 public:
  /// Construct the server to listen on the specified TCP address and port,and
  /// serve up files from the given directory.
  explicit Server(const std::string& address, 
                  const std::string& port,
                  std::size_t thread_pool_size);

  void RegService(google::protobuf::Service* service);
  /// Run the server's io_service loop.
  void Run();

 private:
  /// Initiate an asynchronous accept operation.
  void StartAccept();

  /// Handle completion of an asynchronous accept
  void HandleAccept(const boost::system::error_code&  e);

  /// Handle a request to stop the server.
  void HandleStop();

  /// The number of threads that will call io_service::run().
  std::size_t thread_pool_size_;
  
  /// The io_service used to perform asynchronous operations.
  boost::asio::io_service io_service_;
  
  /// The signal_set is used to register for process termination notifications.
  //boost::asio::signal_set signals_;
  
  /// Acceptor used to listen for incoming connections.
  boost::asio::ip::tcp::acceptor acceptor_;
  
  /// The next connection to be accepted.
  ConnPtr new_connection_;
  
  /// The handler for all incoming requests.
  Handler handler_;

};

} // namespace wrpc


#endif //WRPC_SERVER_H_
