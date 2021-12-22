#include "icarus_lib/repositories/database/cloud/song_repository.hpp"



namespace icarus_lib::database
{

song_repository::song_repository(const models::conn_str &conn_str, const std::string table) : 
    base_repository(conn_str, table)
{
}
song_repository::song_repository(const models::binary_path &conn_str, const std::string table) : 
    base_repository(conn_str, table)
{
}

std::vector<models::song> song_repository::retrieveRecords() {
    auto conn = this->setup_connection();
    auto stmt = mysql_stmt_init(conn);
    std::stringstream qry;
    qry << "SELECT sng.*, alb.Artist AS AlbumArtist FROM Song sng ";
    qry << "LEFT JOIN Album alb ON sng.AlbumId=alb.AlbumId";
    const auto query = qry.str();

    ::mysql_stmt_prepare(stmt, query.c_str(), query.size());
    ::mysql_stmt_execute(stmt);

    auto songs = parseRecords(stmt);

    mysql_stmt_close(stmt);
    mysql_close(conn);

    return songs;
}


models::song song_repository::retrieveRecord(const models::song& song, types::song_filter filter) {
    std::stringstream qry;
    auto conn = this->setup_connection();
    auto stmt = mysql_stmt_init(conn);

    auto valueFilterCount = 1;
    switch (filter) {
        case types::song_filter::TITLE_AND_ARTIST:
            valueFilterCount = 2;
            break;
        case types::song_filter::TITLE_ALBUM_ARTIST_ALBUM:
            valueFilterCount = 3;
            break;
        case types::song_filter::TITLE_ALBUM_ARTIST_ALBUM_TRACK:
            valueFilterCount = 4;
            break;
        default:
            break;
    }

    std::shared_ptr<MYSQL_BIND> params((MYSQL_BIND*) std::calloc(valueFilterCount, sizeof(MYSQL_BIND)));

    qry << "SELECT sng.*, alb.Artist AS AlbumArtist FROM Song sng ";
    qry << "LEFT JOIN Album alb ON sng.AlbumId=alb.AlbumId WHERE ";

    auto titleLength = song.title.size();
    auto artistLength = song.artist.size();
    auto albumArtistLength = song.album_artist.size();
    auto albumLength = song.album.size();

    switch (filter) {
        case types::song_filter::ID:
            qry << "sng.SongId = ?";

            repository_utility::construct_param_long<std::shared_ptr<MYSQL_BIND>, 
                enum_field_types, int>(params, MYSQL_TYPE_LONG, song.id, 0);
            break;
        case types::song_filter::TITLE:
            qry << "sng.Title = ?";

            repository_utility::construct_param_string<std::shared_ptr<MYSQL_BIND>, 
                enum_field_types, std::string>(params, MYSQL_TYPE_STRING, song.title, 0, titleLength);
            break;
        case types::song_filter::TITLE_AND_ARTIST:
            qry << "sng.Title = ? AND sng.Artist = ?";

            repository_utility::construct_param_string<std::shared_ptr<MYSQL_BIND>, 
                enum_field_types, std::string>(params, MYSQL_TYPE_STRING, song.title, 0, titleLength);
            repository_utility::construct_param_string<std::shared_ptr<MYSQL_BIND>, 
                enum_field_types, std::string>(params, MYSQL_TYPE_STRING, song.artist, 1, artistLength);

            std::cout << "title: " << song.title.c_str() << " artist: " << 
                song.artist.c_str() << "\n";
            break;
        case types::song_filter::TITLE_ALBUM_ARTIST_ALBUM:
            qry << "sng.Title = ? AND sng.Album = ? AND alb.Artist = ?";

            repository_utility::construct_param_string<std::shared_ptr<MYSQL_BIND>, 
                enum_field_types, std::string>(params, MYSQL_TYPE_STRING, song.title, 0, titleLength);
            repository_utility::construct_param_string<std::shared_ptr<MYSQL_BIND>, 
                enum_field_types, std::string>(params, MYSQL_TYPE_STRING, song.album, 1, artistLength);
            repository_utility::construct_param_string<std::shared_ptr<MYSQL_BIND>, 
                enum_field_types, std::string>(params, MYSQL_TYPE_STRING, song.album_artist, 2, artistLength);

            break;
        case types::song_filter::TITLE_ALBUM_ARTIST_ALBUM_TRACK:
            qry << "sng.Title = ? AND sng.Album = ? AND alb.Artist = ? AND sng.Track = ?";

            repository_utility::construct_param_string<std::shared_ptr<MYSQL_BIND>, 
                enum_field_types, std::string>(params, MYSQL_TYPE_STRING, song.title, 0, titleLength);
            repository_utility::construct_param_string<std::shared_ptr<MYSQL_BIND>, 
                enum_field_types, std::string>(params, MYSQL_TYPE_STRING, song.album, 1, albumLength);
            repository_utility::construct_param_string<std::shared_ptr<MYSQL_BIND>, 
                enum_field_types, std::string>(params, MYSQL_TYPE_STRING, song.album_artist, 2, albumArtistLength);
            repository_utility::construct_param_long<std::shared_ptr<MYSQL_BIND>, 
                enum_field_types, int>(params, MYSQL_TYPE_STRING, song.track, 3);

            break;
        default:
            break;
    }

    qry << " LIMIT 1";

    const auto query = qry.str();
    auto status = mysql_stmt_prepare(stmt, query.c_str(), query.size());
    status = mysql_stmt_bind_param(stmt, params.get());
    status = mysql_stmt_execute(stmt);

    std::cout << "the query has been performed\n";

    auto retrievedSong = parseRecord(stmt);

    mysql_stmt_close(stmt);
    mysql_close(conn);

    return retrievedSong;
}


bool song_repository::doesSongExist(const models::song& song, types::song_filter filter) {
    std::cout << "checking to see if song exists\n";
    std::stringstream qry;
    auto conn = this->setup_connection();
    auto stmt = mysql_stmt_init(conn);
    auto valueFilterCount = 1;
    switch (filter) {
        case types::song_filter::TITLE_AND_ARTIST:
            valueFilterCount = 2;
            break;
        case types::song_filter::TITLE_ALBUM_ARTIST_ALBUM:
            valueFilterCount = 3;
            break;
        case types::song_filter::TITLE_ALBUM_ARTIST_ALBUM_TRACK:
            valueFilterCount = 4;
            break;
        default:
            break;
    }


    MYSQL_BIND params[valueFilterCount];
    memset(params, 0, sizeof(params));

    qry << "SELECT sng.* FROM Song sng ";
    qry << "LEFT JOIN Album alb ON sng.AlbumId = alb.AlbumId WHERE ";

    auto titleLength = song.title.size();
    auto artistLength = song.artist.size();
    auto albumArtistLength = song.album_artist.size();
    auto albumLength = song.album.size();
    switch (filter) {
        case types::song_filter::ID:
            qry << "sng.SongId = ?";

            params[0].buffer_type = MYSQL_TYPE_LONG;
            params[0].buffer = (char*)&song.id;
            params[0].length = 0;
            params[0].is_null = 0;
            break;
        case types::song_filter::TITLE:
            qry << "sng.Title = ?";

            params[0].buffer_type = MYSQL_TYPE_STRING;
            params[0].buffer = (char*)song.title.c_str();
            params[0].length = &titleLength;
            params[0].is_null = 0;
            break;
        case types::song_filter::TITLE_AND_ARTIST:
            qry << "sng.Title = ? AND sng.Artist = ?";

            params[0].buffer_type = MYSQL_TYPE_STRING;
            params[0].buffer = (char*)song.title.c_str();
            params[0].length = &titleLength;
            params[0].is_null = 0;

            params[1].buffer_type = MYSQL_TYPE_STRING;
            params[1].buffer = (char*)song.artist.c_str();
            params[1].length = &artistLength;
            params[1].is_null = 0;
            break;
        case types::song_filter::TITLE_ALBUM_ARTIST_ALBUM:
            qry << "sng.Title = ? AND sng.Album = ? AND alb.Artist = ?";

            params[0].buffer_type = MYSQL_TYPE_STRING;
            params[0].buffer = (char*)song.title.c_str();
            params[0].length = &titleLength;
            params[0].is_null = 0;

            params[1].buffer_type = MYSQL_TYPE_STRING;
            params[1].buffer = (char*)song.album.c_str();
            params[1].length = &albumLength;
            params[1].is_null = 0;

            params[1].buffer_type = MYSQL_TYPE_STRING;
            params[1].buffer = (char*)song.album_artist.c_str();
            params[1].length = &albumArtistLength;
            params[1].is_null = 0;
            break;
        case types::song_filter::TITLE_ALBUM_ARTIST_ALBUM_TRACK:
            qry << "sng.Title = ? AND sng.Album = ? AND alb.Artist = ? AND sng.Track = ?";

            params[0].buffer_type = MYSQL_TYPE_STRING;
            params[0].buffer = (char*)song.title.c_str();
            params[0].length = &titleLength;
            params[0].is_null = 0;

            params[1].buffer_type = MYSQL_TYPE_STRING;
            params[1].buffer = (char*)song.album.c_str();
            params[1].length = &titleLength;
            params[1].is_null = 0;

            params[2].buffer_type = MYSQL_TYPE_STRING;
            params[2].buffer = (char*)song.album_artist.c_str();
            params[2].length = &titleLength;
            params[2].is_null = 0;

            params[3].buffer_type = MYSQL_TYPE_LONG;
            params[3].buffer = (char*)&song.track;
            params[3].length = 0;
            params[3].is_null = 0;;
            break;
        default:
            break;
    }

    qry << " LIMIT 1";

    const std::string query = qry.str();
    auto status = mysql_stmt_prepare(stmt, query.c_str(), query.size());
    status = mysql_stmt_bind_param(stmt, params);
    status = mysql_stmt_execute(stmt);

    std::cout << "the query has been performed\n";

    ::mysql_stmt_store_result(stmt);
    auto rowCount = ::mysql_stmt_num_rows(stmt);

    mysql_stmt_close(stmt);
    mysql_close(conn);

    return (rowCount > 0) ? true : false;
}

bool song_repository::deleteRecord(const models::song& song) {
    auto conn = this->setup_connection();
    auto status = 0;

    const std::string query("DELETE FROM Song WHERE SongId = " + std::to_string(song.id));

    auto result = this->perform_query(conn, query);

    mysql_close(conn);

    return (result == 0) ? true : false;
}

void song_repository::updateRecord(const models::song &song)
{
    std::cout << "executing query to update record\n";
    auto conn = this->setup_connection();
    auto stmt = mysql_stmt_init(conn);

    std::stringstream qry;
    qry << "UPDATE Song SET Title = ?, Artist = ?, Album = ?, Genre = ?, ";
    qry << "Year = ?, SongPath = ?, CoverArtId = ?, ArtistId = ?, ";
    qry << "AlbumId = ?, GenreId = ?, YearId = ? WHERE SongId = ?";
    const auto query = qry.str();

    auto status = mysql_stmt_prepare(stmt, query.c_str(), query.size());

    MYSQL_BIND params[12];
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

    params[5].buffer_type = MYSQL_TYPE_STRING;
    params[5].buffer = (char*)song.song_path.c_str();
    auto pathLength = song.song_path.size();
    params[5].length = &pathLength;
    params[5].is_null = 0;

    params[6].buffer_type = MYSQL_TYPE_LONG;
    params[6].buffer = (char*)&song.cover_art_id;
    params[6].length = 0;
    params[6].is_null = 0;

    params[7].buffer_type = MYSQL_TYPE_LONG;
    params[7].buffer = (char*)&song.artist_id;
    params[7].length = 0;
    params[7].is_null = 0;

    params[8].buffer_type = MYSQL_TYPE_LONG;
    params[8].buffer = (char*)&song.album_id;
    params[8].length = 0;
    params[8].is_null = 0;
    
    params[9].buffer_type = MYSQL_TYPE_LONG;
    params[9].buffer = (char*)&song.genre_id;
    params[9].length = 0;
    params[9].is_null = 0;

//        params[10].buffer_type = MYSQL_TYPE_LONG;
//        params[10].buffer = (char*)&song.year_id;
//        params[10].length = 0;
//        params[10].is_null = 0;

    params[11].buffer_type = MYSQL_TYPE_LONG;
    params[11].buffer = (char*)&song.id;
    params[11].length = 0;
    params[11].is_null = 0;

    status = ::mysql_stmt_bind_param(stmt, params);
    status = ::mysql_stmt_execute(stmt);

    ::mysql_stmt_close(stmt);
    ::mysql_close(conn);

    std::cout << "updated song record\n";
}

void song_repository::song_repository::saveRecord(const models::song &song)
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


std::shared_ptr<MYSQL_BIND> song_repository::valueBind(models::song& song,
        std::tuple<char*, char*, char*, char*, char*, char*>& metadata)
{
    constexpr auto strLen = 1024;
    constexpr auto valueCount = 16;
    std::shared_ptr<MYSQL_BIND> values((MYSQL_BIND*) std::calloc(valueCount, sizeof(MYSQL_BIND)));
    unsigned long len[valueCount];
    char nullRes[valueCount];


    values.get()[0].buffer_type = MYSQL_TYPE_LONG;
    values.get()[0].buffer = (char*)&song.id;

    values.get()[1].buffer_type = MYSQL_TYPE_STRING;
    values.get()[1].buffer = (char*)std::get<0>(metadata);
    values.get()[1].buffer_length = strLen;

    values.get()[2].buffer_type = MYSQL_TYPE_STRING;
    values.get()[2].buffer = (char*)std::get<1>(metadata);
    values.get()[2].buffer_length = strLen;

    values.get()[3].buffer_type = MYSQL_TYPE_STRING;
    values.get()[3].buffer = (char*)std::get<2>(metadata);
    values.get()[3].buffer_length = strLen;

    values.get()[4].buffer_type = MYSQL_TYPE_STRING;
    values.get()[4].buffer = (char*)std::get<3>(metadata);
    values.get()[4].buffer_length = strLen;

    values.get()[5].buffer_type = MYSQL_TYPE_LONG;
    values.get()[5].buffer = (char*)&song.year;

    values.get()[6].buffer_type = MYSQL_TYPE_LONG;
    values.get()[6].buffer = (char*)&song.duration;

    values.get()[7].buffer_type = MYSQL_TYPE_LONG;
    values.get()[7].buffer = (char*)&song.track;

    values.get()[8].buffer_type = MYSQL_TYPE_LONG;
    values.get()[8].buffer = (char*)&song.disc;

    values.get()[9].buffer_type = MYSQL_TYPE_STRING;
    values.get()[9].buffer = (char*)std::get<4>(metadata);
    values.get()[9].buffer_length = strLen;

    values.get()[10].buffer_type = MYSQL_TYPE_LONG;
    values.get()[10].buffer = (char*)&song.cover_art_id;

    values.get()[11].buffer_type = MYSQL_TYPE_LONG;
    values.get()[11].buffer = (char*)&song.artist_id;

    values.get()[12].buffer_type = MYSQL_TYPE_LONG;
    values.get()[12].buffer = (char*)&song.album_id;;

    values.get()[13].buffer_type = MYSQL_TYPE_LONG;
    values.get()[13].buffer = (char*)&song.genre_id;

    // values.get()[14].buffer_type = MYSQL_TYPE_LONG;
    // values.get()[14].buffer = (char*)&song.year_id;

    values.get()[15].buffer_type = MYSQL_TYPE_STRING;
    values.get()[15].buffer = (char*)std::get<5>(metadata);
    values.get()[15].buffer_length = strLen;

    return values;
}



models::song song_repository::song_repository::parseRecord(MYSQL_STMT *stmt)
{
    mysql_stmt_store_result(stmt);

    std::cout << "amount of rows: " << mysql_stmt_num_rows(stmt) << "\n";

    models::song song;

    auto metaBuff = metadataBuffer();
    auto bindedValues = valueBind(song, metaBuff);
    auto status = mysql_stmt_bind_result(stmt, bindedValues.get());
    status = mysql_stmt_fetch(stmt);

    parse_record_string_field(song, metaBuff);
    
    std::cout << "done parsing record\n";

    return song;
}


std::vector<models::song> song_repository::song_repository::parseRecords(MYSQL_STMT *stmt)
{
    ::mysql_stmt_store_result(stmt);
    auto c = ::mysql_stmt_num_rows(stmt);
    std::cout << "number of results " << c << "\n";
    std::vector<models::song> songs;
    songs.reserve(c);

    auto status = 0;

    while (status == 0) {
        if (::mysql_stmt_field_count(stmt) > 0) {
            models::song song;
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

std::tuple<char*, char*, char*, char*, char*, char*> song_repository::metadataBuffer()
{
    constexpr auto length = 1024;
    char title[length];
    char artist[length];
    char album[length];
    char genre[length];
    char path[length];
    char albumArtist[length];

    return std::make_tuple(title, artist, album, genre, path, albumArtist);
}
void song_repository::parse_record_string_field(models::song &song, std::tuple<char *, char *, char *, char *, char *, char *> metaBuff)
{
        song.title = std::get<0>(metaBuff);
        song.artist = std::get<1>(metaBuff);
        song.album = std::get<2>(metaBuff);
        song.genre = std::get<3>(metaBuff);
        song.song_path = std::get<4>(metaBuff);
        song.album_artist = std::get<5>(metaBuff);
    }

}