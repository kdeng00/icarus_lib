#include <iostream>
#include <string>

#include "example.hpp"

#include "icarus_lib/icarus.hpp"

using std::cout;


int main(int argc, char **argv)
{
    const std::string name("delete_users");

    example::count_check(argc, name);
    const auto conn_str = example::test_connection_string<conn_string>(argv);

    auto user_repo = icarus_lib::user_repository(conn_str);
    auto user = icarus_lib::user();
    user.id = 9;
    auto salt = icarus_lib::pass_sec();
    salt.id = 4;

    auto result = user_repo.delete_salt(salt);
    result = user_repo.delete_user(user);

    return 0;
}