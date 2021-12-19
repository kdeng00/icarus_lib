#ifndef TYPES_H_
#define TYPES_H_

#include "auth.hpp"
#include "music.hpp"
#include "path_type.hpp"


namespace icarus_lib::types
{

using song_filter = types::song_filter;
using artist_filter = types::artist_filter;
using album_filter = types::album_filter;
using genre_filter = types::genre_filter;
using cover_art_filter = types::cover_filter;

using token_filter = types::token_filter;
using user_filter = types::user_filter;
using salt_filter = types::salt_filter;

using scope = types::scope;

using path_type = types::path_type;

}


#endif