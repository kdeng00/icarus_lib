#ifndef MUSICFILTER_H_
#define MUSICFILTER_H_

namespace icarus_lib::types {

enum class album_filter
{
    ID = 0,
    TITLE = 1,
    YEAR = 2
};

enum class artist_filter
{
    ID = 0,
    ARTIST = 1
};

enum class cover_filter
{
    ID = 0,
    SONG_TITLE = 1,
    IMAGE_PATH = 2
};

enum class genre_filter
{
    ID = 0,
    CATEGORY = 1
};

enum class path_type
{
    MUSIC = 0,
    ARCHIVE = 1,
    TEMP = 2,
    COVER_ART = 3
};

enum class song_changed
{
    TITLE = 0,
    ARTIST = 1,
    ALBUM = 2,
    GENRE = 3,
    YEAR = 4
};

enum class song_filter
{
    ID = 0,
    TITLE = 1,
    ALBUM = 2,
    ARTIST = 3,
    ALBUM_ARTIST = 4,
    GENRE = 5,
    YEAR = 6,
    TITLE_AND_ARTIST = 7,
    TITLE_ALBUM_ARTIST_ALBUM = 8,
    TITLE_ALBUM_ARTIST_ALBUM_TRACK = 9
};

enum class song_upload
{
    SUCCESSFUL = 0,
    ALREADY_EXIST = 1,
    FAILED = 2
};

// NOTE: Might be removed
enum class year_filter
{
    ID = 0,
    YEAR = 1
};

}


#endif