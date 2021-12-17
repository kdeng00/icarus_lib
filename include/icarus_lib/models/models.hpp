#ifndef MODELS_H_
#define MODELS_H_

#include <string>
#include <vector>
#include <chrono>
#include <utility>

#include "auth_models.hpp"
#include "config_models.hpp"
#include "music_models.hpp"

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

}


#endif
