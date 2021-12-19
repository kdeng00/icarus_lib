#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "example.hpp"
#include "icarus_lib/icarus.hpp"

using std::cout;
using std::vector;

using song = icarus_lib::song;
using conn_string = icarus_lib::conn_string;
using song_repository = icarus_lib::song_repository;


conn_string test_connection_string(char **argv)
{
    auto conn_str = conn_string();
    conn_str.host = argv[1];
    conn_str.database = argv[2];
    conn_str.username = argv[3];
    conn_str.password = argv[4];

    return conn_str;
}

template<typename Song>
void print_song(const Song song)
{
    cout << "ID: " << song.id << "\n";
    cout << "Title: " << song.title << "\n";
    cout << "Artist: " << song.artist << "\n";
    cout << "Album: " << song.album << "\n";
    cout << "Album Artist: " << song.album_artist << "\n";
    cout << "Genre: " << song.genre << "\n";
    cout << "Year: " << song.year << "\n";
    cout << "Duration: " << song.duration << "\n";
    cout << "Track: " << song.track << "\n";
    cout << "Disc: " << song.disc << "\n";
    cout << "\n";
}

template<typename Song, typename Con>
void print_songs(const Con songs)
{
    if (songs.size() > 0)
    {
        cout << "Printing songs\n\n";

        std::for_each(songs.begin(), songs.end(), [](Song song)
        {
            print_song<Song>(song);
        });
    }
    else
    {
        cout << "No songs to print\n";
    }
}


int main(int argc, char **argv)
{
    const std::string name("fetch_songs");
    example::count_check(argc, name);

    const auto conn_str = example::test_connection_string<conn_string>(argv);

    auto song_repo = song_repository(conn_str);

    const auto songs = song_repo.retrieveRecords();
    print_songs<song, vector<song>>(songs);

    const auto song_my = song_repo.retrieveRecord(songs.at(0), icarus_lib::types::song_filter::TITLE_AND_ARTIST);

    cout << "Printing song:\n";
    print_song<icarus_lib::song>(song_my);


    return 0;
}
