/**
 * @file Issue.hpp
 * @brief Declares Issue and IssueModel service classes.
 */
#ifndef ISSUE_H
#define ISSUE_H

#include <vector>
#include <algorithm>
#include <string>

#include "service/user/User.hpp"
#include "service/comment/Comment.hpp"

/**
 * @brief Represents an issue with comments, labels, and assignment.
 */
class Issue {
 private:
  int _id; ///< Unique identifier for the issue.
  std::string _title; ///< Title of the issue.
  std::vector<Comment> comments; ///< List of comments (first is description).
  std::vector<std::string> labels; ///< Labels/tags attached to the issue.
  /// Assigned user stored by value. Avoids dangling pointers.
  User assignedUser;
  std::string priority; ///< Default Priority of Issues.
  std::string status; ///< Default Status of Issues.

 public:
  /**
   * @brief Default constructor.
   */
  Issue();

  /**
   * @brief Sets the issue ID.
   * @param id The new ID.
   */
  void SetID(int id);

  /**
   * @brief Sets the issue description (stored as comment[0]).
   * @param description The description text.
   */
  void SetDescription(const std::string& description);

  /**
   * @brief Sets the issue title.
   * @param title The new title.
   */
  void SetTitle(const std::string& title);

  /**
   * @brief Gets the issue ID.
   * @return The issue's ID.
   */
  int GetID() const;

  /**
   * @brief Gets the issue title.
   * @return The issue's title.
   */
  const std::string& GetTitle() const;

  /**
   * @brief Gets the issue description.
   * @return The description (comment[0]) or "No description" if empty.
   */
  std::string GetDescription() const;

  /**
   * @brief Gets a string representation of the assigned user.
   * @return User ID and name, or "No User Assigned".
   */
  std::string GetUser() const;

  /**
   * @brief Displays all comments except the description.
   * @return A formatted string of comments.
   */
  std::string displayAllComments() const;

  /**
   * @brief Assigns a user to the issue.
   * @param user Pointer to the user (not modified).
   */
  void SetUser(const User* user);

  /**
   * @brief Adds a comment to the issue.
   * @param comment The comment to add.
   */
  void AddComment(const Comment& comment);

  /**
   * @brief Gets the ID of the assigned user.
   * @return The user ID, or 0 if none assigned.
   */
  int getAssignedUserID() const;

  /**
   * @brief Gets the full list of comments.
   * @return A vector of comments.
   */
  std::vector<Comment>& getCommentsVector();
  const std::vector<Comment>& getCommentsVector() const;

  /**
   * @brief Deletes a comment at the given index.
   * @param index The index of the comment to delete.
   * @return True if successful, false otherwise.
   */
  bool DeleteCommentAt(int index);

  /**
   * @brief Updates a comment at the given index.
   * @param index The index of the comment to update.
   * @param newContent The new content.
   * @return True if successful, false otherwise.
   */
  bool UpdateCommentAt(int index, const std::string& newContent);
  /**
   * @brief Updates the priority of an issue
   * @param new priority to set
   */
  void SetPriority(const std::string& newPriority);
  /**
   * @brief Updates the status of an issue
   * @param new status to set
   */
  void SetStatus(const std::string& newStatus);
  /**
   * @brief Gets the issue priority.
   * @return The issue's priority.
   */
  const std::string& GetPriority() const;
  /**
   * @brief Gets the issue status.
   * @return The issue's status.
   */
  const std::string& GetStatus() const;
  /**
   * @brief Add a label to the issue if not present.
   */
  void AddLabel(const std::string& label);
  /**
   * @brief Remove a label from the issue.
   * @return true if removed, false if not found.
   */
  bool RemoveLabel(const std::string& label);
  /**
   * @brief Get all labels.
   */
  const std::vector<std::string>& GetLabels() const;
};

/**
 * @class IssueModel
 * @brief Manages a collection of issues.
 *
 * Provides functionality to add, find, update, and delete issues.
 */
/**
 * @brief Manages a collection of issues and related operations.
 */
class IssueModel {
 private:
  std::vector<Issue> issues; ///< List of all issues.

 public:
  /**
   * @brief Adds an issue to the model.
   * @param issue The issue to add.
   */
  void AddIssue(const Issue& issue);

  /**
   * @brief Finds an issue by ID.
   * @param id The ID to search for.
   * @return Pointer to the issue if found, nullptr otherwise.
   */
  Issue* FindIssueByID(int id);

