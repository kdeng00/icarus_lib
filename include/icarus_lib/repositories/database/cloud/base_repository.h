#ifndef BASE_REPOSITORY_H_
#define BASE_REPOSITORY_H_

#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <memory>
#include <vector>
#include <utility>


#include <mysql/mysql.h>
#include <nlohmann/json.hpp>
#include <soci/soci.h>
#include <soci/soci-backend.h>
// #include <soci/mysql/soci-mysql.h>

#include "icarus_lib/models/models.hpp"

namespace icarus_lib::database {

template<class ConnStr>
class base_repository
{
protected:
    base_repository() = delete;
    base_repository(const ConnStr &details) : details(details)
    {
    }
    base_repository(const ConnStr &details, const std::string &table_name) :
        details(details), table_name(table_name)
    {
    }
    base_repository(const models::binary_path &bConf, const std::string &table_name) : m_bConf(bConf)
    {
        initialize_base<models::binary_path>(bConf);
    }
    base_repository(const std::string &path) :
        path(path)
    {
    }

    bool test_connection()
    {
        auto *conn = setup_connection();

        if(conn == nullptr)
        {
            return false;
        }

        mysql_close(conn);
        
        return true;
    }

    void create_connection(soci::session &conn)
    {
        auto conn_string = generate_connection_string();
        conn.open("mysql", conn_string);
    }


    std::string generate_connection_string() noexcept
    {
        std::stringstream qry;
        qry << "db=" << this->details.database << " user=";
        qry << this->details.username << " host=" << this->details.server;
        qry << " password='" << this->details.password << "'";

        return qry.str();
    }


    MYSQL *setup_connection()
    {
        auto conn = mysql_init(nullptr);
        const auto server = this->details.host.c_str();
        const auto database = this->details.database.c_str();
        const auto username = this->details.username.c_str();
        const auto password = this->details.password.c_str();

        if(!mysql_real_connect(conn, server, username, password, database, 0, nullptr, 0))
        {
            std::cout << "Failed to connect to the database\n";
        }

        return conn;
    }

    std::shared_ptr<MYSQL_BIND> mysql_bind_init(const int size)
    {
        std::shared_ptr<MYSQL_BIND> params((MYSQL_BIND *) std::calloc(size, sizeof(MYSQL_BIND)));

        return params;
    }

    std::pair<MYSQL *, MYSQL_STMT *> mysql_init_connection()
    {
        auto conn = setup_connection();
        auto stmt = mysql_stmt_init(conn);

        return std::make_pair(conn, stmt);
    }

    MYSQL_RES *perform_query(MYSQL *conn, const std::string &query)
    {
        // send the query to the database
        if(mysql_query(conn, query.c_str()))
        {
            std::cout << "MySQL query error : " << mysql_error(conn) << "\n";
            std::exit(1);
        }

        return mysql_use_result(conn);
    }

    void close_mysql_connection(MYSQL *connection, MYSQL_STMT *statement)
    {
        mysql_stmt_close(statement);
        mysql_close(connection);
    }


    ConnStr details;
    models::binary_path m_bConf;
    std::string table_name;
private:
    template<typename BinaryPath>
    void initialize_base(const BinaryPath &bConf)
    {
        auto path = std::filesystem::canonical(bConf.path).parent_path().string();
        path.append("/database.json");
        std::fstream file(path, std::ios::in);
        std::stringstream buff;
        buff << file.rdbuf();
        file.close();
        const auto content = buff.str();
        auto config = nlohmann::json::parse(content);

        this->details.database = config["database"].get<std::string>();
        this->details.host = config["server"].get<std::string>();
        this->details.username = config["username"].get<std::string>();
        this->details.password = config["password"].get<std::string>();
    }

    std::string path;
};

}

#endif
