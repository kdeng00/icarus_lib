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

#include <mysql/mysql.h>
#include <soci/soci.h>

#include "icarus_lib/models/models.hpp"
#include "icarus_lib/repositories/database/cloud/base_repository.hpp"
#include "icarus_lib/repositories/database/cloud/repository_utility.hpp"

using icarus_lib::repositories::database::cloud::base_repository;
// using icarus_lib::database::repository_utility;


namespace icarus_lib::repositories::database::cloud {

template<class Token, typename Filter, class ConnStr>
class token_repository : public base_repository<ConnStr>
{
public:
    token_repository(const ConnStr &conn_str, const std::string table = "Token") : 
         base_repository<ConnStr>(conn_str, "Token")
    {
    }
    token_repository(const models::binary_path &conn_str, const std::string table = "Token") : 
         base_repository<ConnStr>(conn_str, "Token")
    {
    }


    std::vector<Token> retrieve_all_tokens()
    {
        soci::session conn;
        this->create_connection(conn);

        std::stringstream qry;
        qry << "SELECT * FROM " << this->table_name;

        soci::rowset<soci::row> rows = (conn.prepare << qry.str());

        std::vector<Token> contr;

        for (auto &row : rows)
        {
            contr.emplace_back(parse_record(row));
        }

        conn.close();

        return contr;
    }


    Token retrieve_token(Token &tokn, Filter filter)
    {
        soci::session conn;
        this->create_connection(conn);

        std::shared_ptr<soci::rowset<soci::row>> rows = prepare_query(conn, tokn, filter);

        for (auto &row : *rows)
        {
            auto fetched = parse_record(row);
            tokn.access_token = fetched.access_token;
            tokn.issued = fetched.issued;
            tokn.expires = fetched.expires;

            break;
        }


        conn.close();


        return tokn;
    }


    int create_token(const Token &tokn, const int &usr_id)
    {
        using std::chrono::system_clock;
        using std::tm;

        soci::session conn;
        this->create_connection(conn);

        std::stringstream buff;
        buff << "INSERT INTO " << this->table_name << " (AccessToken, ";
        buff << "Issued, ";
        buff << "Expires, UserId) VALUES(:access_token, ";
        buff << ":originally_issued, :originally_expires, :user_id)";

        const auto query = buff.str();

        auto func1 = this->convert_time_point_to_tm<system_clock::time_point,
                                                       tm>;

        auto issued = func1(tokn.issued);
        auto expires = func1(tokn.expires);

        soci::statement stmt = (conn.prepare << query, soci::use(tokn.access_token, "access_token"),
            soci::use(issued, "originally_issued"), soci::use(expires, "originally_expires"),
            soci::use(usr_id, "user_id"));

        stmt.execute();
        auto rows_affected = stmt.get_affected_rows();

        conn.close();

        return rows_affected;
    }

    // Deletes Token record by id
    int delete_token(const Token &tokn, Filter filter)
    {
        soci::session conn;
        this->create_connection(conn);

        std::shared_ptr<soci::statement> stmt;

        std::stringstream qry;
        qry << "DELETE FROM " << this->table_name << " WHERE ";

        switch(filter)
        {
            case Filter::id:
            {
                qry << "TokenId = :token_id";
                auto stt = (conn.prepare << qry.str(), soci::use(tokn.token_id, "token_id"));
                stmt = std::make_shared<soci::statement>(stt);
                break;
            }
            case Filter::access_token:
            {
                qry << "AccessToken = :access_token";
                auto stt = (conn.prepare << qry.str(), soci::use(tokn.access_token, "access_token"));
                stmt = std::make_shared<soci::statement>(stt);
                break;
            }
            case Filter::user_id:
            {
                qry << "UserId = :user_id";
                auto stt = (conn.prepare << qry.str(), soci::use(tokn.user_id, "user_id"));
                stmt = std::make_shared<soci::statement>(stt);
                break;
            }
            case Filter::token_and_user:
            {
                qry << "TokenId = :token_id AND UserId = :user_id";
                auto stt = (conn.prepare << qry.str(), soci::use(tokn.token_id, "token_id"),
                        soci::use(tokn.user_id, "user_id"));
                stmt = std::make_shared<soci::statement>(stt);
                break;
            }
            default:
            {
                qry << "TokenId = :token_id";
                auto stt = (conn.prepare << qry.str(), soci::use(tokn.token_id, "token_id"));
                stmt = std::make_shared<soci::statement>(stt);
                break;
            }
        }

        stmt->execute();
        const auto affected_rows = stmt->get_affected_rows();


        return affected_rows;
    }


    bool token_exists(const Token &tokn)
    {
        return false;
    }


private:


    std::shared_ptr<soci::rowset<soci::row>> prepare_query(soci::session &conn, const Token &tokn, Filter filter)
    {
        std::stringstream qry;
        qry << "SELECT * FROM " << this->table_name << " WHERE ";


        auto d = std::shared_ptr<soci::rowset<soci::row>>(nullptr);

        switch(filter)
        {
            case Filter::id:
            {
                qry << "TokenId = :token_id";
                soci::rowset<soci::row> some_rows = (conn.prepare << qry.str(), soci::use(tokn.token_id, "token_id"));
                d = std::make_shared<soci::rowset<soci::row>>(some_rows);
                break;
            }
            case Filter::access_token:
            {
                qry << "AccessToken = :access_token";
                soci::rowset<soci::row> some_rows = (conn.prepare << qry.str(), soci::use(tokn.access_token, "access_token"));
                d = std::make_shared<soci::rowset<soci::row>>(some_rows);
                break;
            }
            case Filter::user_id:
            {
                qry << "UserId = :user_id";
                soci::rowset<soci::row> some_rows = (conn.prepare << qry.str(), soci::use(tokn.user_id, "user_id"));
                d = std::make_shared<soci::rowset<soci::row>>(some_rows);
                break;
            }
            case Filter::token_and_user:
            {
                qry << "TokenId = :token_id AND UserId = :user_id";
                soci::rowset<soci::row> some_rows = (conn.prepare << qry.str(), soci::use(tokn.token_id, "token_id"),
                        soci::use(tokn.user_id, "user_id"));
                d = std::make_shared<soci::rowset<soci::row>>(some_rows);
                break;
            }
            default:
            {
                qry << "TokenId = :token_id";
                soci::rowset<soci::row> some_rows = (conn.prepare << qry.str(), soci::use(tokn.token_id, "token_id"));
                d = std::make_shared<soci::rowset<soci::row>>(some_rows);
                break;
            }
        }


        return d;
    }

    Token parse_record(const soci::row &row)
    {
        using std::tm;

        std::tm tm_default;
        std::string str_default("none");
        int int_default = 0;

        auto func1 = this->convert_tm_to_time_point<std::chrono::system_clock::time_point, std::tm>;

        Token tokn;
        tokn.token_id = row.template get<int>("TokenId");
        tokn.access_token = row.template get<std::string>("AccessToken", str_default);
        auto orig_issued = row.template get<tm>("Issued", tm_default);
        auto orig_expires = row.template get<tm>("Expires", tm_default);
        tokn.refresh_token = row.template get<bool>("RefreshToken");
        tokn.issued = func1(orig_issued);
        tokn.expires = func1(orig_expires);
        tokn.active = row.template get<bool>("Active");
        tokn.user_id = row.template get<int>("UserId", int_default);

        return tokn;
    }


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
