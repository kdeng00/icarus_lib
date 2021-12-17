#ifndef ICARUS_H_
#define ICARUS_H_

#include <string>
#include <vector>

#include "icarus.h"
#include "models/models.hpp"
#include "types/auth.hpp"
#include "types/music.hpp"
#include "repositories/database/cloud/user_repository.hpp"

namespace icarus_lib
{

using raw_data = unsigned char;

using song = models::song_type<std::string, int, long, raw_data>;
using artist = models::artist_type<song, std::string, int>;
using album = models::album_type<song, std::string, int>;
using genre = models::genre_type<song, std::string, int>;
// TODO: Will not be used
// using year = year_type<int>;
using cover = models::cover_type<song, std::string, int, raw_data>;

using token = models::token_type<std::string, int, std::chrono::system_clock::time_point>;
using login_result = models::login_result_type<std::string, int>;
using register_result = models::register_result_type<std::string>;
using user = models::user_type<std::string, int>;
using pass_sec = models::pass_sec_type<std::string, int>;
using auth_credentials = models::auth_credentials_type<std::string>;

using database_connection = models::database_connection_type<std::string>;
using conn_string = models::database_connection_type<std::string>;
using binary_path = models::binary_path_type<std::string>;



using song_filter = types::song_filter;
using artist_filter = types::artist_filter;
using album_filter = types::album_filter;
using genre_filter = types::genre_filter;
using cover_art_filter = types::cover_filter;
using token_filter = types::token_filter;
using user_filter = types::user_filter;
using salt_filter = types::salt_filter;

using user_repository = database::user_repository<user, pass_sec, 
    salt_filter, user_filter, conn_string>;

};

#endif
