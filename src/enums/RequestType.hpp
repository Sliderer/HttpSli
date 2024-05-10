#pragma once

#include <string>
#include <unordered_map>

namespace httpsli::http {

enum RequestType { GET, POST, DELETE, UPDATE, OPTIONS };

class EnumGetter {
public:
  static RequestType GetRequestType(const std::string &request_type_name) {
    if (getter_.find(request_type_name) != getter_.end()){
      return getter_.at(request_type_name);
    }
    return RequestType::GET;
  }

private:
  static std::unordered_map<std::string, RequestType> getter_;
};
} // namespace httpsli::http