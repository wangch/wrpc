/*
 *
 *
 */

#ifndef WRPC_EXAMPLE_SERVER_H_
#define WRPC_EXAMPLE_SERVER_H_

#include "lcs.pb.h"

class Service : public dragoon::LocaterService {
 public:
  virtual void SendUsrp(::google::protobuf::RpcController* controller,
                       const ::dragoon::UsrpMessage* request,
                       ::dragoon::ResultMessage* response,
                       ::google::protobuf::Closure* done);
  virtual void SendTmsi(::google::protobuf::RpcController* controller,
                       const ::dragoon::TmsiMessage* request,
                       ::dragoon::ResultMessage* response,
                       ::google::protobuf::Closure* done);
  virtual void SendCell(::google::protobuf::RpcController* controller,
                       const ::dragoon::CellMessage* request,
                       ::dragoon::ResultMessage* response,
                       ::google::protobuf::Closure* done);
  virtual void SendLog(::google::protobuf::RpcController* controller,
                       const ::dragoon::LogMessage* request,
                       ::dragoon::ResultMessage* response,
                       ::google::protobuf::Closure* done);
};

#endif //WRPC_EXAMPLE_SERVER_H_
