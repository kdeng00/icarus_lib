#include <iostream>
#include <string>

#include "example.hpp"

#include "icarus_lib/icarus.h"

using std::cout;


void modify_user(icarus_lib::user &user)
{
    user.username = "thisuserwuschanged100";
    user.firstname = "Zen";
}

void modify_salt(icarus_lib::pass_sec &salt)
{
    salt.salt = "sdfom$435fdS!!SDFd";
}

int main(int argc, char **argv)
{
    const std::string name("update_users");

    example::count_check(argc, name);
    const auto conn_str = example::test_connection_string<conn_string>(argv);

    auto user_repo = icarus_lib::user_repository(conn_str);
    auto user = icarus_lib::user();
    user.id = 2;
    auto salt = icarus_lib::pass_sec();
    salt.id = 2;

    user = user_repo.retrieveUserRecord(user, icarus_lib::user_filter::ID);
    salt = user_repo.retrieverUserSaltRecord(salt, icarus_lib::salt_filter::ID);

    modify_user(user);
    modify_salt(salt);

    auto result = user_repo.update_salt(salt);
    result = user_repo.update_user(user);

    return 0;
}
