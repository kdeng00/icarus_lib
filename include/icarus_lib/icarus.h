#ifndef ICARUS_H_
#define ICARUS_H_

#include <string>
#include <vector>

#include "icarus_lib/models/models.hpp"

namespace icarus_lib
{
    using song = models::song<std::string, int, long>;
    using artist = models::artist<std::string, int>;
    using album = models::album<song, std::string, int>;
    using genre = models::genre<std::string, int>;
    using year = models::year<int>;
    using cover = models::cover<std::string, int>;


    using token = models::token<std::string, int>;
    using login_result = models::login_result<std::string, int>;
    using user = models::user<std::string, int>;
    using pass_sec = models::pass_sec<std::string, int>;
    

    using auth_credentials = models::auth_credentials<std::string>;
    using database_connection = models::database_connection<std::string>;
    using binary_path = models::binary_path<std::string>;
};

#endif
