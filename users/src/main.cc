#include <mariadb/conncpp.hpp>

int main() {
  sql::Driver* driver = sql::mariadb::get_driver_instance();
}
