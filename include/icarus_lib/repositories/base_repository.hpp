#ifndef BASE_REPOSITORIES_H_
#define BASE_REPOSITORIES_H_

#include <iostream>
#include <string>
#include <sstream>

#include <mysql/mysql.h>

#include "models/models.hpp"

namespace icarus_lib::repositories::database::cloud
{
// TODO: Start off with the base class and move on to the other classes
class base_repository
{
public:
    base_repository() = default;
    base_repository(const std::string &path, const std::string &table_name) :
        path(path), table_name(table_name)
    {
    }
    base_repository(const icarus_lib::models::binary_path &config) :
        m_binConf(config)
    {
    }
protected:
    bool test_connection()
    {
        auto conn = mysql_init(nullptr);
        const auto server = this->details.server.c_str();
        const auto database = this->details.database.c_str();
        const auto username = this->details.username.c_str();
        const auto password = this->details.password.c_str();

        if(!mysql_real_connect(conn, server, username, password, database, 0, nullptr, 0))
        {
            std::cout << "Failed to connect to the database\n";
            return false;
        }

        mysql_close(conn);
        
        return true;
    }


    MYSQL *setup_connection()
    {
        auto conn = mysql_init(nullptr);
        const auto server = this->details.server.c_str();
        const auto database = this->details.database.c_str();
        const auto username = this->details.username.c_str();
        const auto password = this->details.password.c_str();

        if(!mysql_real_connect(conn, server, username, password, database, 0, nullptr, 0))
        {
            std::cout << "Failed to connect to the database\n";
        }

        return conn;
    }
    MYSQL *setup_connection(icarus_lib::models::database_connection details)
    {
        auto conn = mysql_init(nullptr);
        const auto server = details.server.c_str();
        const auto database = details.database.c_str();
        const auto username = details.username.c_str();
        const auto password = details.password.c_str();

        if(!mysql_real_connect(conn, server, username, password, database, 0, nullptr, 0))
        {
            std::cout << "Failed to connect to the database\n";
        }

        return conn;
    }

    MYSQL_RES *perform_query(MYSQL *conn, const std::string &query)
    {
        // send the query to the database
        if (mysql_query(conn, query.c_str()))
        {
            printf("MySQL query error : %s\n", mysql_error(conn));
            std::cout << "MySQL query error : " << mysql_error(conn) << "\n";
            std::exit(1);
        }

        return mysql_use_result(conn);
    }

    std::string generate_connection_string() noexcept
    {
        std::stringstream qry;
        qry << "db=" << this->details.database << " user=";
        qry << this->details.username << " host=" << this->details.server;
        qry << " password='" << this->details.password << "'";

        return qry.str();
    }

    icarus_lib::models::database_connection details;
    std::string table_name;
private:
    // NOTE: The two functions will not be in this class. They imply adding
    // a dependency to this class that isn't necessary
    // void initialize_details();
    // void initialize_details(const icarus_lib::models::binary_path &bConf);

    std::string path;
    icarus_lib::models::binary_path m_binConf;
};
}

#endif
