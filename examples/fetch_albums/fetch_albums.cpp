#include <iostream>

#include "example.hpp"
#include "icarus_lib/icarus.h"


using std::cout;


void print_album(const icarus_lib::album &album)
{
    cout << "ID: " << album.id << "\n";
    cout << "Title: " << album.title << "\n";
    cout << "Year: " << album.year << "\n";
}

int main(int argc, char **argv)
{
    const std::string name("fetch_albums");

    example::count_check(argc, name);
    const auto conn_str = example::test_connection_string<conn_string>(argv);

    auto album_repo = icarus_lib::album_repository(conn_str);
    auto album = icarus_lib::album();
    album.id = 40;
    album = album_repo.retrieveRecord(album, icarus_lib::types::album_filter::ID);

    print_album(album);

    auto album_with_song_count = album_repo.retrieveRecordWithSongCount(album, icarus_lib::types::album_filter::ID);
    print_album(album_with_song_count.first);


    return 0;
}
