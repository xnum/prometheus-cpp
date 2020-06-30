#pragma once

#include <memory>
#include <vector>

#include "CivetServer.h"
#include "prometheus/counter.h"
#include "prometheus/registry.h"
#include "prometheus/summary.h"

namespace prometheus {
namespace detail {
class MetricsHandler : public CivetHandler {
 public:
  MetricsHandler(const std::vector<std::weak_ptr<Collectable>>& collectables,
                 const std::vector<std::function<std::string()>>& observables,
                 Registry& registry);

  bool handleGet(CivetServer* server, struct mg_connection* conn) override;

 private:
  const std::vector<std::weak_ptr<Collectable>>& collectables_;
  const std::vector<std::function<std::string()>>& observables_;
  Family<Counter>& bytes_transferred_family_;
  Counter& bytes_transferred_;
  Family<Counter>& num_scrapes_family_;
  Counter& num_scrapes_;
  Family<Summary>& request_latencies_family_;
  Summary& request_latencies_;
};
}  // namespace detail
}  // namespace prometheus
