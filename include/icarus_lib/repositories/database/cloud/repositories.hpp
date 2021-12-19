#ifndef REPOSITORIES_H_
#define REPOSITORIES_H_

// #include "icarus_lib/repositories/database/cloud/.hpps
#include "icarus_lib/repositories/database/cloud/cover_art_repository.hpp"
#include "icarus_lib/repositories/database/cloud/genre_repository.hpp"
#include "icarus_lib/repositories/database/cloud/artist_repository.hpp"
#include "icarus_lib/repositories/database/cloud/album_repository.hpp"
#include "icarus_lib/repositories/database/cloud/user_repository.hpp"
#include "icarus_lib/repositories/database/cloud/song_repository.hpp"
#include "icarus_lib/repositories/database/cloud/token_repository.hpp"
#include "icarus_lib/models/models.hpp"
#include "icarus_lib/types/types.hpp"


namespace icarus_lib::database
{

using song_repository = icarus_lib::repositories::database::cloud::song_repository<models::song, types::song_filter, models::conn_string>;
using token_repository = icarus_lib::repositories::database::cloud::token_repository<models::token, types::token_filter, models::conn_string>;

using album_repository = icarus_lib::repositories::database::cloud::album_repository<models::album, types::album_filter, models::conn_string>;
using artist_repository = icarus_lib::repositories::database::cloud::artist_repository<models::artist, types::artist_filter, models::conn_string>;
using cover_art_repository = icarus_lib::repositories::database::cloud::cover_art_repository<models::cover_art, types::cover_filter, models::conn_string>;
using genre_repository = icarus_lib::repositories::database::cloud::genre_repository<models::genre, types::genre_filter, models::conn_string>;
using user_repository = icarus_lib::repositories::database::cloud::user_repository<models::user, models::pass_sec, 
    types::salt_filter, types::user_filter, models::conn_string>;
}


#endif