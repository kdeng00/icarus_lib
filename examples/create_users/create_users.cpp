#include <iostream>
#include <string>

#include "example.hpp"
#include "icarus_lib/icarus.hpp"

using std::cout;
using std::string;

using user = icarus_lib::user;
using pass_sec = icarus_lib::pass_sec;


user test_user(const string firstname = "David", const string lastname = "Bowie",
    const string email = "kdeedeng@yahoo.com", const string phone = "999-333-2321",
    const string username = "dbowie84", const string password = "maybethiswill435342!") noexcept
{
    icarus_lib::user usr;
    usr.firstname = firstname;
    usr.lastname = lastname;
    usr.username = username;
    usr.password = password;
    usr.email = email;
    usr.phone = phone;
    usr.datecreated = icarus_lib::general_utility::covert_time_point_to_time_str<std::chrono::system_clock::time_point, string>();
    cout << usr.datecreated << "\n";

    return usr;
}

pass_sec test_salt(const icarus_lib::user &user) noexcept
{
    icarus_lib::utility::password_encryption pass_encyrpt;
    const auto salt = pass_encyrpt.hash_password(user);
    const auto password = user.password;
    const auto hashed = pass_encyrpt.hash_password(user);
    const auto equal = pass_encyrpt.is_password_valid(user, salt);
    cout << "hashed " << hashed.hash_password << " ";

    cout << "Hash matches " << equal << "\n";

    return salt;
}

int main(int argc, char **argv)
{
    const std::string name("create_users");
    example::count_check(argc, name);

    const auto conn_str = example::test_connection_string<icarus_lib::conn_string>(argv);
    auto user_repo = icarus_lib::user_repository(conn_str);
    auto usr = test_user();

    user_repo.saveUserRecord(usr);
    usr = user_repo.retrieveUserRecord(usr, icarus_lib::user_filter::USERNAME);

    auto slt = test_salt(usr);
    user_repo.saveUserSalt(slt);

    return 0;
}
