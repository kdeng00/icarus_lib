#ifndef ALBUMREPOSITORY_H_
#define ALBUMREPOSITORY_H_

#include <iostream>
#include <memory>
#include <utility>
#include <vector>
#include <string>
#include <sstream>
#include <cstring>

#include <mysql/mysql.h>

#include "icarus_lib/repositories/database/cloud/base_repository.h"
#include "icarus_lib/repositories/database/cloud/repository_utility.h"

namespace icarus_lib { namespace database {
template<class Album, typename Filter, class ConnStr>
class album_repository : public base_repository<ConnStr>
{
public:
    album_repository(const ConnStr &conn_str, const std::string table = "Album") : 
        base_repository<ConnStr>(conn_str, table)
    {
    }

    std::vector<Album> retrieveRecords()
    {
        auto conn = this->setup_connection();
        auto stmt = mysql_stmt_init(conn);

        const std::string query = "SELECT * FROM Album";
        mysql_stmt_prepare(stmt, query.c_str(), query.size());
        mysql_stmt_execute(stmt);

        auto albums = parseRecords(stmt);

        this->close_mysql_connection(conn, stmt);

        return albums;
    }

    std::pair<Album, int> retrieveRecordWithSongCount(Album &album, Filter filter)
    {
        std::cout << "retrieving album with song count\n";
        std::stringstream qry;
        auto conn = this->setup_connection();
        auto stmt = mysql_stmt_init(conn);

        auto params = this->mysql_bind_init(1);

        qry << "SELECT alb.*, COUNT(*) AS SongCount FROM Album alb LEFT JOIN ";
        qry << "Song sng ON alb.AlbumId=sng.AlbumId WHERE ";

        switch (filter)
        {
        case Filter::ID:
            qry << "sng.AlbumId = ?";

            repository_utility::construct_param_long(params, MYSQL_TYPE_LONG, album.id, 0);
            break;
        default:
            break;
        }
        qry << " GROUP BY alb.AlbumId LIMIT 1";

        const auto query = qry.str();

        auto status = mysql_stmt_prepare(stmt, query.c_str(), query.size());
        status = mysql_stmt_bind_param(stmt, params.get());
        status = mysql_stmt_execute(stmt);

        auto albWSC = parse_records_with_song_count(stmt);

        this->close_mysql_connection(conn, stmt);

        return albWSC.at(0);
    }

    Album retrieveRecord(Album &album, Filter filter)
    {
        std::cout << "retrieving album record\n";
        std::stringstream qry;
        auto conn = this->setup_connection();
        auto stmt = mysql_stmt_init(conn);

        auto params = this->mysql_bind_init(1);

        qry << "SELECT alb.* FROM Album alb WHERE ";

        auto titleLength = album.title.size();

        switch (filter)
        {
        case Filter::ID:
            qry << "alb.AlbumId = ?";

            repository_utility::construct_param_long(params, MYSQL_TYPE_LONG, album.id, 0);
            break;
        case Filter::TITLE:
            qry << "alb.Title = ?";

            repository_utility::construct_param_string(params, MYSQL_TYPE_STRING, album.title, 0, titleLength);
            break;
        default:
            break;
        }

        qry << " LIMIT 1";

        const auto query = qry.str();
        auto status = mysql_stmt_prepare(stmt, query.c_str(), query.size());
        status = mysql_stmt_bind_param(stmt, params.get());
        status = mysql_stmt_execute(stmt);

        const auto albums = parseRecords(stmt);

        this->close_mysql_connection(conn, stmt);

        std::cout << "done\n";

        return (albums.size() > 0) ? albums.at(0) : album;
    }

