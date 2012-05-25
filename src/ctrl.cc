/* Copyright wangchw(wangox@gmail.com). All right reserved.
 * Use of this source code is governed by BSD-style license
 * that can be found in the LICENSE file.
 */


#include <wrpc/ctrl.h>

namespace wrpc {

Controller::Controller() : failed_(false) {
}

Controller::~Controller() {
}

void Controller::Reset() {
}

bool Controller::Failed() const {
  return failed_;
}

std::string Controller::ErrorText() const {
  return reason_;
}

void Controller::StartCancel() {
}

void Controller::SetFailed(const std::string& reason) {
  failed_ = true;
  reason_ = reason;
}

bool Controller::IsCanceled() const {
}

void Controller::NotifyOnCancel(google::protobuf::Closure* callback) {
}

} //namespace wrpc 


