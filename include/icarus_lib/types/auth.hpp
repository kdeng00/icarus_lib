#ifndef ICARUS_TYPES_H
#define ICARUS_TYPES_H

namespace icarus_lib::types {

enum class salt_filter
{
    ID = 0,
    SALT = 1,
    USER_ID = 2
};

enum class scope
{
    UPLOAD = 0,
    DOWNLOAD = 1,
    STREAM = 2,
    RETRIEVE_SONG = 3,
    UPDATE_SONG = 4,
    DELETE_SONG = 5,
    RETRIEVE_ALBUM = 6,
    RETRIEVE_ARTIST = 7,
    RETRIEVE_GENRE = 8,
    RETRIEVE_YEAR = 9,
    DOWNLOAD_COVER_ART = 10
};

enum class user_filter
{
    ID = 0,
    USERNAME = 1
};

enum class token_filter
{
    ACCESS_TOKEN = 0
};

}

#endif