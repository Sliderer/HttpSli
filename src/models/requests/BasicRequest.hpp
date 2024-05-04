#pragma once 

#include <string>

namespace httpsli::requests{
    class BasicRequest {
        public:
            BasicRequest() {}
            
            virtual std::string Serialize() = 0;
    };
}