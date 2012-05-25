


#include "lcs.pb.h"
#include <wrpc/channel.h>
#include <wrpc/client.h>
#include <wrpc/ctrl.h>
#include <string>
#include <iostream>
#include <thread>

using namespace dragoon;

class ClientTest {
 public:
  ClientTest(google::protobuf::RpcChannel* chan);

  void SendCell_Test();
  void SendUsrp_Test();
  void SendTmsi_Test();
  void SendLog_Test();
 private:
  LocaterService* service_;
};

ClientTest::ClientTest(google::protobuf::RpcChannel* chan) : 
    service_(new LocaterService::Stub(chan)) {
}

void Done(google::protobuf::RpcController* ctrl, 
          ResultMessage* rm) {
  if (ctrl->Failed()) {
    std::cout << "Error: " << ctrl->ErrorText() << std::endl;
  } else {
    std::cout << "call ok" << std::endl;
  }

  delete ctrl;
  delete rm;
}

void ClientTest::SendCell_Test() {
  CellMessage cm;
  for (int i = 0; i < 1000*1000*1000; ++i) {
    cm.set_ant(i);
    cm.set_id("xxx_" + std::to_string(i));
    cm.set_freq(100*i);
    cm.set_index(i % 4);
    cm.set_energy(100%(i+1));
    for (int n = 0; n < 3; ++n) {
      cm.add_nb("xxx_" + std::to_string(i-n));
    }
    cm.set_lac("xxx");
    google::protobuf::RpcController* ctrl = new wrpc::Controller;
    ResultMessage* rm = new ResultMessage;
    google::protobuf::Closure* done = google::protobuf::NewCallback(&Done, ctrl, rm);
    service_->SendCell(ctrl, &cm, rm, done);
  }
}

int rmain() {
  wrpc::Client client("127.0.0.1", "12346");
  ClientTest ct(new wrpc::Channel(&client));
  std::thread thr([&ct] { ct.SendCell_Test();});
  ct.SendCell_Test();
  thr.join();
  char c;
  std::cin >> c;
  return 0;
}


