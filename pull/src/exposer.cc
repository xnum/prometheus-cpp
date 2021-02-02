#include "prometheus/exposer.h"

#include <chrono>
#include <string>
#include <thread>

#include "CivetServer.h"
#include "endpoint.h"
#include "handler.h"
#include "prometheus/client_metric.h"
#include "prometheus/detail/future_std.h"

namespace prometheus {

Exposer::Exposer(const std::string& bind_address, const std::size_t num_threads)
    : Exposer(std::vector<std::string>{"listening_ports", bind_address,
                                       "num_threads",
                                       std::to_string(num_threads)}) {}

Exposer::Exposer(std::vector<std::string> options)
    : server_(detail::make_unique<CivetServer>(std::move(options))),
      endpoint_registry_(std::make_shared<Registry>()) {
  collectables_.push_back(endpoint_registry_);

  metricsHandler_ = detail::make_unique<detail::MetricsHandler>(
      collectables_, *endpoint_registry_);
  metricsEndpoint_ = detail::make_unique<detail::Endpoint>(
      *server_, "/metrics", metricsHandler_.get());

  varsHandler_ = detail::make_unique<detail::DumpHandler>(observables_);
  varsEndpoint_ = detail::make_unique<detail::Endpoint>(*server_, "/vars",
                                                        varsHandler_.get());

  aliveHandler_ = detail::make_unique<detail::ProbeHandler>(aliveProbe_);
  aliveEndpoint_ = detail::make_unique<detail::Endpoint>(*server_, "/alive",
                                                         aliveHandler_.get());

  readyHandler_ = detail::make_unique<detail::ProbeHandler>(readyProbe_);
  readyEndpoint_ = detail::make_unique<detail::Endpoint>(*server_, "/ready",
                                                         readyHandler_.get());
}

Exposer::~Exposer() = default;

void Exposer::RegisterCollectable(
    const std::weak_ptr<Collectable>& collectable) {
  collectables_.push_back(collectable);
}

void Exposer::RegisterObservable(std::function<std::string()> observables) {
  observables_.push_back(observables);
}

void Exposer::RegisterAliveness(std::function<bool()> aliveProbe) {
  aliveProbe_ = aliveProbe;
}

void Exposer::RegisterReadiness(std::function<bool()> readyProbe) {
  readyProbe_ = readyProbe;
}

std::vector<int> Exposer::GetListeningPorts() const {
  return server_->getListeningPorts();
}

}  // namespace prometheus
