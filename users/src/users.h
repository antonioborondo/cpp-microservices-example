#ifndef USERS_USERS_H_
#define USERS_USERS_H_

#include <vector>

#include "user.h"

namespace users {
// CRUD:
//  - Create
//  - Read
//  - Update
//  - Delete
void Create(const std::string& username, const std::string& email);
std::vector<User> Read();
User Read(int id);
void Update(int id, const std::string& username, const std::string& email);
void Delete(int id);
}  // namespace users

#endif
