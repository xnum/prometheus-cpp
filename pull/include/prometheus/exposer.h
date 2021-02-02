#pragma once

#include <atomic>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "prometheus/collectable.h"
#include "prometheus/detail/pull_export.h"
#include "prometheus/registry.h"

class CivetServer;

namespace prometheus {

namespace detail {
class Endpoint;
class MetricsHandler;
class DumpHandler;
class ProbeHandler;
}  // namespace detail

class PROMETHEUS_CPP_PULL_EXPORT Exposer {
 public:
  explicit Exposer(const std::string& bind_address,
                   const std::size_t num_threads = 2);
  explicit Exposer(std::vector<std::string> options);
  ~Exposer();
  void RegisterCollectable(const std::weak_ptr<Collectable>& collectable);
  void RegisterObservable(std::function<std::string()> observables);
  void RegisterAliveness(std::function<bool()> aliveProbe);
  void RegisterReadiness(std::function<bool()> readyProbe);

  std::vector<int> GetListeningPorts() const;

 private:
  std::unique_ptr<CivetServer> server_;
  std::shared_ptr<Registry> endpoint_registry_;
  std::vector<std::weak_ptr<Collectable>> collectables_;
  std::vector<std::function<std::string()>> observables_;
  std::function<bool()> aliveProbe_;
  std::function<bool()> readyProbe_;

  std::unique_ptr<detail::MetricsHandler> metricsHandler_;
  std::unique_ptr<detail::DumpHandler> varsHandler_;
  std::unique_ptr<detail::ProbeHandler> aliveHandler_;
  std::unique_ptr<detail::ProbeHandler> readyHandler_;
  std::unique_ptr<detail::Endpoint> metricsEndpoint_;
  std::unique_ptr<detail::Endpoint> varsEndpoint_;
  std::unique_ptr<detail::Endpoint> aliveEndpoint_;
  std::unique_ptr<detail::Endpoint> readyEndpoint_;
};

}  // namespace prometheus
