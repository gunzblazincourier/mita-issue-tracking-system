/**
 * @file IssueController.hpp
 * @brief Declares REST endpoints for issue operations.
 */
#ifndef its_issue_IssueController_hpp
#define its_issue_IssueController_hpp

// c++ system
#include <memory>
#include <string>

// project
#include "server/Constants.hpp"
#include "dto/IssueDTOs.hpp"
#include "service/issue/Issue.hpp"
#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "service/UserService.hpp"

namespace its { namespace issue { namespace controller {

/**
 * @brief Controller providing CRUD and related endpoints for issues.
 */

#include OATPP_CODEGEN_BEGIN(ApiController)

class IssueController : public oatpp::web::server::api::ApiController {
 private:
  OATPP_COMPONENT(std::shared_ptr<::its::issue::service::UserService>, userServiceClient,
                  Qualifiers::SERVICE_ISSUE);
  IssueModel issues;

 public:
  IssueController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper, Qualifiers::SERVICE_ISSUE))
    : oatpp::web::server::api::ApiController(objectMapper) {}

  /**
   * @brief Create a new issue.
   */
  ENDPOINT_INFO(createIssue) {
    info->summary = "Create an issue";
    info->addResponse<String>(Status::CODE_201, "text/plain");
    info->addResponse<String>(Status::CODE_404, "text/plain");
  }

  ADD_CORS(createIssue)
  /**
   * @brief POST /issues
   * @param id Issue id
   * @param title Issue title
   * @param description Issue description
   */
  ENDPOINT("POST", "/issues", createIssue,
           QUERY(Int64, id), QUERY(String, title), QUERY(String, description)) {
    OATPP_ASSERT_HTTP(id, Status::CODE_400, "'id' is required!");
    OATPP_ASSERT_HTTP(title, Status::CODE_400, "'title' is required!");
    OATPP_ASSERT_HTTP(description, Status::CODE_400, "'description' is required!");

    Issue* i = issues.FindIssueByID(id);
    if (i) {
      return createResponse(Status::CODE_400, "'id' already exists!");
    }

    Issue issue;
    issue.SetID(static_cast<int>(id));
    issue.SetTitle(title);
    issue.SetDescription(description);
    issues.AddIssue(issue);
    return createResponse(Status::CODE_201, "Issue created!");
  }

  /**
   * @brief Get an issue by id.
   */
  ENDPOINT_INFO(getIssueById) {
    info->summary = "Get issue by ID";
    info->addResponse<Object<dto::IssueDTO>>(Status::CODE_200, "application/json");
    info->addResponse<String>(Status::CODE_404, "text/plain");
  }
  ADD_CORS(getIssueById)
  /**
   * @brief GET /issues/ID/{id}
   * @param id Issue identifier
   */
  ENDPOINT("GET", "/issues/ID/{id}", getIssueById, PATH(Int64, id)) {
    Issue* issue = issues.FindIssueByID(static_cast<int>(id));
    if (!issue) return createResponse(Status::CODE_404, "'id' doesn't exist!");
    auto dto = dto::IssueDTO::createShared();
    dto->id = issue->GetID();
    dto->title = issue->GetTitle().c_str();
    dto->description = issue->GetDescription().c_str();
    dto->assignedUserId = issue->getAssignedUserID();
    dto->assignedUserName = issue->GetUser().c_str();
    {
      auto ls = oatpp::List<oatpp::String>::createShared();
      for (const auto& l : issue->GetLabels()) {
        ls->push_back(oatpp::String(l.c_str()));
      }
      dto->labels = ls;
    }
    dto->priority = issue->GetPriority().c_str();
    dto->status = issue->GetStatus().c_str();
    return createDtoResponse(Status::CODE_200, dto);
  }

