#ifndef ICARUS_H_
#define ICARUS_H_

#include <string>
#include <vector>

// #include "icarus_lib/models/models.hpp"
#include "models/models.hpp"

namespace icarus_lib
{
    /**
    using song = models::song<std::string, int, long>;
    using artist = models::artist<std::string, int>;
    using album = models::album<song, std::string, int>;
    using genre = models::genre<std::string, int>;
    using year = models::year<int>;
    using cover = models::cover<std::string, int>;


    using token = models::token<std::string, int>;
    using login_result = models::login_result<std::string, int>;
    using register_result = models::register_result<std::string>;
    using user = models::user<std::string, int>;
    using pass_sec = models::pass_sec<std::string, int>;
    

    using auth_credentials = models::auth_credentials<std::string>;
    using database_connection = models::database_connection<std::string>;
    using binary_path = models::binary_path<std::string>;
    */


    /**
using song = song_type<std::string, int, long>;
using artist = artist_type<song, std::string, int>;
using album = album_type<song, std::string, int>;
using genre = genre_type<song, std::string, int>;
using year = year_type<int>;
using cover = cover_type<song, std::string, int>;

using token = token_type<std::string, int, std::chrono::system_clock::time_point>;
using login_result = login_result_type<std::string, int>;
using register_result = register_result_type<std::string>;
using user = user_type<std::string, int>;
using pass_sec = pass_sec_type<std::string, int>;
using auth_credentials = auth_credentials_type<std::string>;

using database_connection = database_connection_type<std::string>;
using binary_path = binary_path_type<std::string>;
*/
};

#endif
