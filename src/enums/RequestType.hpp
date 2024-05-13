#pragma once

#include <string>
#include <unordered_map>

namespace httpsli::http {

enum RequestType { GET, POST, DELETE, UPDATE, OPTIONS };

class StringToEnum {
public:
  static RequestType Get(const std::string &request_type_name) {
    if (getter_.find(request_type_name) != getter_.end()){
      return getter_.at(request_type_name);
    }
    return RequestType::GET;
  }

private:
  static std::unordered_map<std::string, RequestType> getter_;
};

class EnumToString {
public:
  static std::string Get(const RequestType &request_type) {
    if (getter_.find(request_type) != getter_.end()){
      return getter_.at(request_type);
    }
    return "";
  }

private:
  static std::unordered_map<RequestType, std::string> getter_;
};
} // namespace httpsli::http