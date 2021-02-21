#ifndef MODELS_H_
#define MODELS_H_

#include <string>
#include <vector>
#include <chrono>
#include <utility>

namespace icarus_lib::models
{
    template<typename str_val, typename int_val,
             typename long_val>
    class song
    {
    public:
        song() = default;
        song(const int_val id) : id(id)
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


    template<typename str_val, typename int_val>
    class artist {
    public:
        artist() = default;
        artist(const song<str_val, int_val, long> &song) : id(song.artist_id), 
                name(song.artist) { }
        artist(const int_val id) : id(id) { }

        int_val id;
        str_val name;
    };

    template<typename song_val, typename str_val, typename int_val>
    class album {
    public:
        album() = default;
        album(const song_val &song) : 
                id(song.album_id), title(song.album), artist(song.artist), 
                year(song.year) { }
        album(const int_val id) : id(id) { }

        int_val id;
        str_val title;
        str_val artist;
        int_val year;
        std::vector<song_val> songs;
    };

    template<typename str_val, typename int_val>
    class genre {
    public:
        genre() = default;
        genre(const song<str_val, int_val, long> &song) : 
                id(song.genre_id), category(song.genre) { }
        genre(const int_val id) : id(id) { }

        int_val id;
        str_val category;

    };

    template<typename int_val>
    class year {
    public:
        year() = default;
        year(const song<std::string, int_val, long> &song) : id(song.year_id), song_year(song.year)
        {
        }
        year(const int_val id) : id(id)
        {
        }

        int_val id;
        int_val song_year;
    };

    template<typename str_val, typename int_val>
    class cover {
    public:
        cover() = default;
        cover(const song<str_val, int_val, long> &song) : 
                id(song.cover_art_id), song_title(song.title) { }
        cover(const int_val id) : id(id) { }

        int_val id;
        str_val song_title;
        str_val image_path;
        // Not being used but it should be
        std::vector<unsigned char> data;
    };

    template<typename str_val, typename int_val, typename time_p = std::chrono::system_clock::time_point>
    class token {
    public:
        token() = default;
        token(const str_val &access_token) :
                access_token(access_token) { }
        token(const str_val &access_token, const str_val &token_type, 
                const int_val expiration) :
                access_token(access_token), token_type(token_type), 
                expiration(expiration) { }

        int_val token_id;
        str_val access_token;
        int_val expiration;
        str_val token_type;
        time_p issued;
        time_p expires;
        bool refresh_token;
        bool active;
        int_val user_id;
    };

    template<typename str_val, typename int_val>
    struct login_result {
        int_val user_id;
        str_val username;
        str_val access_token;
        str_val token_type;
        str_val message;
        int_val expiration;
    };

    template<typename str_val>
    class register_result {
    public:
        register_result() = default;
        register_result(const str_val &username, bool registered,
                        const str_val &message) : 
                            username(username), registered(registered),
                            message(message)
        {
        }
        register_result(str_val &&username, bool registered,
                        str_val &&message) : 
                            username(std::move(username)), registered(registered),
                            message(std::move(message))
        {
        }


        str_val username;
        bool registered;
        str_val message;
    };

    template<typename str_val, typename int_val>
    struct user {
        int id;
        str_val firstname;
        str_val lastname;
        str_val email;
        str_val phone;
        str_val username;
        str_val password;
    };

    template<typename str_val, typename int_val>
    struct pass_sec {
        int_val id;
        str_val hash_password;
        str_val salt;
        int_val user_id;
    };

    template<typename str_val>
    struct auth_credentials {
        str_val domain;
        str_val api_identifier;
        str_val client_id;
        str_val client_secret;
        str_val uri;
        str_val endpoint;
    };

    template<typename str_val>
    struct database_connection {
        str_val server;
        str_val username;
        str_val password;
        str_val database;
    };

    template<typename str_val>
    class binary_path {
    public:
        binary_path() = default;
        binary_path(const char *p) : path(p) { }
        binary_path(const str_val& p) : path(p) { }
        binary_path(str_val&& p) : path(std::move(p)) { }

        str_val path;
    };
}


#endif
