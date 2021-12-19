#ifndef USERREPOSITORY_H_
#define USERREPOSITORY_H_

#include <mysql/mysql.h>

#include "icarus_lib/models/models.hpp"
#include "icarus_lib/repositories/database/cloud/base_repository.hpp"
#include "icarus_lib/repositories/database/cloud/repository_utility.hpp"

using icarus_lib::database::repository_utility;
using icarus_lib::database::base_repository;

namespace icarus_lib::database {

template<class User, class PassSec, typename SaltFilter, typename Filter, class ConnStr>
class user_repository : public base_repository<ConnStr>
// public base_repository<ConnStr>
{
public:
    user_repository(const ConnStr &conn_str, const std::string table = "User") : 
        base_repository<ConnStr>(conn_str, table)
    {
    }
    user_repository(const models::binary_path &conn_str, const std::string table = "User") : 
        base_repository<ConnStr>(conn_str, table)
    {
    }


    User retrieveUserRecord(User &usr, Filter filter)
    {
        auto conn = this->setup_connection();
        auto stmt = mysql_stmt_init(conn);

        std::stringstream qry;
        qry << "SELECT * FROM User WHERE ";

        auto params = this->mysql_bind_init(1);

        auto userLength = usr.username.size();
        switch (filter) {
        case Filter::ID:
            qry << "UserId = ?";

            repository_utility::construct_param_long(params, MYSQL_TYPE_LONG, usr.id, 0);

            break;
        case Filter::USERNAME:
            qry << "Username = ?";

            repository_utility::construct_param_string(params, MYSQL_TYPE_STRING, usr.username, 0, userLength);
           break;
        default:
            break;
        }

        qry << " LIMIT 1";

        const auto query = qry.str();
        auto status = mysql_stmt_prepare(stmt, query.c_str(), query.size());
        status = mysql_stmt_bind_param(stmt, params.get());
        status = mysql_stmt_execute(stmt);

        usr = parseRecord(stmt);

        this->close_mysql_connection(conn, stmt);

        return usr;
    }
    PassSec retrieverUserSaltRecord(PassSec &userSec, SaltFilter filter)
    {
        std::stringstream qry;
        auto conn = this->setup_connection();
        auto stmt = mysql_stmt_init(conn);

        qry << "SELECT * FROM Salt WHERE ";

        auto params = this->mysql_bind_init(1);

        switch (filter) {
        case SaltFilter::ID:
            qry << "UserId = ?";

            repository_utility::construct_param_long(params, MYSQL_TYPE_LONG, userSec.user_id, 0);
            break;
        default:
            break;
        }

        qry << " LIMIT 1";

        const auto query = qry.str();
        auto status = mysql_stmt_prepare(stmt, query.c_str(), query.size());
        status = mysql_stmt_bind_param(stmt, params.get());
        status = mysql_stmt_execute(stmt);

        userSec = parseSaltRecord(stmt);

        this->close_mysql_connection(conn, stmt);

        return userSec;
    }

    bool doesUserRecordExist(const User &user, Filter filter)
    {
        std::stringstream qry;
        auto conn = this->setup_connection();
        auto stmt = mysql_stmt_init(conn);

        qry << "SELECT * FROM User WHERE ";

        auto params = this->mysql_bind_init(1);

        auto userLength = user.username.size();
        switch (filter) {
        case Filter::username:
            qry << "Username = ?";

            repository_utility::construct_param_long(params, MYSQL_TYPE_STRING, user.username, 0, userLength);
            break;
        default:
            break;
        }

        qry << " LIMIT 1";

        const auto query = qry.str();
        auto status = mysql_stmt_prepare(stmt, query.c_str(), query.size());
        status = mysql_stmt_bind_param(stmt, params.get());
        status = mysql_stmt_execute(stmt);

        mysql_stmt_store_result(stmt);
        const auto rowCount = mysql_stmt_num_rows(stmt);

        this->close_mysql_connection(conn, stmt);

        return (rowCount > 0) ? true : false;
    }

    void saveUserRecord(const User &user)
    {
        std::cout << "inserting user record\n";
        auto conn = this->setup_connection();
        auto stmt = mysql_stmt_init(conn);

        std::stringstream qry;
        qry << "INSERT INTO User(Firstname, Lastname, Phone, Email, Username, Password) ";
        qry << "VALUES(?, ?, ?, ?, ?, ?)";

        const auto query = qry.str();
        auto sizes = fetchUserLengths(user);
        auto params = insertUserValues(user, sizes);

        auto status = mysql_stmt_prepare(stmt, query.c_str(), query.size());
        status = mysql_stmt_bind_param(stmt, params.get());
        status = mysql_stmt_execute(stmt);

        this->close_mysql_connection(conn, stmt);
    }

    void saveUserSalt(const PassSec &userSec)
    {
        std::cout << "inserting user salt record\n";

        auto conn = this->setup_connection();
        auto stmt = mysql_stmt_init(conn);

        std::stringstream qry;
        qry << "INSERT INTO Salt(Salt, UserId) VALUES(?,?)";

        const auto query = qry.str();

        auto sizes = fetchSaltLengths(userSec);
        auto values = insertSaltValues(userSec, sizes);

        auto status = mysql_stmt_prepare(stmt, query.c_str(), query.size());
        status = mysql_stmt_bind_param(stmt, values.get());
        status = mysql_stmt_execute(stmt);

        this->close_mysql_connection(conn, stmt);
    }

    int delete_user(const User &user)
    {
        auto conn = this->setup_connection();
        auto stmt = mysql_stmt_init(conn);

        std::stringstream qry;
        qry << "DELETE FROM " << this->table_name << " WHERE UserId = ?";
        const auto query = qry.str();

        auto params = this->mysql_bind_init(1);

        repository_utility::construct_param_long(params, MYSQL_TYPE_LONG, user.id, 0);

        auto status = mysql_stmt_prepare(stmt, query.c_str(), query.size());
        status = mysql_stmt_bind_param(stmt, params.get());
        status = mysql_stmt_execute(stmt);

        this->close_mysql_connection(conn, stmt);

        return status;
    }

    int update_user(const User &user)
    {
        auto conn = this->setup_connection();
        auto stmt = mysql_stmt_init(conn);

        std::stringstream qry;
        qry << "UPDATE " << this->table_name << " SET Firstname = ?, LastName = ?, Email = ?, ";
        qry << "Phone = ?, Username = ?, Password = ? ";
        qry << "WHERE UserId = ?";
        const auto query = qry.str();

        auto status = mysql_stmt_prepare(stmt, query.c_str(), query.size());

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
        status = mysql_stmt_execute(stmt);

        this->close_mysql_connection(conn, stmt);

        return status;
    }

    int delete_salt(const PassSec &userSec)
    {
        auto conn = this->setup_connection();
        auto stmt = mysql_stmt_init(conn);

        std::stringstream qry;
        qry << "DELETE FROM Salt WHERE SaltId = ?";
        const auto query = qry.str();

        auto params = this->mysql_bind_init(1);

        repository_utility::construct_param_long(params, MYSQL_TYPE_LONG, userSec.id, 0);

        auto status = mysql_stmt_prepare(stmt, query.c_str(), query.size());
        status = mysql_stmt_bind_param(stmt, params.get());
        status = mysql_stmt_execute(stmt);

        this->close_mysql_connection(conn, stmt);

        return status;
    }

    int update_salt(const PassSec &userSec)
    {
        auto conn = this->setup_connection();
        auto stmt = mysql_stmt_init(conn);

        std::stringstream qry;
        qry << "UPDATE Salt SET Salt = ?, UserId = ? WHERE SaltId = ?";
        const auto query = qry.str();

        auto status = mysql_stmt_prepare(stmt, query.c_str(), query.size());

        auto params = this->mysql_bind_init(3);
        auto mysql_string = MYSQL_TYPE_STRING;
        auto mysql_long = MYSQL_TYPE_LONG;

        auto salt_length = userSec.salt.size();

        repository_utility::construct_param_string(params, mysql_string, userSec.salt, 0, salt_length);
        repository_utility::construct_param_long(params, mysql_long, userSec.user_id, 1);
        repository_utility::construct_param_long(params, mysql_long, userSec.id, 2);

        status = mysql_stmt_bind_param(stmt, params.get());
        status = mysql_stmt_execute(stmt);

        this->close_mysql_connection(conn, stmt);

        return status;
    }
private:
    struct UserLengths;
    struct SaltLengths;

    struct UserLengths {
        unsigned long firstnameLength;
        unsigned long lastnameLength;
        unsigned long phoneLength;
        unsigned long emailLength;
        unsigned long usernameLength;
        unsigned long passwordLength;
    };
    struct SaltLengths {
        unsigned long saltLength;
    };


    std::shared_ptr<MYSQL_BIND> insertUserValues(const User &user, 
            std::shared_ptr<UserLengths> lengths)
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

    std::shared_ptr<MYSQL_BIND> insertSaltValues(const PassSec &passSec, 
            std::shared_ptr<SaltLengths> lengths)
    {
        std::shared_ptr<MYSQL_BIND> values((MYSQL_BIND*) std::calloc(6, sizeof(MYSQL_BIND)));

        repository_utility::construct_user_insert_string(values, MYSQL_TYPE_STRING, passSec.hash_password, 0, lengths->saltLength);
        repository_utility::construct_user_insert_long(values, MYSQL_TYPE_LONG, passSec.user_id, 1);

        return values;
    }

    std::shared_ptr<MYSQL_BIND> valueBind(User &user, 
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
    std::shared_ptr<MYSQL_BIND> saltValueBind(PassSec &userSalt, char *salt)
    {
        std::shared_ptr<MYSQL_BIND> values((MYSQL_BIND*) std::calloc(3, sizeof(MYSQL_BIND)));
        long unsigned int strLen = 1024;
        auto tmp = &salt;

        repository_utility::construct_param_bind_long(values, MYSQL_TYPE_LONG, userSalt.id, 0);
        repository_utility::construct_param_bind_cstring(values, MYSQL_TYPE_STRING, &(*tmp)[0], 1, strLen);
        repository_utility::construct_param_bind_long(values, MYSQL_TYPE_LONG, userSalt.user_id, 2);

        return values;
    }
    std::shared_ptr<UserLengths> fetchUserLengths(const User &user)
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
    std::shared_ptr<SaltLengths> fetchSaltLengths(const PassSec &pass_sec)
    {
        auto saltLen = std::make_shared<SaltLengths>();
        saltLen->saltLength = pass_sec.salt.size();

        return saltLen;
    }

    std::tuple<char*, char*, char*, char*, char*, char*> fetchUV()
    {
        char firstname[1024];
        char lastname[1024];
        char phone[1024];
        char email[1024];
        char username[1024];
        char password[1024];

        return std::make_tuple(firstname, lastname, phone, email, username, password);
    }

    User parseRecord(MYSQL_STMT *stmt)
    {
        User user;
        auto usv = fetchUV();

        auto bindedValues = valueBind(user, usv);
        auto status = mysql_stmt_bind_result(stmt, bindedValues.get());
        status = mysql_stmt_fetch(stmt);

        user.firstname = std::get<0>(usv);
        user.lastname = std::get<1>(usv);
        user.email = std::get<2>(usv);
        user.phone = std::get<3>(usv);
        user.username = std::get<4>(usv);
        user.password = std::get<5>(usv);

        return user;
    }

    PassSec parseSaltRecord(MYSQL_STMT *stmt)
    {
        PassSec userSalt;
        char saltKey[1024];

        auto bindedValues = saltValueBind(userSalt, saltKey);
        auto status = mysql_stmt_bind_result(stmt, bindedValues.get());
        status = mysql_stmt_fetch(stmt);

        userSalt.salt = saltKey;

        return userSalt;
    }
};

}

#endif