  /**
   * @brief Gets the full list of issues.
   * @return A vector of all issues.
   */
  const std::vector<Issue>& GetIssuesVector() const;

  /**
   * @brief Update the title of an issue by ID.
   * @param id The issue ID.
   * @param newTitle The new title.
   * @return true if updated, false if issue not found.
   */
  bool UpdateIssueTitle(int id, const std::string& newTitle);

  /**
   * @brief Delete an issue by ID.
   * @param id The issue ID.
   * @return true if deleted, false if not found.
   */
  bool DeleteIssueByID(int id);
};

#endif
// Issue
Issue::Issue() : _id(0), _title(""), comments(), assignedUser(), priority("Low"), status("New") {}

// DisplaySelf removed (not used). Use view/TextUI to present Issue information.

void Issue::SetID(int id) { _id = id; }

void Issue::SetDescription(const std::string& description) {
  if (comments.empty()) {
    Comment c; c.SetContent(description); comments.insert(comments.begin(), c);
  } else {
    comments[0].SetContent(description);
  }
}

void Issue::SetTitle(const std::string& title) { _title = title; }

int Issue::GetID() const { return _id; }
const std::string& Issue::GetTitle() const { return _title; }

std::string Issue::GetDescription() const {
  if (comments.empty()) return "No description";
  return comments[0].GetContent();
}

std::string Issue::GetUser() const {
  if (assignedUser.GetID() == 0) return "No User Assigned";
  return "ID: " + std::to_string(assignedUser.GetID()) + ", Name: " + assignedUser.GetName();
}



void Issue::SetUser(const User* user) { if (user) assignedUser = *user; else assignedUser = User(); }

void Issue::AddComment(const Comment& comment) { comments.push_back(comment); }

int Issue::getAssignedUserID() const { return assignedUser.GetID(); }

std::vector<Comment>& Issue::getCommentsVector() { return comments; }

const std::vector<Comment>& Issue::getCommentsVector() const { return comments; }

bool Issue::DeleteCommentAt(int index) {
  if (index <= 0 || index >= static_cast<int>(comments.size())) return false;
  comments.erase(comments.begin() + index);
  return true;
}

bool Issue::UpdateCommentAt(int index, const std::string& newContent) {
  if (index < 0 || index >= static_cast<int>(comments.size())) return false;
  comments[index].SetContent(newContent);
  return true;
}

void Issue::SetPriority(const std::string& newPriority) { priority = newPriority; }
const std::string& Issue::GetPriority() const { return priority; }

void Issue::SetStatus(const std::string& newStatus) { status = newStatus; }
const std::string& Issue::GetStatus() const { return status; }

std::string Issue::displayAllComments() const {
  if (comments.size() <= 1) return "No comments\n";
  std::string result = "Comments:\n";
  for (size_t i = 1; i < comments.size(); ++i) result += std::to_string(i) + ". " + comments[i].GetContent() + "\n";
  return result;
}

void IssueModel::AddIssue(const Issue& issue) {
  issues.push_back(issue);
}

Issue* IssueModel::FindIssueByID(int id) {
  auto it = std::find_if(issues.begin(), issues.end(),
                         [id](const Issue& i){ return i.GetID() == id; });
  if (it != issues.end()) {
    return &(*it);
  }
  return nullptr;
}

const std::vector<Issue>& IssueModel::GetIssuesVector() const {
  return issues;
}

bool IssueModel::UpdateIssueTitle(int id, const std::string& newTitle) {
  Issue* issue = FindIssueByID(id);
  if (issue) {
    issue->SetTitle(newTitle);
    return true;
  }
  return false;
}

bool IssueModel::DeleteIssueByID(int id) {
  auto it = std::remove_if(issues.begin(), issues.end(),
                           [id](const Issue& i) { return i.GetID() == id; });
  if (it != issues.end()) {
    issues.erase(it, issues.end());
    return true;
  }
  return false;
}

void Issue::AddLabel(const std::string& label) {
  if (label.empty()) return;
  auto it = std::find(labels.begin(), labels.end(), label);
  if (it == labels.end()) labels.push_back(label);
}

bool Issue::RemoveLabel(const std::string& label) {
  auto it = std::remove(labels.begin(), labels.end(), label);
  if (it == labels.end()) return false;
  labels.erase(it, labels.end());
  return true;
}

const std::vector<std::string>& Issue::GetLabels() const { return labels; }


