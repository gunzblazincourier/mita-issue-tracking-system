/**
 * @file UserController.hpp
 * @brief Declares REST API endpoints for user operations.
 */
#ifndef its_user_UserController_hpp
#define its_user_UserController_hpp

// c++ system
#include <memory>
#include <string>

// project
#include "server/Constants.hpp"
#include "dto/UserDTOs.hpp"
#include "service/user/User.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

namespace its { namespace user { namespace controller {

/**
 * @brief Controller providing CRUD endpoints for users.
 */

#include OATPP_CODEGEN_BEGIN(ApiController)

class UserController : public oatpp::web::server::api::ApiController {
 public:
  UserController(
      OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper,
                      Qualifiers::SERVICE_USER))
      : oatpp::web::server::api::ApiController(objectMapper) {}

 private:
  UserModel users;

 public:
  /**
   * @brief Update username of a user by ID.
   */
  ENDPOINT_INFO(updateUsername) {
    info->summary = "Update username of a user by ID";
    info->addResponse<String>(Status::CODE_200, "text/plain");
    info->addResponse<String>(Status::CODE_400, "text/plain");
    info->addResponse<String>(Status::CODE_404, "text/plain");
  }

  ADD_CORS(
      updateUsername,
      "*",
      "GET, POST, PUT, DELETE, OPTIONS",
      "DNT, User-Agent, X-Requested-With, If-Modified-Since, Cache-Control, Content-Type, Range",
      "1728000")
  /**
   * @brief PUT /users/{id}
   * @param id User identifier
   * @param name New username
   */
  ENDPOINT("PUT", "/users/{id}", updateUsername,
           PATH(Int64, id), QUERY(String, name)) {
    OATPP_ASSERT_HTTP(name, Status::CODE_400, "'name' is required!");
    User* u = users.FindUserByID(static_cast<int>(id));
    if (!u) return createResponse(Status::CODE_404, "'id' not found");
    u->SetName(name ? std::string(name->c_str()) : std::string());
    return createResponse(Status::CODE_200, "Username updated!");
  }

  /**
   * @brief Delete user by ID.
   */
  ENDPOINT_INFO(deleteUser) {
    info->summary = "Delete user by ID";
    info->addResponse<String>(Status::CODE_200, "text/plain");
    info->addResponse<String>(Status::CODE_404, "text/plain");
  }

  ADD_CORS(
      deleteUser,
      "*",
      "GET, POST, PUT, DELETE, OPTIONS",
      "DNT, User-Agent, X-Requested-With, If-Modified-Since, Cache-Control, Content-Type, Range",
      "1728000")
  /**
   * @brief DELETE /users/{id}
   * @param id User identifier
   */
  ENDPOINT("DELETE", "/users/{id}", deleteUser,
           PATH(Int64, id)) {
    bool removed = users.DeleteUserByID(static_cast<int>(id));
    if (!removed) return createResponse(Status::CODE_404, "User not found");
    return createResponse(Status::CODE_200, "User deleted");
  }


  /**
   * @brief Create a user.
   */
  ENDPOINT_INFO(createUser) {
    info->summary = "Create a user";
    info->addResponse<String>(Status::CODE_201, "text/plain");
    info->addResponse<String>(Status::CODE_400, "text/plain");
  }

  ADD_CORS(createUser,
      "*",
      "GET, POST, PUT, DELETE, OPTIONS",
      "DNT, User-Agent, X-Requested-With, If-Modified-Since, Cache-Control, Content-Type, Range",
      "1728000")
  /**
   * @brief POST /users
   * @param id New user id
   * @param name New user name
   */
  ENDPOINT("POST", "/users", createUser, QUERY(Int64, id), QUERY(String, name)) {
    OATPP_ASSERT_HTTP(id, Status::CODE_400, "'id' is required!");
    OATPP_ASSERT_HTTP(name, Status::CODE_400, "'name' is required!");

    if (users.FindUserByID(static_cast<int>(id))) {
      return createResponse(Status::CODE_400, "'id' already exists!");
    }
    User u;
    u.SetID(static_cast<int>(id));
    u.SetName(name ? std::string(name->c_str()) : std::string());
    users.AddUser(u);
    return createResponse(Status::CODE_201, "User created!");
  }

  /**
   * @brief Get user by ID.
   */
  ENDPOINT_INFO(getUserById) {
    info->summary = "Get user by ID";
    info->addResponse<Object<dto::UserDTO>>(Status::CODE_200, "application/json");
    info->addResponse<String>(Status::CODE_404, "text/plain");
  }

  ADD_CORS(getUserById,
      "*",
      "GET, POST, PUT, DELETE, OPTIONS",
      "DNT, User-Agent, X-Requested-With, If-Modified-Since, Cache-Control, Content-Type, Range",
      "1728000")
  /**
   * @brief GET /users/{id}
   * @param id User identifier
   */
  ENDPOINT("GET", "/users/{id}", getUserById, PATH(Int64, id)) {
    User* u = users.FindUserByID(static_cast<int>(id));
    if (!u) return createResponse(Status::CODE_404, "'id' not found");
    auto dto = dto::UserDTO::createShared();
    dto->id = u->GetID();
    dto->name = u->GetName().c_str();
    return createDtoResponse(Status::CODE_200, dto);
  }

  /**
   * @brief List all users.
   */
  ENDPOINT_INFO(listUsers) {
    info->summary = "List all users";
    info->addResponse<List<Object<dto::UserDTO>>>(Status::CODE_200, "application/json");
  }

  ADD_CORS(listUsers,
      "*",
      "GET, POST, PUT, DELETE, OPTIONS",
      "DNT, User-Agent, X-Requested-With, If-Modified-Since, Cache-Control, Content-Type, Range",
      "1728000")
  /**
   * @brief GET /users
   */
  ENDPOINT("GET", "/users", listUsers) {
    auto list = oatpp::List<oatpp::Object<dto::UserDTO>>::createShared();
    for (const auto& u : users.GetUsersVector()) {
      auto dto = dto::UserDTO::createShared();
      dto->id = u.GetID();
      dto->name = u.GetName().c_str();
      list->push_back(dto);
    }
    return createDtoResponse(Status::CODE_200, list);
  }
};

#include OATPP_CODEGEN_END(ApiController)

}}} // namespace controller

#endif // its_user_UserController_hpp
