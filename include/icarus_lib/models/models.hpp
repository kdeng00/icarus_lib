#ifndef MODELS_H_
#define MODELS_H_

#include <string>
#include <vector>
#include <chrono>
#include <utility>

#include "auth_models.hpp"
#include "config_models.hpp"
#include "music_models.hpp"
#include "icarus_lib/types/music.hpp"
#include "icarus_lib/types/auth.hpp"

namespace icarus_lib::models
{

using raw_data = unsigned char;

using song = song_type<std::string, int, long, raw_data>;
using artist = artist_type<song, std::string, int>;
using album = album_type<song, std::string, int>;
using genre = genre_type<song, std::string, int>;
// TODO: Will not be used
// using year = year_type<int>;
using cover = cover_type<song, std::string, int, raw_data>;

using token = token_type<std::string, int, std::chrono::system_clock::time_point>;
using login_result = login_result_type<std::string, int>;
using register_result = register_result_type<std::string>;
using user = user_type<std::string, int>;
using pass_sec = pass_sec_type<std::string, int>;
using auth_credentials = auth_credentials_type<std::string>;

using database_connection = database_connection_type<std::string>;
using conn_string = database_connection_type<std::string>;
using binary_path = binary_path_type<std::string>;



using song_filter = types::song_filter;
using artist_filter = types::artist_filter;
using album_filter = types::album_filter;
using genre_filter = types::genre_filter;
using cover_art_filter = types::cover_filter;
using token_filter = types::token_filter;
using user_filter = types::user_filter;
using salt_filter = types::salt_filter;

// TODO: Repository classes
/**
using album_repository = database::album_repository<album, types::album_filter, conn_string>;
using artist_repository = database::artist_repository<artist, types::artist_filter, conn_string>;
using cover_art_repository = database::cover_art_repository<cover_art, types::cover_filter, conn_string>;
using genre_repository = database::genre_repository<genre, types::genre_filter, conn_string>;
using song_repository = database::song_repository<song, types::song_filter, conn_string>;
using token_repository = database::token_repository<token, types::token_filter, conn_string>;
using user_repository = database::user_repository<user, pass_sec, 
    types::salt_filter, types::user_filter, conn_string>;
*/

}


#endif
