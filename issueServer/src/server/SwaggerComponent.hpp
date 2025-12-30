/**
 * @file SwaggerComponent.hpp
 * @brief Configures Swagger document info and UI resources for issues.
 */
#ifndef its_issue_SwaggerComponent_hpp
#define its_issue_SwaggerComponent_hpp

// c++ system
#include <memory>

// project
#include "Constants.hpp"
#include "oatpp-swagger/Model.hpp"
#include "oatpp-swagger/Resources.hpp"
#include "oatpp/core/macro/component.hpp"

namespace its { namespace issue {

/**
 * @brief Provides Swagger components for Issue Service API docs.
 */
class SwaggerComponent {
 public:
  /**
   *  General API docs info
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::DocumentInfo>, swaggerDocumentInfo)(
      Qualifiers::SERVICE_ISSUE, [] {
    oatpp::swagger::DocumentInfo::Builder builder;

    builder
      .setTitle("Issue Service")
      .setDescription("Handles Issues")
      .setVersion("1.0")
      .addServer("http://localhost:9100", "server on localhost");

    return builder.build();
  }());

  /**
   *  Swagger-Ui Resources (<oatpp-examples>/lib/oatpp-swagger/res)
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::Resources>, swaggerResources)(
      Qualifiers::SERVICE_ISSUE, [] {
    // Make sure to specify correct full path to oatpp-swagger/res folder !!!
    return oatpp::swagger::Resources::streamResources("/usr/local/include/oatpp-1.3.0/bin/oatpp-swagger/res");
  }());
};
}} // namespace its

#endif /* its_issue_SwaggerComponent_hpp */