  /**
   * @brief List all issues.
   */
  ENDPOINT_INFO(listIssues) {
    info->summary = "List all issues";
    info->addResponse<List<Object<dto::IssueDTO>>>(Status::CODE_200, "application/json");
  }
  ADD_CORS(listIssues)
  /**
   * @brief GET /issues
   */
  ENDPOINT("GET", "/issues", listIssues) {
    auto list = oatpp::List<oatpp::Object<dto::IssueDTO>>::createShared();
    for (const auto& i : issues.GetIssuesVector()) {
      auto dto = dto::IssueDTO::createShared();
      dto->id = i.GetID();
      dto->title = i.GetTitle().c_str();
      dto->description = i.GetDescription().c_str();
      dto->assignedUserId = i.getAssignedUserID();
      dto->assignedUserName = i.GetUser().c_str();
      {
        auto ls = oatpp::List<oatpp::String>::createShared();
        for (const auto& l : i.GetLabels()) {
          ls->push_back(oatpp::String(l.c_str()));
        }
        dto->labels = ls;
      }
      dto->priority = i.GetPriority().c_str();
      dto->status = i.GetStatus().c_str();
      list->push_back(dto);
    }
    return createDtoResponse(Status::CODE_200, list);
  }

  /**
   * @brief Get all issues assigned to a user.
   */
  ENDPOINT_INFO(getIssuesByUser) {
    info->summary = "Get all issues assigned to a user";
    info->addResponse<List<Object<dto::IssueDTO>>>(Status::CODE_200, "application/json");
    info->addResponse<String>(Status::CODE_500, "text/plain");
  }
    ADD_CORS(getIssuesByUser)
  /**
   * @brief GET /issues/UserID/{userId}
   * @param userId Assigned user identifier
   */
  ENDPOINT("GET", "/issues/UserID/{userId}", getIssuesByUser, PATH(Int64, userId)) {
    auto response = userServiceClient->getUserById(static_cast<int>(userId));
    OATPP_ASSERT_HTTP(response->getStatusCode() == 200, Status::CODE_500, "Unable to find user");

    auto list = oatpp::List<oatpp::Object<dto::IssueDTO>>::createShared();
    for (const auto& i : issues.GetIssuesVector()) {
      auto dto = dto::IssueDTO::createShared();
      dto->id = i.GetID();
      dto->title = i.GetTitle().c_str();
      dto->description = i.GetDescription().c_str();
      dto->assignedUserId = i.getAssignedUserID();
      dto->assignedUserName = i.GetUser().c_str();
      {
        auto ls = oatpp::List<oatpp::String>::createShared();
        for (const auto& l : i.GetLabels()) {
          ls->push_back(oatpp::String(l.c_str()));
        }
        dto->labels = ls;
      }
      dto->priority = i.GetPriority().c_str();
      dto->status = i.GetStatus().c_str();

      if (dto->assignedUserId == userId) {
        list->push_back(dto);
      }
    }
    return createDtoResponse(Status::CODE_200, list);
  }

  /**
   * @brief Assign a user to an issue.
   */
  ENDPOINT_INFO(assignUserToIssue) {
    info->summary = "Assign a user to an issue";
    info->addResponse<Object<dto::IssueDTO>>(Status::CODE_200, "application/json");
    info->addResponse<String>(Status::CODE_404, "text/plain");
    info->addResponse<String>(Status::CODE_500, "text/plain");
  }
  ADD_CORS(assignUserToIssue,
  "*",                               // Allow all origins
  "GET, POST, PUT, DELETE, OPTIONS",  // Allow specific methods
  "DNT, User-Agent, X-Requested-With, If-Modified-Since, Cache-Control, Content-Type, Range", // Allowed headers
  "1728000"                           // Cache pre-flight requests for 20 days (1728000 seconds)
)
//   ADD_CORS(assignUserToIssue)

  /**
   * @brief PUT /issues/{id}/users/assign
   * @param id Issue identifier
   * @param userId User identifier to assign
   */
  ENDPOINT("PUT", "/issues/{id}/users/assign", assignUserToIssue,
              PATH(Int64, id), QUERY(Int64, userId)) {
            Issue* issue = issues.FindIssueByID(static_cast<int>(id));
            if (!issue) return createResponse(Status::CODE_404, "Issue not found");

    auto response = userServiceClient->getUserById(static_cast<int>(userId));
    OATPP_ASSERT_HTTP(response->getStatusCode() == 200, Status::CODE_500, "Unable to find user");

    auto result = response->readBodyToDto<oatpp::Object<dto::UserDTO>>(getDefaultObjectMapper().get());
    std::string userName = std::string(result->name->c_str());

    User u;
    u.SetID(static_cast<int>(userId));
    u.SetName(userName.c_str());
    issue->SetUser(&u);
    return createResponse(Status::CODE_200, "User assigned!");
  }

