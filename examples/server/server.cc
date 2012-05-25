/*
 *
 *
 */

#include "server.h"
#include "client.h"
#include <wrpc/server.h>
#include <iostream>


void Service::SendUsrp(::google::protobuf::RpcController* controller,
                         const ::dragoon::UsrpMessage*,
                         ::dragoon::ResultMessage*,
                         ::google::protobuf::Closure* done) {
  controller->SetFailed("Method SendUsrp() not implemented.");
  done->Run();
}

void Service::SendTmsi(::google::protobuf::RpcController* controller,
                         const ::dragoon::TmsiMessage*,
                         ::dragoon::ResultMessage*,
                         ::google::protobuf::Closure* done) {
  controller->SetFailed("Method SendTmsi() not implemented.");
  done->Run();
}

void Service::SendCell(::google::protobuf::RpcController* controller,
                         const ::dragoon::CellMessage* cell,
                         ::dragoon::ResultMessage* res,
                         ::google::protobuf::Closure* done) {
  std::cout << "Lac: " << cell->lac() << std::endl;
  std::cout << "Cell id: " << cell->id() << std::endl;
  std::cout << "Cell antenna: " << cell->ant() << std::endl;
  std::cout << "Cell freq: " << cell->freq() << std::endl;
  std::cout << "Cell index: " << cell->index() << std::endl;
  std::cout << "Cell energy: " << cell->energy() << std::endl;
  std::cout << "Cell nb: ";
  for (int i = 0; i < cell->nb_size(); ++i) {
    std::cout << cell->nb(i) << " ";
  }
  std::cout << std::endl;
  res->set_r(100);
  if (done) {
    done->Run();
  }
}

void Service::SendLog(::google::protobuf::RpcController* controller,
                         const ::dragoon::LogMessage*,
                         ::dragoon::ResultMessage*,
                         ::google::protobuf::Closure* done) {
  controller->SetFailed("Method SendLog() not implemented.");
  done->Run();
}

int main(int argc, char* argv[]) {
  wrpc::Server s("0.0.0.0", "12345", 2);
  s.RegService(new Service);
  std::thread thr(rmain);
  s.Run();
  thr.join();
  return 0;
}
