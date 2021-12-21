#include <iostream>
#include <vector>
#include <string>
#include <filesystem>

#include "example.hpp"

#include "icarus_lib/icarus.hpp"

using std::vector;
using std::string;
using std::cout;

// Album
// Artist
// Genre
// CoverArt

template<class Song, typename Container = vector<Song>>
Container test_songs(const string &pathStr)
{
    constexpr auto amount = 3;

    filesystem::path my_path(pathStr);
    vector<string> paths;

    auto i = 0;
    for (auto &p : filesystem::directory_iterator(my_path))
    {
        cout << "File: " << p << "\n";
        const auto pa = p.path();
        const auto filename = pa.string();
        const auto extension = pa.extension();
        // cout << "" << extension << "\n";

        auto is_song = extension.compare(".mp3") == 0;

        if (is_song && i < amount)
        {
            paths.push_back(filename);
            ++i;
        }
        else if (is_song && i >= amount)
        {
            break;
        }
    }

    Container container;
    for (auto &p: paths)
    {
        Song song;
        song.song_path = p;
        container.push_back(song);
    }


    return container;
}


int main(int argc, char **argv)
{
    const string name("create_songs");
    example::count_check(argc, name);
    const auto conn_str = example::test_connection_string<icarus_lib::conn_string>(argv);
    const string path_val(argv[5]);

    cout << "Retrieving songs from directory: " << path_val << "\n";

    icarus_lib::song_repository song_repo(conn_str);
    auto songs = test_songs<icarus_lib::song>(path_val);

    for (auto &song : songs)
    {

    }

    return 0;
}
