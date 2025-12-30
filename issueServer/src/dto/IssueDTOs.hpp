/**
 * @file IssueDTOs.hpp
 * @brief Declares DTOs for issue and user payloads.
 */
#ifndef ISSUE_DTO_hpp
#define ISSUE_DTO_hpp

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

namespace its { namespace issue { namespace dto {

/**
 * @brief Data Transfer Object representing an Issue.
 */

#include OATPP_CODEGEN_BEGIN(DTO)

class IssueDTO : public oatpp::DTO {
  DTO_INIT(IssueDTO, DTO)

  DTO_FIELD(Int64, id);
  DTO_FIELD(String, title);
  DTO_FIELD(String, description);
  DTO_FIELD(Int64, assignedUserId); // reference to UserServer
  DTO_FIELD(String, assignedUserName); // added for verification
  DTO_FIELD(List<String>, labels); // labels attached to the issue
  DTO_FIELD(String, priority);
  DTO_FIELD(String, status);
};

/**
 * @brief Minimal User DTO used by Issue service.
 */
class UserDTO : public oatpp::DTO {
  DTO_INIT(UserDTO, DTO)

  DTO_FIELD(Int64, id, "id");
  DTO_FIELD(String, name, "name");
};

#include OATPP_CODEGEN_END(DTO)
}}}

#endif /* ISSUE_DTO_hpp */
