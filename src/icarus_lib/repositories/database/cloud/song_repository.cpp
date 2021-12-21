#include "icarus_lib/repositories/database/cloud/song_repository.hpp"



namespace icarus_lib::database
{

    template<class Song, class Filter>
    void song_repository<Song, Filter>::saveRecord(const Song &song)
    {
        std::cout << "beginning to insert song record\n";
        auto conn = this->setup_connection();
        auto status = 0;

        MYSQL_STMT *stmt = mysql_stmt_init(conn);

        std::string query = "INSERT INTO Song(Title, Artist, Album, Genre, ";
        query.append("Year, Duration, Track, Disc, SongPath, CoverArtId, ArtistId, ");
        query.append("AlbumId, GenreId, YearId) VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");

        status = mysql_stmt_prepare(stmt, query.c_str(), query.size());

        MYSQL_BIND params[14];
        memset(params, 0, sizeof(params));

        params[0].buffer_type = MYSQL_TYPE_STRING;
        params[0].buffer = (char*)song.title.c_str();
        auto titleLength = song.title.size();
        params[0].length = &titleLength;
        params[0].is_null = 0;

        params[1].buffer_type = MYSQL_TYPE_STRING;
        params[1].buffer = (char*)song.artist.c_str();
        auto artistLength = song.artist.size();
        params[1].length = &artistLength;
        params[1].is_null = 0;

        params[2].buffer_type = MYSQL_TYPE_STRING;
        params[2].buffer = (char*)song.album.c_str();
        auto albumLength = song.album.size();
        params[2].length = &albumLength;
        params[2].is_null = 0;

        params[3].buffer_type = MYSQL_TYPE_STRING;
        params[3].buffer = (char*)song.genre.c_str();
        auto genreLength = song.genre.size();
        params[3].length = &genreLength;
        params[3].is_null = 0;

        params[4].buffer_type = MYSQL_TYPE_LONG;
        params[4].buffer = (char*)&song.year;
        params[4].length = 0;
        params[4].is_null = 0;

        params[5].buffer_type = MYSQL_TYPE_LONG;
        params[5].buffer = (char*)&song.duration;
        params[5].length = 0;
        params[5].is_null = 0;

        params[6].buffer_type = MYSQL_TYPE_LONG;
        params[6].buffer = (char*)&song.track;
        params[6].length = 0;
        params[6].is_null = 0;

        params[7].buffer_type = MYSQL_TYPE_LONG;
        params[7].buffer = (char*)&song.disc;
        params[7].length = 0;
        params[7].is_null = 0;

        params[8].buffer_type = MYSQL_TYPE_STRING;
        params[8].buffer = (char*)song.song_path.c_str();
        auto pathLength = song.song_path.size();
        params[8].length = &pathLength;
        params[8].is_null = 0;

        params[9].buffer_type = MYSQL_TYPE_LONG;
        params[9].buffer = (char*)&song.cover_art_id;
        params[9].length = 0;
        params[9].is_null = 0;

        params[10].buffer_type = MYSQL_TYPE_LONG;
        params[10].buffer = (char*)&song.artist_id;
        params[10].length = 0;
        params[10].is_null = 0;

        params[11].buffer_type = MYSQL_TYPE_LONG;
        params[11].buffer = (char*)&song.album_id;
        params[11].length = 0;
        params[11].is_null = 0;
        
        params[12].buffer_type = MYSQL_TYPE_LONG;
        params[12].buffer = (char*)&song.genre_id;
        params[12].length = 0;
        params[12].is_null = 0;

//        params[13].buffer_type = MYSQL_TYPE_LONG;
//        params[13].buffer = (char*)&song.year_id;
//        params[13].length = 0;
//        params[13].is_null = 0;

        status = mysql_stmt_bind_param(stmt, params);
        status = mysql_stmt_execute(stmt);

        mysql_stmt_close(stmt);
        mysql_close(conn);

        std::cout << "done inserting song record\n";
    }


    template<class Song, class Filter>
    Song song_repository<Song, Filter>::parseRecord(MYSQL_STMT *stmt)
    {
        mysql_stmt_store_result(stmt);

        std::cout << "amount of rows: " << mysql_stmt_num_rows(stmt) << "\n";

        Song song;

        auto metaBuff = metadataBuffer();
        auto bindedValues = valueBind(song, metaBuff);
        auto status = mysql_stmt_bind_result(stmt, bindedValues.get());
        status = mysql_stmt_fetch(stmt);

        parse_record_string_field(song, metaBuff);
        
        std::cout << "done parsing record\n";

        return song;
    }

    template<class Song, class Filter>
    std::vector<Song> song_repository<Song, Filter>::parseRecords(MYSQL_STMT *stmt)
    {
        ::mysql_stmt_store_result(stmt);
        auto c = ::mysql_stmt_num_rows(stmt);
        std::cout << "number of results " << c << "\n";
        std::vector<Song> songs;
        songs.reserve(c);

        auto status = 0;

        while (status == 0) {
            if (::mysql_stmt_field_count(stmt) > 0) {
                Song song;
                auto metaBuff = metadataBuffer();
                auto val = valueBind(song, metaBuff);

                status = ::mysql_stmt_bind_result(stmt, val.get());

                while (1) {
                    std::cout << "fetching statement result\n";
                    status = ::mysql_stmt_fetch(stmt);

                    if (status == 1 || status == MYSQL_NO_DATA) {
                        break;
                    }
                
                    parse_record_string_field(song, metaBuff);

                    songs.push_back(song);
                }
            }
            std::cout << "fetching next result\n";
            status = ::mysql_stmt_next_result(stmt);
        }

        return songs;
    }


}