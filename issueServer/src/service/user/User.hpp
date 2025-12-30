#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include <algorithm>

/**
 * @class User
 * @brief Represents a user in the issue tracking system.
 *
 * Each user has a unique ID and a name. Users can be assigned to issues.
 */
class User {
 private:
  int _id = 0; ///< Unique identifier for the user.
  std::string name = "\0"; ///< Name of the user.

 public:
  /**
   * @brief Default constructor.
   */
  User();

  /**
   * @brief Constructs a user with a specific ID.
   * @param id The user's ID.
   */
  explicit User(int id);

  /**
   * @brief Gets the user's ID.
   * @return The ID of the user.
   */
  int GetID() const;

  /**
   * @brief Compares two users by ID.
   * @param other The user to compare with.
   * @return True if IDs match, false otherwise.
   */
  bool operator==(const User& other) const;

  /**
   * @brief Sets the user's ID.
   * @param id The new ID.
   */
  void SetID(int id);

  /**
   * @brief Gets the user's name.
   * @return The name of the user.
   */
  const std::string& GetName() const;

  /**
   * @brief Sets the user's name.
   * @param n The new name.
   */
  void SetName(const std::string& n);
};

/**
 * @class UserModel
 * @brief Manages a collection of users.
 *
 * Provides functionality to add users and search for them by ID.
 */
class UserModel {
 private:
  std::vector<User> users; ///< List of all users.

 public:
  /**
   * @brief Adds a user to the model.
   * @param user The user to add.
   */
  void AddUser(const User& user);

  /**
   * @brief Finds a user by ID.
   * @param id The ID to search for.
   * @return Pointer to the user if found, nullptr otherwise.
   */
  User* FindUserByID(int id);

  /**
   * @brief Gets the full list of users.
   * @return A vector of all users.
   */
  const std::vector<User>& GetUsersVector() const;

  /**
   * @brief Update a user's name by ID.
   * @return true if updated, false if user not found.
   */
  bool UpdateUserName(int id, const std::string& newName);

  /**
   * @brief Delete a user by ID.
   * @return true if deleted, false if not found.
   */
  bool DeleteUserByID(int id);
};

#endif

User::User() : _id(0), name("") {}
User::User(int id) : _id(id), name("") {}

int User::GetID() const { return _id; }
bool User::operator==(const User& other) const { return _id == other._id; }
void User::SetID(int id) { _id = id; }
const std::string& User::GetName() const { return name; }
void User::SetName(const std::string& n) { name = n; }

// UserModel
void UserModel::AddUser(const User& user) { users.push_back(user); }
User* UserModel::FindUserByID(int id) {
  auto it = std::find_if(users.begin(), users.end(), [id](const User& u){ return u.GetID() == id; });
  return (it != users.end()) ? &(*it) : nullptr;
}
const std::vector<User>& UserModel::GetUsersVector() const { return users; }

bool UserModel::UpdateUserName(int id, const std::string& newName) {
  User* u = FindUserByID(id);
  if (!u) return false;
  u->SetName(newName);
  return true;
}


bool UserModel::DeleteUserByID(int id) {
  auto it = std::remove_if(users.begin(), users.end(), [id](const User& u){ return u.GetID() == id; });
  if (it == users.end()) return false;
  users.erase(it, users.end());
  return true;
}
