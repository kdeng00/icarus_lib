#ifndef ICARUS_H_
#define ICARUS_H_

#include <string>
#include <vector>

// #include "icarus.h"
#include "icarus_lib/models/models.hpp"
// #include "types/auth.hpp"
// #include "types/music.hpp"
#include "icarus_lib/types/types.hpp"

#include "icarus_lib/manager/managers.hpp"
#include "icarus_lib/repositories/database/cloud/album_repository.hpp"
#include "icarus_lib/repositories/database/cloud/artist_repository.hpp"
#include "icarus_lib/repositories/database/cloud/base_repository.h"
#include "icarus_lib/repositories/database/cloud/cover_art_repository.hpp"
#include "icarus_lib/repositories/database/cloud/genre_repository.hpp"
#include "icarus_lib/repositories/database/cloud/song_repository.hpp"
#include "icarus_lib/repositories/database/cloud/token_repository.hpp"
#include "icarus_lib/repositories/database/cloud/user_repository.hpp"

#include "icarus_lib/utility/metadata_retriever.hpp"
#include "icarus_lib/utility/image_file.hpp"

namespace icarus_lib
{

using raw_data = unsigned char;
using str_val = std::string;
using int_val = int;

using song = models::song_type<str_val, int_val, long, raw_data>;
using artist = models::artist_type<song, str_val, int_val>;
using album = models::album_type<song, str_val, int_val>;
using genre = models::genre_type<song, str_val, int_val>;
// TODO: Will not be used
// using year = year_type<int_val>;
using cover = models::cover_type<song, str_val, int_val, raw_data>;
using cover_art = models::cover_type<song, str_val, int_val, raw_data>;

using token = models::token_mod<str_val, int_val, std::chrono::system_clock::time_point>;
using login_result = models::login_result_type<str_val, int_val>;
using register_result = models::register_result_type<str_val>;
using user = models::user_type<str_val, int_val>;
using pass_sec = models::pass_sec_type<str_val, int_val>;
using auth_credentials = models::auth_credentials_type<str_val>;

using database_connection = models::database_connection_type<str_val>;
using conn_string = models::database_connection_type<str_val>;
using conn_str = models::database_connection_type<str_val>;
using binary_path = models::binary_path_type<str_val>;



using song_filter = types::song_filter;
using artist_filter = types::artist_filter;
using album_filter = types::album_filter;
using genre_filter = types::genre_filter;
using cover_art_filter = types::cover_filter;
using cover_filter = types::cover_filter;
using token_filter = types::token_filter;
using user_filter = types::user_filter;
using salt_filter = types::salt_filter;
using token_filter = types::token_filter;

using path_type = types::path_type;

using album_repository = database::album_repository<album, album_filter>;
using base_repository = database::base_repository;
using artist_repository = database::artist_repository<artist, artist_filter>;
using cover_art_repository = database::cover_art_repository<cover_art, cover_filter>;
using genre_repository = database::genre_repository<genre, genre_filter>;
using song_repository = database::song_repository<song, song_filter>;
// using token_repository = repositories::database::cloud::token_repository<token, token_filter>;
using token_repository = database::token_repository<token, token_filter>;
using user_repository = database::user_repository;

using directory_manager = manager::directory_manager;

using metadata_retriever = utility::metadata_retriever;
using image_file = utility::image_file;

};

#endif
