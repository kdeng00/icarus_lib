#ifndef ICARUS_H_
#define ICARUS_H_

#include <string>
#include <vector>

namespace icarus_lib
{
    class song
    {
    public:
        song(const int id);


        int id;
        std::string title;
        std::string artist;
        std::string album;
        std::string album_artist;
        std::string genre;
        int year;
        long duration;
        int track;
        int disc;
        std::string song_path;
        std::vector<unsigned char> data;
        int cover_art_id;
        int artist_id;
        int album_id;
        int genre_id;
        int year_id;
    };
};

#endif
