#ifndef MUSIC_MODELS_H_
#define MUSIC_MODELS_H_

#include <string>
#include <vector>


namespace icarus_lib::models
{
template<typename str_val, typename int_val,
         typename long_val>
class song_type
{
public:
    song_type() = default;
    song_type(const int_val id) : id(id)
    {
    }


    int_val id;
    str_val title;
    str_val artist;
    str_val album;
    str_val album_artist;
    str_val genre;
    int_val year;
    long_val duration;
    int_val track;
    int_val disc;
    str_val song_path;
    std::vector<unsigned char> data;
    int_val cover_art_id;
    int_val artist_id;
    int_val album_id;
    int_val genre_id;
    int_val year_id;
};


template<class song_val, typename str_val, typename int_val>
class artist_type
{
public:
    artist_type() = default;
    artist_type(const song_val &song) : id(song.artist_id), 
            name(song.artist) { }
    artist_type(const int_val id) : id(id) { }

    int_val id;
    str_val name;
};

template<class song_val, typename str_val, typename int_val>
class album_type
{
public:
    album_type() = default;
    album_type(const song_val &song) : 
            id(song.album_id), title(song.album), artist(song.artist), 
            year(song.year) { }
    album_type(const int_val id) : id(id) { }

    int_val id;
    str_val title;
    str_val artist;
    int_val year;
    std::vector<song_val> songs;
};

template<class song_val, typename str_val, typename int_val>
class genre_type
{
public:
    genre_type() = default;
    genre_type(const song_val &song) : 
            id(song.genre_id), category(song.genre) { }
    genre_type(const int_val id) : id(id) { }

    int_val id;
    str_val category;

};

template<typename int_val>
class year_type
{
public:
    year_type() = default;
    year_type(const song_type<std::string, int_val, long> &song) : id(song.year_id), song_year(song.year)
    {
    }
    year_type(const int_val id) : id(id)
    {
    }

    int_val id;
    int_val song_year;
};

template<class song_val, typename str_val, typename int_val>
class cover_type
{
public:
    cover_type() = default;
    cover_type(const song_val &song) : 
            id(song.cover_art_id), song_title(song.title) { }
    cover_type(const int_val id) : id(id) { }

    int_val id;
    str_val song_title;
    str_val image_path;
    // Not being used but it should be
    std::vector<unsigned char> data;
};
}

#endif
