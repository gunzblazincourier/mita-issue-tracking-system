/**
 * @file UserService.hpp
 * @brief Declares client for interacting with User Service.
 */
#ifndef example_user_example_userService
#define example_user_example_userService

#include "oatpp/web/client/ApiClient.hpp"
#include "oatpp/core/macro/codegen.hpp"

namespace its { namespace issue { namespace service {

#include OATPP_CODEGEN_BEGIN(ApiClient)

/**
 * @brief API client for User Service operations.
 */
class UserService : public oatpp::web::client::ApiClient {
public:

  API_CLIENT_INIT(UserService)

    /** @brief Create a user in User Service. */
    API_CALL("POST", "/users", createUser, QUERY(Int64, id), QUERY(String, name))
    /** @brief List all users from User Service. */
    API_CALL("GET", "/users", displayAllUsers)
    /** @brief Get a user by id. */
    API_CALL("GET", "/users/{userId}", getUserById, PATH(Int64, userId))
    /** @brief Update a user's name. */
    API_CALL("PUT", "/users/{userId}", updateUsername, PATH(Int64, userId), QUERY(String, name))
    /** @brief Delete a user by id. */
    API_CALL("DELETE", "/users/{userId}", deleteUser, PATH(Int64, userId))
};

#include OATPP_CODEGEN_END(ApiClient)

}}}

#endif //example_user_example_userService
