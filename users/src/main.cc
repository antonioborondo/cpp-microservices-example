#include <mariadb/conncpp.hpp>
#include <iostream>

int main() {
    try {
        sql::Driver* driver = sql::mariadb::get_driver_instance();

        sql::SQLString url = "jdbc:mariadb://localhost:3306/app";

        sql::Properties properties({
            {"user", "user"},
            {"password", "pwd"}
        });

        std::unique_ptr<sql::Connection> conn(driver->connect(url, properties));

        std::cout << "✅ Connected to MariaDB!" << std::endl;

        std::unique_ptr<sql::Statement> stmt(conn->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT * FROM users"));

        while (res->next()) {
            std::cout << "User: " << res->getString("username")
                      << "  Email: " << res->getString("email") << std::endl;
        }
    } catch (sql::SQLException& e) {
        std::cerr << "❌ Database error: " << e.what() << std::endl;
        return 1;
    }
}
