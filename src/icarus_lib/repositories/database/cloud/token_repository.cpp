#include "icarus_lib/repositories/database/cloud/token_repository.hpp"


namespace icarus_lib::database
{

token_repository::token_repository(const models::conn_string &conn_str, const std::string table) : 
        base_repository(conn_str, table)
{
}
token_repository::token_repository(const models::binary_path &conn_str, const std::string table) : 
        base_repository(conn_str, table) 
{
}



models::token token_repository::retrieve_token(models::token &tokn, types::token_filter filter)
{
    std::stringstream qry;
    auto conn = this->setup_connection();
    auto stmt = ::mysql_stmt_init(conn);

    auto valueFilterCount = 1;

    MYSQL_BIND params[valueFilterCount];
    memset(params, 0, sizeof(params));

    qry << "SELECT * FROM Token WHERE TokenID = ?";
    params[0].buffer_type = MYSQL_TYPE_LONG;
    params[0].buffer = (char *)&tokn.token_id;
    params[0].length = 0;
    params[0].is_null = 0;

    const auto query = qry.str();
    auto status = ::mysql_stmt_prepare(stmt, query.c_str(), query.size());
    status = ::mysql_stmt_bind_param(stmt, params);
    status = ::mysql_stmt_execute(stmt);

    auto token = parseRecord(stmt);

    ::mysql_stmt_close(stmt);
    ::mysql_close(conn);

    return tokn;
}

int token_repository::create_token(const models::token &tokn)
{
    auto conn = this->setup_connection();
    auto stmt = ::mysql_stmt_init(conn);

    std::stringstream buff;
    buff << "INSERT INTO " << this->table_name << " (AccessToken, ";
    buff << "TokenType, UserId) VALUES (?, ?, ?)";
    // buff << "Issued, ";
    // buff << "Expires, UserId) VALUES(:access_token, ";
    // buff << ":originally_issued, :originally_expires, :user_id)";

    const auto query = buff.str();

    auto status = ::mysql_stmt_prepare(stmt, query.c_str(), query.size());

    MYSQL_BIND params[3];
    memset(params, 0, sizeof(params));

    params[0].buffer_type = MYSQL_TYPE_STRING;
    params[0].buffer = (char*)tokn.access_token.c_str();
    auto tokenLength = tokn.access_token.size();
    params[0].length = &tokenLength;
    params[0].is_null = 0;

    params[1].buffer_type = MYSQL_TYPE_STRING;
    params[1].buffer = (char*)tokn.token_type.c_str();
    auto tokenTypeLength = tokn.token_type.size();
    params[1].length = &tokenTypeLength;
    params[1].is_null = 0;

    params[2].buffer_type = MYSQL_TYPE_LONG;
    params[2].buffer = (char*)&tokn.user_id;
    params[2].length = 0;
    params[2].is_null = 0;


    /**
    auto issued = convert_time_point_to_tm<std::chrono::system_clock::time_point,
        std::tm>(tokn.issued);
    auto expires = convert_time_point_to_tm<std::chrono::system_clock::time_point,
        std::tm>(tokn.expires);
    */

    status = ::mysql_stmt_bind_param(stmt, params);
    status = ::mysql_stmt_execute(stmt);
    ::mysql_stmt_store_result(stmt);
    const auto rows_affected = ::mysql_stmt_num_rows(stmt);

    ::mysql_stmt_close(stmt);
    ::mysql_close(conn);


    return rows_affected;
}

// Deletes Token record by id
int token_repository::delete_token(const models::token &tokn, types::token_filter filter)
{
    auto conn = this->setup_connection();
    auto status = 0;
    std::stringstream qry;
    qry << "DELETE FROM Token WHERE TokenId = " + tokn.token_id;
    auto result = perform_query(conn, qry.str());

    ::mysql_close(conn);


    return result == 0 ? true : false;
}


bool token_repository::token_exists(const models::token &tokn)
{
    std::stringstream qry;
    auto conn = this->setup_connection();
    auto stmt = ::mysql_stmt_init(conn);

    auto valueFilterCount = 1;

    MYSQL_BIND params[valueFilterCount];
    memset(params, 0, sizeof(params));

    qry << "SELECT * FROM Token WHERE TokenID = ?";
    params[0].buffer_type = MYSQL_TYPE_LONG;
    params[0].buffer = (char *)&tokn.token_id;
    params[0].length = 0;
    params[0].is_null = 0;

    const auto query = qry.str();
    auto status = ::mysql_stmt_prepare(stmt, query.c_str(), query.size());
    status = ::mysql_stmt_bind_param(stmt, params);
    status = ::mysql_stmt_execute(stmt);

    ::mysql_stmt_store_result(stmt);
    const auto rowcount = ::mysql_stmt_num_rows(stmt);

    ::mysql_stmt_close(stmt);
    ::mysql_close(conn);

    return rowcount > 0 ? true : false;
}


    
models::token token_repository::parseRecord(MYSQL_STMT *stmt)
{
    ::mysql_stmt_store_result(stmt);
    std::cout << "amount of rows: " << ::mysql_stmt_num_rows(stmt) << "\n";

    models::token token;
    auto metaBuff = metadataBuffer();
    auto bindedValues = valueBind(token, metaBuff);
    auto status = ::mysql_stmt_bind_result(stmt, bindedValues.get());
    status = ::mysql_stmt_fetch(stmt);

    token.access_token = std::get<0>(metaBuff);
    token.token_type = std::get<1>(metaBuff);
    
    std::cout << "done parsing record\n";

    return token;
}

std::shared_ptr<MYSQL_BIND> token_repository::valueBind(models::token &token,
        std::tuple<char*, char*> &metadata)
{
    constexpr auto valueCount = 4;
    constexpr auto wordLen = 1024;
    std::shared_ptr<MYSQL_BIND> values((MYSQL_BIND*) 
            std::calloc(valueCount, sizeof(MYSQL_BIND)));

    values.get()[0].buffer_type = MYSQL_TYPE_LONG;
    values.get()[0].buffer = reinterpret_cast<char*>(&token.token_id);

    values.get()[1].buffer_type = MYSQL_TYPE_STRING;
    values.get()[1].buffer = reinterpret_cast<char*>(std::get<0>(metadata));
    values.get()[1].buffer_length = wordLen;


    values.get()[2].buffer_type = MYSQL_TYPE_STRING;
    values.get()[2].buffer = reinterpret_cast<char*>(std::get<1>(metadata));
    values.get()[2].buffer_length = wordLen;

    values.get()[3].buffer_type = MYSQL_TYPE_LONG;
    values.get()[3].buffer = reinterpret_cast<char*>(&token.user_id);

    return values;
}

std::tuple<char *, char *> token_repository::metadataBuffer()
{
    constexpr auto wordLen = 1024;
    char accessToken[wordLen];
    char tokenType[wordLen];

    return std::make_pair(accessToken, tokenType);
}

}
