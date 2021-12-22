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
#include <cstdlib>


#include "mysql/mysql.h"
#include "nlohmann/json.hpp"

#include "icarus_lib/models/models.hpp"

namespace icarus_lib::database {


class base_repository
{
protected:
    base_repository() = default;
    base_repository(const models::conn_string &details, const std::string &table_name);
    base_repository(const models::binary_path &bConf, const std::string &table_name);

    bool test_connection();


    std::string generate_connection_string() noexcept;


    MYSQL *setup_connection();

    std::shared_ptr<MYSQL_BIND> mysql_bind_init(const int size);

    std::pair<MYSQL *, MYSQL_STMT *> mysql_init_connection();

    MYSQL_RES *perform_query(MYSQL *conn, const std::string &query);

    void close_mysql_connection(MYSQL *connection, MYSQL_STMT *statement);
    


    models::conn_string details;
    models::binary_path m_bConf;
    std::string table_name;

    void initialize_base(const models::binary_path &bConf);
private:
    std::string path;
};

}

#endif
