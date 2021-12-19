#ifndef COVERARTREPOSITORY_H_
#define COVERARTREPOSITORY_H_

#include <memory>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>

#include <mysql/mysql.h>

#include "icarus_lib/models/models.hpp"
#include "icarus_lib/repositories/database/cloud/base_repository.hpp"
#include "icarus_lib/repositories/database/cloud/repository_utility.hpp"

namespace icarus_lib::repositories::database::cloud {

template<class Cover, typename Filter, class ConnStr>
class cover_art_repository : public base_repository<ConnStr>
{
public:
    cover_art_repository(const ConnStr &conn_str) : base_repository<ConnStr>(conn_str, "CoverArt")
    {
    }
    cover_art_repository(const models::binary_path &conn_str) : base_repository<ConnStr>(conn_str, "CoverArt")
    {
    }

    std::vector<Cover> retrieveRecords()
    {
        auto conn = this->setup_connection();
        auto stmt = mysql_stmt_init(conn);
        const std::string query = "SELECT * FROM CoverArt";

        mysql_stmt_prepare(stmt, query.c_str(), query.size());
        mysql_stmt_execute(stmt);

        auto cover_arts = parse_records(stmt);

        mysql_stmt_close(stmt);
        mysql_close(conn);

        return cover_arts;
    }

    Cover retrieve_record(Cover &cover, Filter filter = Filter::id)
    {
        std::stringstream qry;
        auto conn = this->setup_connection();
        auto stmt = mysql_stmt_init(conn);

        qry << "SELECT * FROM CoverArt WHERE ";

        MYSQL_BIND params[1];
        memset(params, 0, sizeof(params));

        auto songTitleLength = cover.song_title.size();
        switch (filter) {
            case Filter::id:
                qry << "CoverArtId = ?";

                params[0].buffer_type = MYSQL_TYPE_LONG;
                params[0].buffer = (char*)&cover.id;
                params[0].length = 0;
                params[0].is_null = 0;
                break;
            case Filter::songTitle:
                qry << "SongTitle = ?";

                params[0].buffer_type = MYSQL_TYPE_STRING;
                params[0].buffer = (char*)cover.song_title.c_str();
                params[0].length = &songTitleLength;
                params[0].is_null = 0;
                break;
            default:
                break;
        }

        qry << " LIMIT 1";

        const std::string query = qry.str();
        auto status = mysql_stmt_prepare(stmt, query.c_str(), query.size());
        status = mysql_stmt_bind_param(stmt, params);
        status = mysql_stmt_execute(stmt);

        auto covDb = parseRecord(stmt);

        mysql_stmt_close(stmt);
        mysql_close(conn);
        std::cout << "retrieved cover art record\n";

        return covDb;
    }
    
    bool doesCoverArtExist(const Cover &cover, Filter filter)
    {
        auto conn = this->setup_connection();
        auto stmt = mysql_stmt_init(conn);

        MYSQL_BIND params[1];
        memset(params, 0, sizeof(params));

        std::stringstream qry;
        qry << "SELECT * FROM CoverArt WHERE ";

        auto titleLength = cover.song_title.size();
        switch (filter) {
            case Filter::id:
                qry << "CoverArtId = ?";

                params[0].buffer_type = MYSQL_TYPE_LONG;
                params[0].buffer = (char*)&cover.id;
                params[0].length = 0;
                params[0].is_null = 0;
                break;
            case Filter::songTitle:
                qry << "SongTitle = ?";

                params[0].buffer_type = MYSQL_TYPE_STRING;
                params[0].buffer = (char*)cover.song_title.c_str();
                params[0].length = &titleLength;
                params[0].is_null = 0;
                break;
            default:
                break;
        }

        qry << " LIMIT 1";

        const auto query = qry.str();
        auto status = mysql_stmt_prepare(stmt, query.c_str(), query.size());
        status = mysql_stmt_bind_param(stmt, params);
        status = mysql_stmt_execute(stmt);

        std::cout << "the query has been performed\n";

        mysql_stmt_store_result(stmt);
        auto rowCount = mysql_stmt_num_rows(stmt);

        mysql_stmt_close(stmt);
        mysql_close(conn);

        return (rowCount > 0) ? true : false;
    }

