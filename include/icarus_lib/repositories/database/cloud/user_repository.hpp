#ifndef USERREPOSITORY_H_
#define USERREPOSITORY_H_

#include <tuple>

#include "icarus_lib/repositories/database/cloud/base_repository.h"
#include "icarus_lib/repositories/database/cloud/repository_utility.h"
#include "icarus_lib/types/types.hpp"

namespace icarus_lib::database {


struct UserLengths {
    unsigned long firstnameLength;
    unsigned long lastnameLength;
    unsigned long phoneLength;
    unsigned long emailLength;
    unsigned long usernameLength;
    unsigned long passwordLength;
    unsigned long date_created_length;
};
struct SaltLengths {
    unsigned long saltLength;
};




class user_repository : public base_repository
{
public:
    user_repository(const models::conn_string &conn_str, const std::string table = "User");
    user_repository(const models::binary_path &conn_str, const std::string table = "User");


    using user_values = std::tuple<char *, char *, char *, char *, char *, char *, MYSQL_TIME> ;


    models::user retrieveUserRecord(models::user &usr, types::user_filter filter);
    models::pass_sec retrieverUserSaltRecord(models::pass_sec &userSec, types::salt_filter filter)
    {
        std::stringstream qry;
        auto conn = this->setup_connection();
        auto stmt = ::mysql_stmt_init(conn);

        qry << "SELECT * FROM Salt WHERE ";

        auto params = this->mysql_bind_init(1);

        switch (filter) {
        case types::salt_filter::ID:
            qry << "UserId = ?";

            repository_utility::construct_param_long(params, MYSQL_TYPE_LONG, userSec.user_id, 0);
            break;
        default:
            break;
        }

        qry << " LIMIT 1";

        const auto query = qry.str();
        auto status = ::mysql_stmt_prepare(stmt, query.c_str(), query.size());
        status = ::mysql_stmt_bind_param(stmt, params.get());
        status = ::mysql_stmt_execute(stmt);

        userSec = parseSaltRecord(stmt);

        this->close_mysql_connection(conn, stmt);

        return userSec;
    }

    bool doesUserRecordExist(const models::user &user, types::user_filter filter)
    {
        std::stringstream qry;
        auto conn = this->setup_connection();
        auto stmt = ::mysql_stmt_init(conn);

        qry << "SELECT * FROM User WHERE ";

        auto params = this->mysql_bind_init(1);

        auto userLength = user.username.size();
        switch (filter) {
        case types::user_filter::USERNAME:
            qry << "Username = ?";

            repository_utility::construct_param_string(params, MYSQL_TYPE_STRING, user.username, 0, userLength);
            break;
        default:
            break;
        }

        qry << " LIMIT 1";

        const auto query = qry.str();
        auto status = ::mysql_stmt_prepare(stmt, query.c_str(), query.size());
        status = ::mysql_stmt_bind_param(stmt, params.get());
        status = ::mysql_stmt_execute(stmt);

        ::mysql_stmt_store_result(stmt);
        const auto rowCount = ::mysql_stmt_num_rows(stmt);

        this->close_mysql_connection(conn, stmt);

        return (rowCount > 0) ? true : false;
    }

    void saveUserRecord(const models::user &user);

    void saveUserSalt(const models::pass_sec &userSec);

    int delete_user(const models::user &user)
    {
        auto conn = this->setup_connection();
        auto stmt = ::mysql_stmt_init(conn);

        std::stringstream qry;
        qry << "DELETE FROM " << this->table_name << " WHERE UserId = ?";
        const auto query = qry.str();

        auto params = this->mysql_bind_init(1);

        repository_utility::construct_param_long(params, MYSQL_TYPE_LONG, user.id, 0);

        auto status = ::mysql_stmt_prepare(stmt, query.c_str(), query.size());
        status = ::mysql_stmt_bind_param(stmt, params.get());
        status = ::mysql_stmt_execute(stmt);

        this->close_mysql_connection(conn, stmt);

        return status;
    }