  /**
   * @brief Remove the assigned user from an issue.
   */
  ENDPOINT_INFO(removeUserFromIssue) {
    info->summary = "Remove the assigned user from an issue";
    info->addResponse<String>(Status::CODE_200, "text/plain");
    info->addResponse<String>(Status::CODE_404, "text/plain");
  }
  ADD_CORS(removeUserFromIssue,
  "*",                               // Allow all origins
  "GET, POST, PUT, DELETE, OPTIONS",  // Allow specific methods
  "DNT, User-Agent, X-Requested-With, If-Modified-Since, Cache-Control, Content-Type, Range", // Allowed headers
  "1728000"                           // Cache pre-flight requests for 20 days (1728000 seconds)
)
//   ADD_CORS(removeUserFromIssue)
  /**
   * @brief PUT /issues/{id}/users/unassign
   * @param id Issue identifier
   */
  ENDPOINT("PUT", "/issues/{id}/users/unassign", removeUserFromIssue,
           PATH(Int64, id)) {
    Issue* issue = issues.FindIssueByID(static_cast<int>(id));
    if (!issue) return createResponse(Status::CODE_404, "Issue not found");
    issue->SetUser(nullptr);
    return createResponse(Status::CODE_200, "User removed from issue");
  }


  /**
   * @brief Update fields of an issue.
   */
  ENDPOINT_INFO(updateIssue) {
    info->summary = "Update a field of issue (ID update disallowed). Please enter '0' to not update field";
    info->addResponse<String>(Status::CODE_200, "text/plain");
    info->addResponse<String>(Status::CODE_404, "text/plain");
  }
  ADD_CORS(updateIssue,
  "*",                               // Allow all origins
  "GET, POST, PUT, DELETE, OPTIONS",  // Allow specific methods
  "DNT, User-Agent, X-Requested-With, If-Modified-Since, Cache-Control, Content-Type, Range", // Allowed headers
  "1728000"                           // Cache pre-flight requests for 20 days (1728000 seconds)
)
  /**
   * @brief PUT /issues/{id}
   * @param id Issue identifier
   */
  ENDPOINT("PUT", "/issues/{id}", updateIssue,
           PATH(Int64, id), QUERY(String, title), QUERY(String, description),
           QUERY(String, priority), QUERY(String, status)) {
    Issue* issue = issues.FindIssueByID(static_cast<int>(id));
    if (!issue) return createResponse(Status::CODE_404, "Issue not found");

    if (title != "0") issue->SetTitle(title);
    if (description != "0") issue->SetDescription(description);
    if (priority != "0") issue->SetPriority(priority);
    if (status != "0") issue->SetStatus(status);
    return createResponse(Status::CODE_200, "Issue Updated!");
  }

  /**
   * @brief Delete an issue by id.
   */
  ENDPOINT_INFO(deleteIssue) {
    info->summary = "Delete an issue by ID";
    info->addResponse<String>(Status::CODE_200, "text/plain");
    info->addResponse<String>(Status::CODE_404, "text/plain");
  }
    ADD_CORS(deleteIssue,
  "*",                               // Allow all origins
  "GET, POST, PUT, DELETE, OPTIONS",  // Allow specific methods
  "DNT, User-Agent, X-Requested-With, If-Modified-Since, Cache-Control, Content-Type, Range", // Allowed headers
  "1728000"                           // Cache pre-flight requests for 20 days (1728000 seconds)
)
  /**
   * @brief DELETE /issues/{id}
   * @param id Issue identifier
   */
  ENDPOINT("DELETE", "/issues/{id}", deleteIssue, PATH(Int64, id)) {
    bool removed = issues.DeleteIssueByID(static_cast<int>(id));
    if (!removed) return createResponse(Status::CODE_404, "Issue not found");
    return createResponse(Status::CODE_200, "Issue deleted");
  }