    bool doesAlbumExists(const Album &album, Filter filter)
    {
        auto conn = this->setup_connection();
        auto stmt = mysql_stmt_init(conn);

        MYSQL_BIND params[1];
        memset(params, 0, sizeof(params));

        std::stringstream qry;
        qry << "SELECT * FROM Album WHERE ";

        auto titleLength = album.title.size();
        switch (filter)
        {
        case Filter::id:
            qry << "AlbumId = ?";

            params[0].buffer_type = MYSQL_TYPE_LONG;
            params[0].buffer = (char*)&album.id;
            params[0].length = 0;
            params[0].is_null = 0;
            break;
        case Filter::title:
            qry << "Title = ?";

            params[0].buffer_type = MYSQL_TYPE_STRING;
            params[0].buffer = (char*)album.title.c_str();
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

    void saveAlbum(const Album &album)
    {
        std::cout << "beginning to insert album record\n";

        auto conn = this->setup_connection();
        MYSQL_STMT *stmt = mysql_stmt_init(conn);

        const std::string query = "INSERT INTO Album(Title, Artist, Year) VALUES(?, ?, ?)";

        auto status = mysql_stmt_prepare(stmt, query.c_str(), query.size());

        MYSQL_BIND params[3];
        memset(params, 0, sizeof(params));

        params[0].buffer_type = MYSQL_TYPE_STRING;
        params[0].buffer = (char*)album.title.c_str();
        auto titleLength = album.title.size();
        params[0].length= &titleLength;
        params[0].is_null = 0;

        params[1].buffer_type = MYSQL_TYPE_STRING;
        params[1].buffer = (char*)album.artist.c_str();
        auto artistLength = album.artist.size();
        params[1].length = &artistLength;
        params[1].is_null = 0;

        params[2].buffer_type = MYSQL_TYPE_LONG;
        params[2].buffer = (char*)&album.year;
        params[2].length = 0;
        params[2].is_null = 0;

        status = mysql_stmt_bind_param(stmt, params);
        status = mysql_stmt_execute(stmt);

        mysql_stmt_close(stmt);
        mysql_close(conn);

        std::cout << "done inserting album record\n";
    }
    void deleteAlbum(const Album &album, Filter filter)
    {
        std::cout << "deleting album record\n";

        std::stringstream qry;
        auto conn = this->setup_connection();
        auto stmt = mysql_stmt_init(conn);

        MYSQL_BIND params[1];
        std::memset(params, 0, sizeof(params));

        qry << "DELETE FROM Album WHERE ";

        switch (filter)
        {
        case Filter::id:
            qry << "AlbumId = ?";

            params[0].buffer_type = MYSQL_TYPE_LONG;
            params[0].buffer = (char*)&album.id;
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

        std::cout << "execute delete query\n";
    }
private:
    std::vector<Album> parseRecords(MYSQL_STMT *stmt)
    {
        std::cout << "parsing album record\n";
        mysql_stmt_store_result(stmt);

        std::vector<Album> albums;
        albums.reserve(mysql_stmt_num_rows(stmt));

        const auto valAmt = 3;
        unsigned long len[valAmt];
        char nullRes[valAmt];

        for (auto status = 0; status == 0; status = mysql_stmt_next_result(stmt))
        {
            if (mysql_stmt_field_count(stmt) > 0)
            {
                Album alb;
                auto metaBuff = metadataBuffer();
                auto bindedValues = valueBind(alb, metaBuff);
                status = mysql_stmt_bind_result(stmt, bindedValues.get());

                while (true)
                {
                    std::cout << "fetching statement result\n";
                    status = mysql_stmt_fetch(stmt);

                    if (status == 1 || status == MYSQL_NO_DATA) break;

                    alb.title = std::get<0>(metaBuff);
                    alb.artist = std::get<1>(metaBuff);
                    albums.push_back(std::move(alb));
                }
            }
            std::cout << "fetching next result\n";
        }

        return albums;
    }

    std::vector<std::pair<Album, int>> parse_records_with_song_count(MYSQL_STMT *stmt)
    {
        std::cout << "parsing album record with song count\n";
        mysql_stmt_store_result(stmt);

        std::vector<std::pair<Album, int>> albums;
        albums.reserve(mysql_stmt_num_rows(stmt));

        const auto valAmt = 3;
        unsigned long len[valAmt];
        char nullRes[valAmt];

        for (auto status = 0; status == 0; status = mysql_stmt_next_result(stmt))
        {
            if (mysql_stmt_field_count(stmt) > 0)
            {
                Album alb;
                auto metaBuff = metadataBuffer();
                auto song_count = 0;
                auto bindedValues = valueBindWithSongCount(alb, metaBuff, song_count);
                status = mysql_stmt_bind_result(stmt, bindedValues.get());

                while (true)
                {
                    std::cout << "fetching statement result\n";
                    status = mysql_stmt_fetch(stmt);

                    if (status == 1 || status == MYSQL_NO_DATA) break;

                    alb.title = std::get<0>(metaBuff);
                    alb.artist = std::get<1>(metaBuff);
                    albums.push_back(std::make_pair(std::move(alb), song_count));
                }
            }
            std::cout << "fetching next result\n";
        }

        return albums;
    }


    std::shared_ptr<MYSQL_BIND> valueBind(Album &album, 
            std::tuple<char*, char*> &metadata)
    {
        constexpr auto wordLen = 1024;
        constexpr auto valueCount = 4;
        std::shared_ptr<MYSQL_BIND> values((MYSQL_BIND*) std::calloc(valueCount, sizeof(MYSQL_BIND)));

        values.get()[0].buffer_type = MYSQL_TYPE_LONG;
        values.get()[0].buffer = (char*)&album.id;

        values.get()[1].buffer_type = MYSQL_TYPE_STRING;
        values.get()[1].buffer = (char*)std::get<0>(metadata);
        values.get()[1].buffer_length = wordLen;

        values.get()[2].buffer_type = MYSQL_TYPE_STRING;
        values.get()[2].buffer = (char*)std::get<1>(metadata);
        values.get()[2].buffer_length = wordLen;

        values.get()[3].buffer_type = MYSQL_TYPE_LONG;
        values.get()[3].buffer = (char*)&album.year;

        return values;
    }

    std::shared_ptr<MYSQL_BIND> valueBindWithSongCount(Album &album, 
            std::tuple<char*, char*> &metadata, int &songCount)
    {
        constexpr auto wordLen = 1024;
        constexpr auto valueCount = 5;
        std::shared_ptr<MYSQL_BIND> values(
            (MYSQL_BIND*) std::calloc(valueCount, sizeof(MYSQL_BIND)));

        values.get()[0].buffer_type = MYSQL_TYPE_LONG;
        values.get()[0].buffer = (char*)&album.id;

        values.get()[1].buffer_type = MYSQL_TYPE_STRING;
        values.get()[1].buffer = (char*)std::get<0>(metadata);
        values.get()[1].buffer_length = wordLen;

        values.get()[2].buffer_type = MYSQL_TYPE_STRING;
        values.get()[2].buffer = (char*)std::get<1>(metadata);
        values.get()[2].buffer_length = wordLen;

        values.get()[3].buffer_type = MYSQL_TYPE_LONG;
        values.get()[3].buffer = (char*)&album.year;

        values.get()[4].buffer_type = MYSQL_TYPE_LONG;
        values.get()[4].buffer = (char*)&songCount;

        return values;
    }

    std::tuple<char*, char*> metadataBuffer()
    {
        constexpr auto wordLen = 1024;
        char title[wordLen];
        char artist[wordLen];

        return std::make_tuple(title, artist);
    }
};
}}

#endif
