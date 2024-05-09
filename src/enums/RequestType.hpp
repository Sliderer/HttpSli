#pragma once

#include <unordered_map>
#include <string>

namespace httpsli::http {

    enum RequestType{
        GET, 
        POST, 
        DELETE, 
        UPDATE, 
        OPTIONS
    };

    class EnumGetter {
        public:

            static RequestType GetRequestType(const std::string& request_type_name){
                return getter_.at(request_type_name);
            }
        private:
            static std::unordered_map<std::string, RequestType> getter_;
    };
}