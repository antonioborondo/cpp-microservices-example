#ifndef USERS_USER_H_
#define USERS_USER_H_

#include <string>

namespace users {
class User {
  int id_;
  std::string username_;
  std::string email_;

 public:
  User(int id, const std::string& username, const std::string& email);
  int GetId() const;
  std::string GetUsername() const;
  void SetUsername(const std::string& username);
  std::string GetEmail() const;
  void SetEmail(const std::string& email);
};
}  // namespace users

#endif
