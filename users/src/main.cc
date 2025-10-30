#include <iostream>
#include <string>

#include <httplib/httplib.h>
#include <mariadb/conncpp.hpp>
#include <nlohmann/json.hpp>

static std::string env_or(const char* k, const char* def) {
    if (const char* v = std::getenv(k); v && *v) return v;
    return def;
}

int main() {
  httplib::Server server;

  server.Get("/api/v1/users", [](const httplib::Request&,
                          httplib::Response& response) {
    std::string result{""};

    try {
      sql::Driver* driver = sql::mariadb::get_driver_instance();

      const std::string host = env_or("DB_HOST", "localhost");

      sql::SQLString url = "jdbc:mariadb://" + host + ":3306/app";

      sql::Properties properties({{"user", "user"}, {"password", "pwd"}});

      std::unique_ptr<sql::Connection> conn(driver->connect(url, properties));

      std::cout << "✅ Connected to MariaDB!" << std::endl;

      std::unique_ptr<sql::Statement> stmt(conn->createStatement());
      std::unique_ptr<sql::ResultSet> res(
          stmt->executeQuery("SELECT * FROM users"));

      nlohmann::json items;
      while (res->next()) {
        items.push_back({{"id", res->getInt("id")},
                         {"username", res->getString("username")},
                         {"email", res->getString("email")},
                         {"created", res->getString("created")}});
      }

      nlohmann::json body{
                {"items", items},
                {"count", items.size()}
            };

      response.status = 200;
      response.set_content(body.dump(), "application/json");
    } catch (sql::SQLException& e) {
            nlohmann::json err{
                {"error", {{"code","db_error"}, {"message", e.what()}}}
            };
            response.status = 500;
            response.set_content(err.dump(), "application/json");
    }
  });

  server.listen("0.0.0.0", 8082);
}
