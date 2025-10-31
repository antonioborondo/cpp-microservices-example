#include <iostream>
#include <string>

#include "users.h"

#include <httplib/httplib.h>
#include <nlohmann/json.hpp>

using namespace httplib;
using namespace nlohmann;
using namespace users;

int main() {
  Server server;

  server.Get("/api/v1/users", [](const Request&, Response& response) {
    const auto users{Read()};
    json users_json;
    for (const auto& user : users) {
      users_json.push_back({{"id", user.GetId()},
                            {"user", user.GetUsername()},
                            {"email", user.GetEmail()}});
    }
    json body{{"users", users_json}};

    response.status = 200;
    response.set_content(body.dump(), "application/json");
  });

  server.listen("0.0.0.0", 8082);
}
