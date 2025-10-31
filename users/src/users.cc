#include "users.h"

#include <mariadb/conncpp.hpp>

namespace users {

static std::string env_or(const char* k, const char* def) {
  if (const char* v = std::getenv(k); v && *v) return v;
  return def;
}

void Create(const std::string& username, const std::string& email) {
  // Insert into database
  sql::Driver* driver = sql::mariadb::get_driver_instance();

  const std::string host = env_or("DB_HOST", "localhost");

  sql::SQLString url = "jdbc:mariadb://" + host + ":3306/app";

  sql::Properties properties({{"user", "user"}, {"password", "pwd"}});

  std::unique_ptr<sql::Connection> conn(driver->connect(url, properties));
  std::unique_ptr<sql::PreparedStatement> stmt(conn->prepareStatement(
      "INSERT INTO users (username, email) VALUES (?, ?)"));
  stmt->setString(1, username);
  stmt->setString(2, email);
  stmt->execute();

  // Retrieve auto-generated ID
  int new_id = 0;
  std::unique_ptr<sql::Statement> s(conn->createStatement());
  std::unique_ptr<sql::ResultSet> rs(
      s->executeQuery("SELECT LAST_INSERT_ID() AS id"));
  if (rs->next()) new_id = rs->getInt("id");
}

std::vector<User> Read() {
  sql::Driver* driver = sql::mariadb::get_driver_instance();

  const std::string host = env_or("DB_HOST", "localhost");

  sql::SQLString url = "jdbc:mariadb://" + host + ":3306/app";

  sql::Properties properties({{"user", "user"}, {"password", "pwd"}});

  std::unique_ptr<sql::Connection> conn(driver->connect(url, properties));

  std::unique_ptr<sql::Statement> stmt(conn->createStatement());
  std::unique_ptr<sql::ResultSet> res(
      stmt->executeQuery("SELECT * FROM users"));

  std::vector<User> users;

  while (res->next()) {
    users.emplace_back(res->getInt("id"),
                       static_cast<std::string>(res->getString("username")),
                       static_cast<std::string>(res->getString("email")));
  }
  return users;
}

User Read(int id) {
  User dummy(0, "", "");
  return dummy;
}

void Update(int id, const std::string& username, const std::string& email) {}

void Delete(int id) {}
}  // namespace users
