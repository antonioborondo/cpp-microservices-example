#include "user.h"

using namespace std;

namespace users {
User::User(int id, const string& username, const string& email)
    : id_{id}, username_{username}, email_{email} {}
int User::GetId() const { return id_; }
string User::GetUsername() const { return username_; }
void User::SetUsername(const string& username) { username_ = username; }
string User::GetEmail() const { return email_; }
void User::SetEmail(const string& email) { email_ = email; }
}  // namespace users
