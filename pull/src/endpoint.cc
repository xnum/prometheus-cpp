#include "endpoint.h"

#include "basic_auth.h"
#include "handler.h"
#include "prometheus/detail/future_std.h"

namespace prometheus {
namespace detail {

Endpoint::Endpoint(CivetServer& server, std::string uri, CivetHandler* handler)
    : server_(server), uri_(std::move(uri)), handler_(handler) {
  server_.addHandler(uri_, handler_);
}

Endpoint::~Endpoint() { server_.removeHandler(uri_); }

const std::string& Endpoint::GetURI() const { return uri_; }

}  // namespace detail
}  // namespace prometheus
