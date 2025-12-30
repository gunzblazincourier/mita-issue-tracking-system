#include "Runner.hpp"
#include <memory>
#include "AppComponent.hpp"
#include "controller/IssueController.hpp"
#include "oatpp-swagger/Controller.hpp"
#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/network/Server.hpp"

namespace its { namespace issue {

Runner::Runner() {
  OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router,
                  Qualifiers::SERVICE_ISSUE);

  oatpp::web::server::api::Endpoints docEndpoints;

  /* Add IssueController */
  docEndpoints.append(
      router->addController(std::make_shared<controller::IssueController>())
          ->getEndpoints());

  OATPP_COMPONENT(std::shared_ptr<oatpp::swagger::DocumentInfo>, documentInfo,
                  Qualifiers::SERVICE_ISSUE);
  OATPP_COMPONENT(std::shared_ptr<oatpp::swagger::Resources>, resources,
                  Qualifiers::SERVICE_ISSUE);

  router->addController(oatpp::swagger::Controller::createShared(
      docEndpoints, documentInfo, resources));
}

void Runner::run(std::list<std::thread>& acceptingThreads) {
  /* Get router component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router,
                  Qualifiers::SERVICE_ISSUE);

  /* Create connection handler */
  auto connectionHandler =
      oatpp::web::server::HttpConnectionHandler::createShared(router);

  acceptingThreads.push_back(std::thread([router, connectionHandler] {
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>,
                    connectionProvider, Qualifiers::SERVICE_ISSUE);
    oatpp::network::Server server(connectionProvider, connectionHandler);
    OATPP_LOGI("issue-service", "server is listening on port '%s",
               connectionProvider->getProperty("port").getData());
    server.run();
  }));

  acceptingThreads.push_back(std::thread([router, connectionHandler] {
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>,
                    connectionProvider, Qualifiers::SERVICE_ISSUE_VH);
    oatpp::network::Server server(connectionProvider, connectionHandler);
    OATPP_LOGI("issue-service",
               "server is listening on virtual interface '%s",
               connectionProvider->getProperty("host").getData());
    server.run();
  }));
}
}}
