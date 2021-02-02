#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "basic_auth.h"
#include "prometheus/collectable.h"
#include "prometheus/registry.h"

class CivetServer;
class CivetHandler;

namespace prometheus {
namespace detail {
class MetricsHandler;

class Endpoint {
 public:
  explicit Endpoint(CivetServer& server, std::string uri,
                    CivetHandler* handler);
  ~Endpoint();

  const std::string& GetURI() const;

 private:
  CivetServer& server_;
  const std::string uri_;
  CivetHandler* handler_;
};

}  // namespace detail
}  // namespace prometheus