    void deleteRecord(const Cover &cov, Filter filter = Filter::id)
    {
        auto conn = this->setup_connection();
        auto stmt = mysql_stmt_init(conn);
        std::cout << "delete CoverArt record\n";
        std::stringstream qry;
        qry << "DELETE FROM CoverArt WHERE ";

        MYSQL_BIND params[1];
        std::memset(params, 0, sizeof(params));

        switch (filter) {
            case Filter::id:
                qry << "CoverArtId = ?";

                params[0].buffer_type = MYSQL_TYPE_LONG;
                params[0].buffer = (char*)&cov.id;
                params[0].length = 0;
                params[0].is_null = 0;
                break;
            default:
                break;
        }

        const auto query = qry.str();

        auto status = mysql_stmt_prepare(stmt, query.c_str(), query.size());
        status = mysql_stmt_bind_param(stmt, params);
        status = mysql_stmt_execute(stmt);

        mysql_stmt_close(stmt);
        mysql_close(conn);

        std::cout << "deleted CoverArt record\n";
    }
    void saveRecord(const Cover &cov)
    {
        std::cout << "saving cover art record";
        auto conn = this->setup_connection();
        auto stmt = mysql_stmt_init(conn);

        MYSQL_BIND params[2];
        memset(params, 0, sizeof(params));
        // my_bool isNull;
        char isNull;

        const std::string query = "INSERT INTO CoverArt(SongTitle, ImagePath) VALUES(?, ?)";

        auto status = mysql_stmt_prepare(stmt, query.c_str(), query.size());

        params[0].buffer_type = MYSQL_TYPE_STRING;
        params[0].buffer = (char*)cov.song_title.c_str();
        auto songTitleLength = cov.song_title.size();
        params[0].length = &songTitleLength;
        params[0].is_null = 0;

        params[1].buffer_type = MYSQL_TYPE_STRING;
        params[1].buffer = (char*)cov.image_path.c_str();
        auto imagePathLength = cov.image_path.size();
        params[1].length = &imagePathLength;
        params[1].is_null = 0;

        status = mysql_stmt_bind_param(stmt, params);
        status = mysql_stmt_execute(stmt);

        mysql_stmt_close(stmt);
        mysql_close(conn);

        std::cout << "saved cover art record\n";
    }
    void updateRecord(const Cover &cover)
    {
        std::stringstream qry;
        auto conn = this->setup_connection();
        auto stmt = mysql_stmt_init(conn);

        qry << "UPDATE CoverArt SET ";
        qry << "SongTitle = ?, ";
        qry << "ImagePath = ? ";
        qry << "WHERE CoverArtId = ?";

        MYSQL_BIND params[3];
        memset(params, 0, sizeof(params));

        auto songTitleLength = cover.song_title.size();
        auto imagePathLength = cover.image_path.size();

        params[0].buffer_type = MYSQL_TYPE_STRING;
        params[0].buffer = (char*)cover.song_title.c_str();
        params[0].length = &songTitleLength;
        params[0].is_null = 0;

        params[1].buffer_type = MYSQL_TYPE_STRING;
        params[1].buffer = (char*)cover.image_path.c_str();
        params[1].length = &imagePathLength;
        params[1].is_null = 0;

        params[2].buffer_type = MYSQL_TYPE_LONG;
        params[2].buffer = (char*)&cover.id;
        params[2].length = 0;
        params[2].is_null = 0;

        const std::string query = qry.str();
        auto status = mysql_stmt_prepare(stmt, query.c_str(), query.size());
        status = mysql_stmt_bind_param(stmt, params);
        status = mysql_stmt_execute(stmt);

        mysql_stmt_close(stmt);
        mysql_close(conn);

        std::cout << "updated cover art record\n";
    }
private:
    std::vector<Cover> parseRecords(MYSQL_STMT *stmt)
    {
        mysql_stmt_store_result(stmt);
        const auto rowCount = mysql_stmt_num_rows(stmt);
        std::cout << "number of results " << rowCount << "\n";

        std::vector<Cover> coverArts;
        coverArts.reserve(rowCount);

        constexpr auto valAmt = 3;

        for(auto status = 0; status == 0; status = mysql_stmt_next_result(stmt))
        {
            if(mysql_stmt_field_count(stmt) > 0)
            {
                Cover cover;
                auto metaBuff = metadataBuffer();
                auto bindedValues = valueBind(cover, metaBuff);
                status = mysql_stmt_bind_result(stmt, bindedValues.get());

                std::cout << "fetching statement result\n";
                status = mysql_stmt_fetch(stmt);

                if(status == 1 || status == MYSQL_NO_DATA) break;

                cover.song_title = std::get<0>(metaBuff);
                cover.image_path = std::get<1>(metaBuff);
                coverArts.push_back(cover);
            }
            std::cout << "fetching next result\n";
        }

        return coverArts;
    }

    std::shared_ptr<MYSQL_BIND> valueBind(Cover &cover,
            std::tuple<char*, char*> &metadata)
    {
        constexpr auto valueCount = 3;
        constexpr auto wordLen = 1024;
        std::shared_ptr<MYSQL_BIND> values((MYSQL_BIND*) 
                std::calloc(valueCount, sizeof(MYSQL_BIND)));

        values.get()[0].buffer_type = MYSQL_TYPE_LONG;
        values.get()[0].buffer = reinterpret_cast<char*>(&cover.id);

        values.get()[1].buffer_type = MYSQL_TYPE_STRING;
        values.get()[1].buffer = reinterpret_cast<char*>(std::get<0>(metadata));
        values.get()[1].buffer_length = wordLen;

        values.get()[2].buffer_type = MYSQL_TYPE_STRING;
        values.get()[2].buffer = reinterpret_cast<char*>(std::get<1>(metadata));
        values.get()[2].buffer_length = wordLen;

        return values;
    }

    std::tuple<char*, char*> metadataBuffer()
    {
        constexpr auto wordLen = 1024;
        char songTitle[wordLen];
        char imagePath[wordLen];

        return std::make_pair(songTitle, imagePath);
    }

    Cover parseRecord(MYSQL_STMT *stmt)
    {
        std::cout << "parsing cover art record\n";
        mysql_stmt_store_result(stmt);
        Cover cover;

        auto metaBuff = metadataBuffer();
        auto bindedValues = valueBind(cover, metaBuff);
        auto status = mysql_stmt_bind_result(stmt, bindedValues.get());
        status = mysql_stmt_fetch(stmt);

        cover.song_title = std::get<0>(metaBuff);
        cover.image_path = std::get<1>(metaBuff);

        std::cout << "done parsing cover art record\n";

        return cover;
    }
};

}

#endif
