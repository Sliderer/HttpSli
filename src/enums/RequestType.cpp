#include "RequestType.hpp"

namespace httpsli::http {
std::unordered_map<std::string, RequestType> StringToEnum::getter_ = {
    {"GET", RequestType::GET},
    {"POST", RequestType::POST},
    {"DELETE", RequestType::DELETE},
    {"UPDATE", RequestType::UPDATE},
    {"OPTIONS", RequestType::OPTIONS}};

std::unordered_map<RequestType, std::string> EnumToString::getter_ = {
    {RequestType::GET, "GET"},
    {RequestType::POST, "POST"},
    {RequestType::DELETE, "DELETE"},
    {RequestType::UPDATE, "UPDATE"},
    {RequestType::OPTIONS, "OPTIONS"}};
}