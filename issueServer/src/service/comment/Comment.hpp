/**
 * @file Comment.hpp
 * @brief Declares a simple comment model used by issues.
 */
#ifndef COMMENT_H
#define COMMENT_H

#include <string>

/**
 * @class Comment
 * @brief Represents a comment attached to an issue.
 */
class Comment {
 private:
  std::string content; ///< The content of the comment.

 public:
  /**
   * @brief Sets the comment content.
   * @param input The new content.
   */
  void SetContent(const std::string& input);

  /**
   * @brief Gets the comment content.
   * @return The content of the comment.
   */
  const std::string& GetContent() const;
};



#endif
// Comment
void Comment::SetContent(const std::string& input) { content = input; }
const std::string& Comment::GetContent() const { return content; }
