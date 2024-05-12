#pragma once

#include <string>

namespace httpsli::requests {
class BasicRequest {
public:
  BasicRequest() {}

  virtual std::string Serialize() const = 0;
};
} // namespace httpsli::requests