#ifndef MANAGERS_H_
#define MANAGERS_H_

#include "models/models.hpp"
#include "manager/directory_manager.hpp"
#include "manager/album_manager.hpp"

namespace icarus_lib::manager
{

using directory_manager = manager::directory_manager;
using album_manager = manager::album_manager<models::album>;

}

#endif