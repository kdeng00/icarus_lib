#include <iostream>
#include <string>

#include "example.hpp"
#include "icarus_data/icarus_data.h"

using std::cout;
using std::string;

using user = icarus_data::user;
using pass_sec = icarus_data::pass_sec;


user test_user(const string firstname = "David", const string lastname = "Bowie",
    const string &email = "kdeedeng@yahoo.com", const string& phone = "999-333-2321",
    const string &username = "dbowie84", const string &password = "thisisatest") noexcept
{
    auto usr = icarus_data::user();
    usr.firstname = firstname;
    usr.lastname = lastname;
    usr.username = username;
    usr.password = password;
    usr.email = email;
    usr.phone = phone;

    return usr;
}

pass_sec test_salt(const int user_id,
    const string some_salt = "$dkngv.enierunfeic") noexcept
{
    auto salt = icarus_data::pass_sec();
    salt.salt = some_salt;
    salt.hash_password = some_salt;
    salt.user_id = user_id;

    return salt;
}

int main(int argc, char **argv)
{
    const std::string name("create_users");
    example::count_check(argc, name);

    const auto conn_str = example::test_connection_string<conn_string>(argv);
    auto user_repo = icarus_data::user_repository(conn_str);
    auto usr = test_user();

    user_repo.saveUserRecord(usr);
    usr = user_repo.retrieveUserRecord(usr, icarus_data::types::user_filter::USERNAME);

    auto slt = test_salt(usr.id);
    user_repo.saveUserSalt(slt);

    return 0;
}