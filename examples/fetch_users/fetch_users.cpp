#include <iostream>

#include "example.hpp"

#include "icarus_lib/icarus.hpp"

using std::cout;

using icarus_lib::conn_string;
using icarus_lib::types::salt_filter;
using icarus_lib::types::user_filter;

int main(int argc, char **argv)
{
    const std::string name("fetch_users");

    example::count_check(argc, name);
    const auto conn_str = example::test_connection_string<conn_string>(argv);

    auto user_repo = icarus_lib::user_repository(conn_str);

    auto usr = icarus_lib::user();
    usr.id = 1;
    usr = user_repo.retrieveUserRecord(usr, user_filter::ID);

    cout << "Username: " << usr.username << "\n";

    auto salt = icarus_lib::pass_sec();
    salt.user_id = usr.id;
    salt = user_repo.retrieverUserSaltRecord(salt, salt_filter::ID);

    cout << "Salt: " << salt.salt << "\n";


    return 0;
}
