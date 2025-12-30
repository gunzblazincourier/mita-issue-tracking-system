/**
 * @file SwaggerComponent.hpp
 * @brief Configures Swagger document info and UI resources.
 */
#ifndef its_user_SwaggerComponent_hpp
#define its_user_SwaggerComponent_hpp

// c++ system
#include <memory>

// project headers
#include "Constants.hpp"
#include "oatpp-swagger/Model.hpp"
#include "oatpp-swagger/Resources.hpp"
#include "oatpp/core/macro/component.hpp"

namespace its { namespace user {

/**
 * @brief Provides Swagger components for API docs.
 */
class SwaggerComponent {
 public:
  /**
   *  General API docs info
   */
    OATPP_CREATE_COMPONENT(
      std::shared_ptr<oatpp::swagger::DocumentInfo>, swaggerDocumentInfo)(
      Qualifiers::SERVICE_USER, [] {
    oatpp::swagger::DocumentInfo::Builder builder;

    builder
      .setTitle("User Service")
      .setDescription("Handles ITS Users")
      .setVersion("1.0")
      .addServer("http://localhost:9120", "server on localhost");

    return builder.build();
  }());


  /**
   *  Swagger-Ui Resources (<oatpp-examples>/lib/oatpp-swagger/res)
   */
    OATPP_CREATE_COMPONENT(
      std::shared_ptr<oatpp::swagger::Resources>, swaggerResources)(
      Qualifiers::SERVICE_USER, [] {
        // Make sure to specify correct full path to oatpp-swagger/res folder !!!
        return oatpp::swagger::Resources::streamResources(
            "/usr/local/include/oatpp-1.3.0/bin/oatpp-swagger/res");
      }());
};
}} // namespace user


#endif /* its_user_SwaggerComponent_hpp */
