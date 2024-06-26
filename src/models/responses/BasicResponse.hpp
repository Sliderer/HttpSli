#pragma once

#include <string>

namespace httpsli::responses {
class BasicResponse {
public:
  BasicResponse() {}

  virtual std::string Serialize() const = 0;
};
} // namespace httpsli::responses