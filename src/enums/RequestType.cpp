#include "RequestType.hpp"

namespace httpsli::http {
std::unordered_map<std::string, RequestType> EnumGetter::getter_ = {
    {"GET", RequestType::GET},
    {"POST", RequestType::POST},
    {"DELETE", RequestType::DELETE},
    {"UPDATE", RequestType::UPDATE},
    {"OPTIONS", RequestType::OPTIONS}};
}