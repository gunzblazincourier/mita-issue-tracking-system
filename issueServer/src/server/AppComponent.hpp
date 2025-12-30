/**
 * @file AppComponent.hpp
 * @brief Declares application components and service wiring for issues.
 */
#ifndef its_issue_AppComponent_hpp
#define its_issue_AppComponent_hpp

// c++ system
#include <memory>

// project
#include "SwaggerComponent.hpp"

#include "service/UserService.hpp"

#include "oatpp/web/server/HttpRouter.hpp"
#include "oatpp/web/client/HttpRequestExecutor.hpp"

#include "oatpp/network/virtual_/server/ConnectionProvider.hpp"
#include "oatpp/network/virtual_/client/ConnectionProvider.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/network/tcp/client/ConnectionProvider.hpp"
#include "oatpp/network/ConnectionPool.hpp"

#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

#include "oatpp/core/macro/component.hpp"

namespace its { namespace issue {

/**
 * @brief Host and port pair used to configure providers.
 */
struct HostPort {
  oatpp::String host;
  v_uint16 port;
};

/**
 * @brief Application wiring: router, mappers, providers, and user service.
 */
class AppComponent {
 private:
  HostPort m_issue;
  HostPort m_userService;

 public:
  AppComponent(const HostPort& issue, const HostPort& userService)
      : m_issue(issue), m_userService(userService) {}

  /**
   * Swagger component
   */
  SwaggerComponent swaggerComponent;

  /**
   * Create virtualhost interface
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::virtual_::Interface>,
                         virtualInterface)(Qualifiers::SERVICE_ISSUE, [] {
    return oatpp::network::virtual_::Interface::obtainShared("issue.virtualhost");
  }());

  /**
   * Create "real-port" connection provider
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>,
                         serverConnectionProvider)(Qualifiers::SERVICE_ISSUE, [this] {
    return oatpp::network::tcp::server::ConnectionProvider::createShared(
      {m_issue.host, m_issue.port});
  }());


  /**
   * Create "virtualhost" connection provider
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>,
                         virtualConnectionProvider)(Qualifiers::SERVICE_ISSUE_VH, [] {
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::virtual_::Interface>, interface, Qualifiers::SERVICE_ISSUE);
    return oatpp::network::virtual_::server::ConnectionProvider::createShared(
      interface);
  }());

  /**
   * Create Router component
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter)(
      Qualifiers::SERVICE_ISSUE, [] {
        return oatpp::web::server::HttpRouter::createShared();
      }());

  /**
   * Create ObjectMapper component to serialize/deserialize DTOs in Contoller's API
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper)(
      Qualifiers::SERVICE_ISSUE, [] {
        return oatpp::parser::json::mapping::ObjectMapper::createShared();
      }());

  /**
   * Create UserService component
   */

  OATPP_CREATE_COMPONENT(std::shared_ptr<service::UserService>, userService)(
      Qualifiers::SERVICE_ISSUE, [this] {
    OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper, Qualifiers::SERVICE_ISSUE);

    std::shared_ptr<oatpp::network::ClientConnectionProvider> connectionProvider;

    if (m_userService.port == 0) {
      auto interface = oatpp::network::virtual_::Interface::obtainShared(m_userService.host);
      connectionProvider = oatpp::network::virtual_::client::ConnectionProvider::createShared(interface);
    } else {
        connectionProvider = oatpp::network::tcp::client::ConnectionProvider::createShared(
          {m_userService.host, m_userService.port});
    }

    auto connectionPool = oatpp::network::ClientConnectionPool::createShared(
      connectionProvider, 10, std::chrono::seconds(5));

    auto requestExecutor = oatpp::web::client::HttpRequestExecutor::createShared(connectionPool);
    return service::UserService::createShared(requestExecutor, objectMapper);
  }());
};

}} // namespace issue

#endif /* its_issue_AppComponent_hpp */
