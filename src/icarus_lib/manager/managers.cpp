#ifndef MANAGERS_H_
#define MANAGERS_H_

#include "icarus_lib/models/models.hpp"
#include "icarus_lib/manager/directory_manager.hpp"
// #include "icarus_lib/manager/album_manager.hpp"
#include "icarus_lib/manager/token_manager.hpp"

namespace icarus_lib::manager
{

using directory_manager = manager::directory_manager_t;
// using album_manager = manager::album_manager<models::album>;
using token_manager = manager::token_manager_t<models::token>;

}

#endif