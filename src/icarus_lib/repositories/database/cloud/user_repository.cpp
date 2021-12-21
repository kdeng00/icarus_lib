#include "icarus_lib/repositories/database/cloud/user_repository.hpp"

#include "mysql/mysql.h"

using std::shared_ptr;
using std::string;
using std::stringstream;


namespace icarus_lib::database
{


user_repository::user_repository(const models::conn_string &conn_str, const std::string table) : 
    base_repository(conn_str, table)
{
}

user_repository::user_repository(const models::binary_path &conn_str, const std::string table) : 
    base_repository(conn_str, table)
{
}



models::user user_repository::retrieveUserRecord(models::user &usr, types::user_filter filter)
{
    auto conn = this->setup_connection();
    auto stmt = ::mysql_stmt_init(conn);

    std::stringstream qry;
    qry << "SELECT * FROM User WHERE ";

    auto params = this->mysql_bind_init(1);

    auto userLength = usr.username.size();
    switch (filter) {
    case types::user_filter::ID:
        qry << "UserId = ?";

        repository_utility::construct_param_long(params, MYSQL_TYPE_LONG, usr.id, 0);

        break;
    case types::user_filter::USERNAME:
        qry << "Username = ?";

        repository_utility::construct_param_string(params, MYSQL_TYPE_STRING, usr.username, 0, userLength);
        break;
    default:
        break;
    }

    qry << " LIMIT 1";

    const auto query = qry.str();
    auto status = ::mysql_stmt_prepare(stmt, query.c_str(), query.size());
    status = ::mysql_stmt_bind_param(stmt, params.get());
    status = ::mysql_stmt_execute(stmt);

    usr = parseRecord(stmt);

    this->close_mysql_connection(conn, stmt);

    return usr;
}


void user_repository::saveUserRecord(const models::user &user)
{
    std::cout << "inserting user record\n";
    MYSQL *conn = this->setup_connection();
    auto stmt = ::mysql_stmt_init(conn);

    std::stringstream qry;
    qry << "INSERT INTO User(Firstname, Lastname, Phone, Email, Username, Password) ";
    qry << "VALUES(?, ?, ?, ?, ?, ?)";

    const auto query = qry.str();
    auto sizes = this->fetchUserLengths(user);
    auto params = this->insertUserValues(user, sizes);

    auto status = ::mysql_stmt_prepare(stmt, query.c_str(), query.size());
    status = ::mysql_stmt_bind_param(stmt, params.get());
    status = ::mysql_stmt_execute(stmt);

    this->close_mysql_connection(conn, stmt);
}


void user_repository::saveUserSalt(const models::pass_sec &userSec)
{
    std::cout << "inserting user salt record\n";

    auto conn = this->setup_connection();
    auto stmt = ::mysql_stmt_init(conn);

    std::stringstream qry;
    qry << "INSERT INTO Salt(Salt, UserId) VALUES(?,?)";

    const auto query = qry.str();

    auto sizes = this->fetchSaltLengths(userSec);
    auto values = this->insertSaltValues(userSec, sizes);

    auto status = ::mysql_stmt_prepare(stmt, query.c_str(), query.size());
    status = ::mysql_stmt_bind_param(stmt, values.get());
    status = ::mysql_stmt_execute(stmt);

    this->close_mysql_connection(conn, stmt);
}



models::user user_repository::parseRecord(MYSQL_STMT *stmt)
{
    models::user user;
    auto usv = this->fetchUV();

    auto bindedValues = this->valueBind(user, usv);
    auto status = ::mysql_stmt_bind_result(stmt, bindedValues.get());
    status = ::mysql_stmt_fetch(stmt);

    user.firstname = std::get<0>(usv);
    user.lastname = std::get<1>(usv);
    user.email = std::get<2>(usv);
    user.phone = std::get<3>(usv);
    user.username = std::get<4>(usv);
    user.password = std::get<5>(usv);

    return user;
}



std::tuple<char*, char*, char*, char*, char*, char*> user_repository::fetchUV()
{
    char firstname[1024];
    char lastname[1024];
    char phone[1024];
    char email[1024];
    char username[1024];
    char password[1024];

    return std::make_tuple(firstname, lastname, phone, email, username, password);
}

/**

std::shared_ptr<MYSQL_BIND> user_repository::insertSaltValues(const PassSec &passSec, 
    std::shared_ptr<SaltLengths> lengths)
{
    auto values = this->mysql_bind_init(6);

    auto mysql_type = MYSQL_TYPE_STRING;

    repository_utility::construct_user_insert_string(values, mysql_type, user.firstname, 0, lengths->firstnameLength);
    repository_utility::construct_user_insert_string(values, mysql_type, user.lastname, 1, lengths->lastnameLength);
    repository_utility::construct_user_insert_string(values, mysql_type, user.phone, 2, lengths->phoneLength);
    repository_utility::construct_user_insert_string(values, mysql_type, user.email, 3, lengths->emailLength);
    repository_utility::construct_user_insert_string(values, mysql_type, user.username, 4, lengths->usernameLength);
    repository_utility::construct_user_insert_string(values, mysql_type, user.password, 5, lengths->passwordLength);

    return values;
}
*/


std::shared_ptr<MYSQL_BIND> user_repository::insertUserValues(
    const models::user &user, std::shared_ptr<UserLengths> lengths)
{
    std::shared_ptr<MYSQL_BIND> values((MYSQL_BIND*) std::calloc(6, sizeof(MYSQL_BIND)));
    /**
    auto userLen = std::make_shared<UserLengths>();
    userLen->firstnameLength = user.firstname.size();
    userLen->lastnameLength = user.lastname.size();
    userLen->phoneLength = user.phone.size();
    userLen->emailLength = user.email.size();
    userLen->usernameLength = user.username.size();
    userLen->passwordLength = user.password.size();
    */
    auto mysql_type = MYSQL_TYPE_STRING;

    repository_utility::construct_user_insert_string(values, mysql_type, user.firstname, 0, lengths->firstnameLength);
    repository_utility::construct_user_insert_string(values, mysql_type, user.lastname, 1, lengths->lastnameLength);
    repository_utility::construct_user_insert_string(values, mysql_type, user.phone, 2, lengths->phoneLength);
    repository_utility::construct_user_insert_string(values, mysql_type, user.email, 3, lengths->emailLength);
    repository_utility::construct_user_insert_string(values, mysql_type, user.username, 4, lengths->usernameLength);
    repository_utility::construct_user_insert_string(values, mysql_type, user.password, 5, lengths->passwordLength);

    // return //userLen;
    return values;
}


std::shared_ptr<MYSQL_BIND> user_repository::insertSaltValues(const models::pass_sec &passSec, 
        std::shared_ptr<SaltLengths> lengths)
{
    std::shared_ptr<MYSQL_BIND> values((MYSQL_BIND*) std::calloc(6, sizeof(MYSQL_BIND)));

    repository_utility::construct_user_insert_string(values, MYSQL_TYPE_STRING, passSec.hash_password, 0, lengths->saltLength);
    repository_utility::construct_user_insert_long(values, MYSQL_TYPE_LONG, passSec.user_id, 1);

    return values;
}


std::shared_ptr<MYSQL_BIND> user_repository::valueBind(models::user &user, 
        std::tuple<char*, char*, char*, char*, char*, char*> &us)
{
    std::shared_ptr<MYSQL_BIND> values((MYSQL_BIND*) std::calloc(7, sizeof(MYSQL_BIND)));
    long unsigned int strLen = 1024;

    repository_utility::construct_param_bind_long(values, MYSQL_TYPE_LONG, user.id, 0);
    repository_utility::construct_param_bind_cstring(values, MYSQL_TYPE_STRING, std::get<0>(us), 1, strLen);
    repository_utility::construct_param_bind_cstring(values, MYSQL_TYPE_STRING, std::get<1>(us), 2, strLen);
    repository_utility::construct_param_bind_cstring(values, MYSQL_TYPE_STRING, std::get<2>(us), 3, strLen);
    repository_utility::construct_param_bind_cstring(values, MYSQL_TYPE_STRING, std::get<3>(us), 4, strLen);
    repository_utility::construct_param_bind_cstring(values, MYSQL_TYPE_STRING, std::get<4>(us), 5, strLen);
    repository_utility::construct_param_bind_cstring(values, MYSQL_TYPE_STRING, std::get<5>(us), 6, strLen);

    return values;
}


std::shared_ptr<MYSQL_BIND> user_repository::saltValueBind(models::pass_sec &userSalt, char *salt)
{
    std::shared_ptr<MYSQL_BIND> values((MYSQL_BIND*) std::calloc(3, sizeof(MYSQL_BIND)));
    long unsigned int strLen = 1024;
    auto tmp = &salt;

    repository_utility::construct_param_bind_long(values, MYSQL_TYPE_LONG, userSalt.id, 0);
    repository_utility::construct_param_bind_cstring(values, MYSQL_TYPE_STRING, &(*tmp)[0], 1, strLen);
    repository_utility::construct_param_bind_long(values, MYSQL_TYPE_LONG, userSalt.user_id, 2);

    return values;
}

shared_ptr<UserLengths> user_repository::fetchUserLengths(const models::user &user)
{
    auto userLen = std::make_shared<UserLengths>();
    userLen->firstnameLength = user.firstname.size();
    userLen->lastnameLength = user.lastname.size();
    userLen->phoneLength = user.phone.size();
    userLen->emailLength = user.email.size();
    userLen->usernameLength = user.username.size();
    userLen->passwordLength = user.password.size();

    return userLen;
}

std::shared_ptr<SaltLengths> user_repository::fetchSaltLengths(const models::pass_sec &pass_sec)
{
    auto saltLen = std::make_shared<SaltLengths>();
    saltLen->saltLength = pass_sec.salt.size();

    return saltLen;
}


models::pass_sec user_repository::parseSaltRecord(MYSQL_STMT *stmt)
{
    models::pass_sec userSalt;
    char saltKey[1024];

    auto bindedValues = this->saltValueBind(userSalt, saltKey);
    auto status = ::mysql_stmt_bind_result(stmt, bindedValues.get());
    status = ::mysql_stmt_fetch(stmt);

    userSalt.salt = saltKey;

    return userSalt;
}

}