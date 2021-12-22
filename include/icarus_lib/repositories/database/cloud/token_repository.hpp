#ifndef TOKENREPOSITORY_REPO_H_
#define TOKENREPOSITORY_REPO_H_

#include <memory>
#include <chrono>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <ctime>

#include "icarus_lib/types/types.hpp"
#include "icarus_lib/repositories/database/cloud/base_repository.h"
#include "icarus_lib/repositories/database/cloud/repository_utility.h"


namespace icarus_lib::database {

class token_repository : public base_repository
{
public:
    token_repository(const models::conn_string &conn_str, const std::string table = "Token");
    token_repository(const models::binary_path &conn_str, const std::string table = "Token");


    template<typename Token = models::token, typename Container = std::vector<Token>>
    Container retrieve_all_tokens()
    {
        auto conn = this->setup_connection();
        auto stmt = ::mysql_stmt_init(conn);

        std::stringstream qry;
        qry << "SELECT * FROM " << this->table_name;
        const auto query = qry.str();

        ::mysql_stmt_prepare(stmt, query.c_str(), query.size());
        ::mysql_stmt_execute(stmt);

        auto contr = parse_records<Token, Container>(stmt);

        ::mysql_stmt_close(stmt);
        ::mysql_close(conn);

        return contr;
    }


    models::token retrieve_token(models::token &tokn, types::token_filter filter);
    


    int create_token(const models::token &tokn);
    

    // Deletes Token record by id
    int delete_token(const models::token &tokn, types::token_filter filter);
    


    bool token_exists(const models::token &tokn);
    


private:
    models::token parseRecord(MYSQL_STMT *stmt);
     

    
    template<typename Token, typename Container>
    Container parse_records(MYSQL_STMT *stmt)
    {
        ::mysql_stmt_store_result(stmt);
        const auto rowCount = ::mysql_stmt_num_rows(stmt);
        std::cout << "number of results " << rowCount << "\n";

        Container tokens;
        tokens.reserve(rowCount);

        constexpr auto valAmt = 3;

        for(auto status = 0; status == 0; status = ::mysql_stmt_next_result(stmt))
        {
            if(::mysql_stmt_field_count(stmt) > 0)
            {
                Token token;
                auto metaBuff = metadataBuffer();
                auto bindedValues = valueBind(token, metaBuff);
                status = ::mysql_stmt_bind_result(stmt, bindedValues.get());

                std::cout << "fetching statement result\n";
                status = ::mysql_stmt_fetch(stmt);

                if(status == 1 || status == MYSQL_NO_DATA) break;

                token.access_token = std::get<0>(metaBuff);
                token.token_type  = std::get<1>(metaBuff);
                tokens.push_back(token);
            }
            std::cout << "fetching next result\n";
        }

        return tokens;
    }

    std::shared_ptr<MYSQL_BIND> valueBind(models::token &token,
            std::tuple<char*, char*> &metadata);
    

    std::tuple<char*, char*> metadataBuffer();
    

    template<typename time_point_type, typename tm_type>
    tm_type convert_time_point_to_tm(const time_point_type &time)
    {
        auto tmp_time_t = std::chrono::system_clock::to_time_t(time);

        return *std::localtime(&tmp_time_t);
    }

    template<typename time_point_type, typename tm_type>
    time_point_type convert_tm_to_time_point(tm_type &time)
    {
        auto time_t_val = std::mktime(&time);

        return std::chrono::system_clock::from_time_t(time_t_val);
    }
};

}


#endif
