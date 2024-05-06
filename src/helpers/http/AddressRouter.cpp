#include "AddressRouter.hpp"
#include <optional>

namespace httpsli::helpers::http {
    bool HandlerPair::IsMatching(const std::string& address) const {
        return true;
    }

    Handler HandlerPair::GetHandler() const {
        return handler_;
    }

    std::optional<Handler> AddressRouter::FindHandler(httpsli::requests::http::HttpRequest& request) const{
        for (const HandlerPair& pair : handlers_){
            if (pair.IsMatching(request.GetAddress())){
                return pair.GetHandler();
            }
        }
        return std::nullopt;
    }
}