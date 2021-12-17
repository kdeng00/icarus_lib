#include <iostream>
#include <string>

#include "example.hpp"

#include "icarus_data/icarus_data.h"

using std::cout;


int main(int argc, char **argv)
{
    const std::string name("delete_users");

    example::count_check(argc, name);
    const auto conn_str = example::test_connection_string<conn_string>(argv);

    auto user_repo = icarus_data::user_repository(conn_str);
    auto user = icarus_data::user();
    user.id = 9;
    auto salt = icarus_data::pass_sec();
    salt.id = 4;

    auto result = user_repo.delete_salt(salt);
    result = user_repo.delete_user(user);

    return 0;
}