  /**
   * @brief Add a label to an issue.
   */
  ENDPOINT_INFO(addLabel) {
    info->summary = "Add a label to an issue";
    info->addResponse<String>(Status::CODE_200, "text/plain");
    info->addResponse<String>(Status::CODE_404, "text/plain");
    info->addResponse<String>(Status::CODE_400, "text/plain");
  }
  ADD_CORS(addLabel)
  /**
   * @brief POST /issues/{id}/labels
   * @param id Issue identifier
   * @param label Label text
   */
  ENDPOINT("POST", "/issues/{id}/labels", addLabel,
           PATH(Int64, id), QUERY(String, label)) {
    OATPP_ASSERT_HTTP(label, Status::CODE_400, "'label' is required!");
    Issue* issue = issues.FindIssueByID(static_cast<int>(id));
    if (!issue) return createResponse(Status::CODE_404, "Issue not found");
    issue->AddLabel(std::string(label->c_str()));
    return createResponse(Status::CODE_200, "Label added");
  }

  /**
   * @brief Remove a label from an issue.
   */
  ENDPOINT_INFO(removeLabel) {
    info->summary = "Remove a label from an issue";
    info->addResponse<String>(Status::CODE_200, "text/plain");
    info->addResponse<String>(Status::CODE_404, "text/plain");
    info->addResponse<String>(Status::CODE_400, "text/plain");
  }
  ADD_CORS(removeLabel)
  /**
   * @brief DELETE /issues/{id}/labels
   * @param id Issue identifier
   * @param label Label text
   */
  ENDPOINT("DELETE", "/issues/{id}/labels", removeLabel,
           PATH(Int64, id), QUERY(String, label)) {
    OATPP_ASSERT_HTTP(label, Status::CODE_400, "'label' is required!");
    Issue* issue = issues.FindIssueByID(static_cast<int>(id));
    if (!issue) return createResponse(Status::CODE_404, "Issue not found");
    bool ok = issue->RemoveLabel(std::string(label->c_str()));
    if (!ok) return createResponse(Status::CODE_404, "Label not found");
    return createResponse(Status::CODE_200, "Label removed");
  }

  /**
   * @brief List labels for an issue.
   */
  ENDPOINT_INFO(listLabels) {
    info->summary = "List labels for an issue";
    info->addResponse<List<String>>(Status::CODE_200, "application/json");
    info->addResponse<String>(Status::CODE_404, "text/plain");
  }
  ADD_CORS(listLabels)
  /**
   * @brief GET /issues/{id}/labels
   * @param id Issue identifier
   */
  ENDPOINT("GET", "/issues/{id}/labels", listLabels, PATH(Int64, id)) {
    Issue* issue = issues.FindIssueByID(id);
    if (!issue) return createResponse(Status::CODE_404, "Issue not found");
    auto ls = oatpp::List<oatpp::String>::createShared();
    for (const auto& l : issue->GetLabels()) ls->push_back(oatpp::String(l.c_str()));
    return createDtoResponse(Status::CODE_200, ls);
  }