    int update_user(const models::user &user)
    {
        auto conn = this->setup_connection();
        auto stmt = ::mysql_stmt_init(conn);

        std::stringstream qry;
        qry << "UPDATE " << this->table_name << " SET Firstname = ?, LastName = ?, Email = ?, ";
        qry << "Phone = ?, Username = ?, Password = ? ";
        qry << "WHERE UserId = ?";
        const auto query = qry.str();

        auto status = ::mysql_stmt_prepare(stmt, query.c_str(), query.size());

        auto params = this->mysql_bind_init(7);
        auto mysql_string = MYSQL_TYPE_STRING;
        auto mysql_long = MYSQL_TYPE_LONG;

        auto firstname_length = user.firstname.size();
        auto lastname_length = user.lastname.size();
        auto email_length = user.email.size();
        auto phone_length = user.phone.size();
        auto username_length = user.username.size();
        auto password_length = user.password.size();

        repository_utility::construct_param_string(params, mysql_string, user.firstname, 0, firstname_length);
        repository_utility::construct_param_string(params, mysql_string, user.lastname, 1, lastname_length);
        repository_utility::construct_param_string(params, mysql_string, user.email, 2, email_length);
        repository_utility::construct_param_string(params, mysql_string, user.phone, 3, phone_length);
        repository_utility::construct_param_string(params, mysql_string, user.username, 4, username_length);
        repository_utility::construct_param_string(params, mysql_string, user.password, 5, password_length);
        repository_utility::construct_param_long(params, mysql_long, user.id, 6);

        status = mysql_stmt_bind_param(stmt, params.get());
        status = ::mysql_stmt_execute(stmt);

        this->close_mysql_connection(conn, stmt);

        return status;
    }

    int delete_salt(const models::pass_sec &userSec)
    {
        auto conn = this->setup_connection();
        auto stmt = ::mysql_stmt_init(conn);

        std::stringstream qry;
        qry << "DELETE FROM Salt WHERE SaltId = ?";
        const auto query = qry.str();

        auto params = this->mysql_bind_init(1);

        repository_utility::construct_param_long(params, MYSQL_TYPE_LONG, userSec.id, 0);

        auto status = ::mysql_stmt_prepare(stmt, query.c_str(), query.size());
        status = ::mysql_stmt_bind_param(stmt, params.get());
        status = ::mysql_stmt_execute(stmt);

        this->close_mysql_connection(conn, stmt);

        return status;
    }

    int update_salt(const models::pass_sec &userSec)
    {
        auto conn = this->setup_connection();
        auto stmt = ::mysql_stmt_init(conn);

        std::stringstream qry;
        qry << "UPDATE Salt SET Salt = ?, UserId = ? WHERE SaltId = ?";
        const auto query = qry.str();

        auto status = ::mysql_stmt_prepare(stmt, query.c_str(), query.size());

        auto params = this->mysql_bind_init(3);
        auto mysql_string = MYSQL_TYPE_STRING;
        auto mysql_long = MYSQL_TYPE_LONG;

        auto salt_length = userSec.salt.size();

        repository_utility::construct_param_string(params, mysql_string, userSec.salt, 0, salt_length);
        repository_utility::construct_param_long(params, mysql_long, userSec.user_id, 1);
        repository_utility::construct_param_long(params, mysql_long, userSec.id, 2);

        status = mysql_stmt_bind_param(stmt, params.get());
        status = ::mysql_stmt_execute(stmt);

        this->close_mysql_connection(conn, stmt);

        return status;
    }
private:
    std::shared_ptr<MYSQL_BIND> insertUserValues(const models::user &user, 
            std::shared_ptr<UserLengths> lengths);

    std::shared_ptr<MYSQL_BIND> insertSaltValues(const models::pass_sec &passSec, 
            std::shared_ptr<SaltLengths> lengths);

    std::shared_ptr<MYSQL_BIND> valueBind(models::user &user, user_values &us);
    std::shared_ptr<MYSQL_BIND> saltValueBind(models::pass_sec &userSalt, char *salt);
    std::shared_ptr<UserLengths> fetchUserLengths(const models::user &user);
    std::shared_ptr<SaltLengths> fetchSaltLengths(const models::pass_sec &pass_sec);

    // std::tuple<char*, char*, char*, char*, char*, char*, char *> fetchUV();
    user_values fetchUV();

    models::user parseRecord(MYSQL_STMT *stmt);

    models::pass_sec parseSaltRecord(MYSQL_STMT *stmt);

    my_bool m_is_null;
    my_bool m_error;
    unsigned long m_str_length;
};

}

#endif
