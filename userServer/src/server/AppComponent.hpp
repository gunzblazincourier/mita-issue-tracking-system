/**
 * @file AppComponent.hpp
 * @brief Declares application components and providers for the user service.
 */
#ifndef its_user_AppComponent_hpp
#define its_user_AppComponent_hpp

// c++ system
#include <memory>

// project headers
#include "Constants.hpp"

#include "SwaggerComponent.hpp"
#include "../service/user/User.hpp"

#include "oatpp/web/server/HttpRouter.hpp"

#include "oatpp/network/virtual_/server/ConnectionProvider.hpp"
#include "oatpp/network/virtual_/client/ConnectionProvider.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/network/tcp/client/ConnectionProvider.hpp"
#include "oatpp/network/ConnectionPool.hpp"

#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

#include "oatpp/core/macro/component.hpp"

namespace its { namespace user {

/**
 * @brief Host and port pair used for configuring providers.
 */
struct HostPort {
  oatpp::String host;
  v_uint16 port;
};


/**
 * @brief Application wiring: router, mappers, and connection providers.
 */
class AppComponent {
 private:
  HostPort m_hostPort;
  HostPort m_virtualHost;

 public:
  AppComponent(const HostPort& hostPort, const HostPort& virtualHost)
      : m_hostPort(hostPort), m_virtualHost(virtualHost) {}

  /**
   *  Swagger component
   */
  SwaggerComponent swaggerComponent;

  /**
   * Create virtualhost interface
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::virtual_::Interface>,
                         virtualInterface)(Qualifiers::SERVICE_USER, [this] {
    return oatpp::network::virtual_::Interface::obtainShared(m_virtualHost.host);
  }());

  /**
   * Create "real-port" connection provider
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>,
                         serverConnectionProvider)(Qualifiers::SERVICE_USER, [this] {
    return oatpp::network::tcp::server::ConnectionProvider::createShared(
        {m_hostPort.host, m_hostPort.port});
  }());

  /**
   * Create "virtualhost" connection provider
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>,
                         virtualConnectionProvider)(Qualifiers::SERVICE_USER_VH, [] {
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::virtual_::Interface>, interface,
                    Qualifiers::SERVICE_USER);
    return oatpp::network::virtual_::server::ConnectionProvider::createShared(interface);
  }());

  /**
   *  Create Router component
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter)(
      Qualifiers::SERVICE_USER, [] {
        return oatpp::web::server::HttpRouter::createShared();
      }());

  /**
   *  Create ObjectMapper component to serialize/deserialize DTOs in Contoller's API
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>,
                         apiObjectMapper)(Qualifiers::SERVICE_USER, [] {
    return oatpp::parser::json::mapping::ObjectMapper::createShared();
  }());
};

}} // namespace user
#endif /* its_user_AppComponent_hpp */
