#include <iostream>
#include <string>

#include <httplib/httplib.h>
#include <mariadb/conncpp.hpp>

int main() {
  httplib::Server server;

  server.Get("/users", [](const httplib::Request&,
                          httplib::Response& response) {
    std::string result{""};

    try {
      sql::Driver* driver = sql::mariadb::get_driver_instance();

      sql::SQLString url = "jdbc:mariadb://db:3306/app";

      sql::Properties properties({{"user", "user"}, {"password", "pwd"}});

      std::unique_ptr<sql::Connection> conn(driver->connect(url, properties));

      std::cout << "✅ Connected to MariaDB!" << std::endl;

      std::unique_ptr<sql::Statement> stmt(conn->createStatement());
      std::unique_ptr<sql::ResultSet> res(
          stmt->executeQuery("SELECT * FROM users"));

      while (res->next()) {
        result += "User: ";
        result += res->getString("username");
        result += " Email: ";
        result += res->getString("email");
        result += "\n";
      }
    } catch (sql::SQLException& e) {
      result += "❌ Database error: ";
      result += e.what();
      result += "\n";
    }

    response.set_content(result, "text/plain");
  });

  server.listen("0.0.0.0", 8081);
}
