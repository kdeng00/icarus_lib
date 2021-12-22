#include "icarus_lib/repositories/database/cloud/base_repository.h"

#include "mysql/mysql.h"

namespace icarus_lib::database
{



base_repository::base_repository(const models::conn_string &details, const std::string &table_name) :
    details(details), table_name(table_name)
{
}

base_repository::base_repository(const models::binary_path &bConf, const std::string &table_name) : m_bConf(bConf)
{
    this->initialize_base(bConf);
}


bool base_repository::test_connection()
{
    auto *conn = this->setup_connection();

    if(conn == nullptr)
    {
        return false;
    }

    ::mysql_close(conn);
    
    return true;
}


std::string base_repository::generate_connection_string() noexcept
{
    std::stringstream qry;
    qry << "db=" << this->details.database << " user=";
    qry << this->details.username << " host=" << this->details.host;
    qry << " password='" << this->details.password << "'";

    return qry.str();
}


void base_repository::initialize_base(const models::binary_path &bConf)
{
    std::cout << "Initializing base...\n";

    auto path = std::filesystem::canonical(bConf.path).parent_path().string();
    path.append("/database.json");
    std::fstream file(path, std::ios::in);
    std::stringstream buff;
    buff << file.rdbuf();
    file.close();
    const auto content = buff.str();
    std::cout << "Content: " << content << "\n";
    auto config = nlohmann::json::parse(content);

    details.database = config["database"].get<std::string>();
    details.host = config["server"].get<std::string>();
    details.username = config["username"].get<std::string>();
    details.password = config["password"].get<std::string>();
}


MYSQL * base_repository::setup_connection()
{
    auto conn = mysql_init(nullptr);
    const auto server = details.host.c_str();
    const auto database = details.database.c_str();
    const auto username = details.username.c_str();
    const auto password = details.password.c_str();

    // std::cout << this->generate_connection_string() << "\n";

    if(!mysql_real_connect(conn, details.host.c_str(), details.username.c_str(), 
        details.password.c_str(), details.database.c_str(), 0, nullptr, 0))
    {
        fprintf(stderr, "Failed to connect to database: Error: %s\n",
        mysql_error(conn));
    }

    return conn;
}


std::shared_ptr<MYSQL_BIND> base_repository::mysql_bind_init(const int size)
{
    std::shared_ptr<MYSQL_BIND> params((MYSQL_BIND *) std::calloc(size, sizeof(MYSQL_BIND)));

    return params;
}


std::pair<MYSQL *, MYSQL_STMT *> base_repository::mysql_init_connection()
{
    auto conn = this->setup_connection();
    auto stmt = ::mysql_stmt_init(conn);

    return std::make_pair(conn, stmt);
}


MYSQL_RES *base_repository::perform_query(MYSQL *conn, const std::string &query)
{
    // send the query to the database
    if(::mysql_query(conn, query.c_str()))
    {
        std::cout << "MySQL query error : " << mysql_error(conn) << "\n";
        std::exit(1);
    }

    return mysql_use_result(conn);
}



void base_repository::close_mysql_connection(MYSQL *connection, MYSQL_STMT *statement)
{
    ::mysql_stmt_close(statement);
    ::mysql_close(connection);
}

}