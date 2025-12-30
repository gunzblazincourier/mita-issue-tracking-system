/**
 * @file UserDTOs.hpp
 * @brief Declares DTOs used by user endpoints.
 */
#ifndef its_user_UserDTOs_hpp
#define its_user_UserDTOs_hpp

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

namespace its { namespace user { namespace dto {

/**
 * @brief Data Transfer Object for user payloads.
 */

#include OATPP_CODEGEN_BEGIN(DTO)

class UserDTO : public oatpp::DTO {
  DTO_INIT(UserDTO, DTO)

  DTO_FIELD(Int64, id, "id");
  DTO_FIELD(String, name, "name");
};

#include OATPP_CODEGEN_END(DTO)

}}} // namespace dto

#endif // its_user_UserDTOs_hpp