  /**
   * @brief Add a comment to an issue.
   */
  ENDPOINT_INFO(addComment) {
    info->summary = "Add a comment to an issue";
    info->addResponse<String>(Status::CODE_200, "text/plain");
    info->addResponse<String>(Status::CODE_404, "text/plain");
    info->addResponse<String>(Status::CODE_400, "text/plain");
  }
  ADD_CORS(addComment,   "*",                               // Allow all origins
  "GET, POST, PUT, DELETE, OPTIONS",  // Allow specific methods
  "DNT, User-Agent, X-Requested-With, If-Modified-Since, Cache-Control, Content-Type, Range", // Allowed headers
  "1728000" )
  /**
   * @brief POST /issues/{id}/comments
   * @param id Issue identifier
   * @param content Comment body
   */
  ENDPOINT("POST", "/issues/{id}/comments", addComment,
           PATH(Int64, id), QUERY(String, content)) {
    OATPP_ASSERT_HTTP(content, Status::CODE_400, "'content' is required!");
    Issue* issue = issues.FindIssueByID(id);
    if (!issue) return createResponse(Status::CODE_404, "Issue not found");

    Comment c; c.SetContent(std::string(content->c_str()));
    issue->AddComment(c);
    return createResponse(Status::CODE_200, "Comment added");
  }
  ADD_CORS(listComments)
  /**
   * @brief List comments for an issue.
   */
  ENDPOINT_INFO(listComments) {
    info->summary = "List comments for an issue";
    info->addResponse<List<String>>(Status::CODE_200, "application/json");
    info->addResponse<String>(Status::CODE_404, "text/plain");
  }
  /**
   * @brief GET /issues/{id}/comments
   * @param id Issue identifier
   */
  ENDPOINT("GET", "/issues/{id}/comments", listComments, PATH(Int64, id)) {
    Issue* issue = issues.FindIssueByID(id);
    if (!issue) return createResponse(Status::CODE_404, "Issue not found");
    auto out = oatpp::List<oatpp::String>::createShared();
    const auto& v = issue->getCommentsVector();
    for (const auto& c : v) out->push_back(oatpp::String(c.GetContent().c_str()));
    return createDtoResponse(Status::CODE_200, out);
  }

  /**
   * @brief Update a comment by index.
   */
  ENDPOINT_INFO(updateComment) {
    info->summary = "Update a comment by index";
    info->addResponse<String>(Status::CODE_200, "text/plain");
    info->addResponse<String>(Status::CODE_404, "text/plain");
    info->addResponse<String>(Status::CODE_400, "text/plain");
  }
  ADD_CORS(updateComment, "*",                               // Allow all origins
  "GET, POST, PUT, DELETE, OPTIONS",  // Allow specific methods
  "DNT, User-Agent, X-Requested-With, If-Modified-Since, Cache-Control, Content-Type, Range", // Allowed headers
  "1728000")
  /**
   * @brief PUT /issues/{id}/comments/{index}
   * @param id Issue identifier
   * @param index Comment index
   * @param content New comment body
   */
  ENDPOINT("PUT", "/issues/{id}/comments/{index}", updateComment,
           PATH(Int64, id), PATH(Int64, index), QUERY(String, content)) {
    OATPP_ASSERT_HTTP(content, Status::CODE_400, "'content' is required!");
    Issue* issue = issues.FindIssueByID(static_cast<int>(id));
    if (!issue) return createResponse(Status::CODE_404, "Issue not found");
    bool ok = issue->UpdateCommentAt(static_cast<int>(index), std::string(content->c_str()));
    if (!ok) return createResponse(Status::CODE_404, "Comment not found");
    return createResponse(Status::CODE_200, "Comment updated");
  }

  /**
   * @brief Delete a comment by index.
   */
  ENDPOINT_INFO(deleteComment) {
    info->summary = "Delete a comment by index";
    info->addResponse<String>(Status::CODE_200, "text/plain");
    info->addResponse<String>(Status::CODE_404, "text/plain");
  }
  ADD_CORS(deleteComment, "*",                               // Allow all origins
  "GET, POST, PUT, DELETE, OPTIONS",  // Allow specific methods
  "DNT, User-Agent, X-Requested-With, If-Modified-Since, Cache-Control, Content-Type, Range", // Allowed headers
  "1728000")
  /**
   * @brief DELETE /issues/{id}/comments/{index}
   * @param id Issue identifier
   * @param index Comment index
   */
  ENDPOINT("DELETE", "/issues/{id}/comments/{index}", deleteComment,
           PATH(Int64, id), PATH(Int64, index)) {
    Issue* issue = issues.FindIssueByID(static_cast<int>(id));
    if (!issue) return createResponse(Status::CODE_404, "Issue not found");
    bool ok = issue->DeleteCommentAt(static_cast<int>(index));
    if (!ok) return createResponse(Status::CODE_404, "Comment not found");
    return createResponse(Status::CODE_200, "Comment deleted");
  }
};

#include OATPP_CODEGEN_END(ApiController)

}}}

#endif
