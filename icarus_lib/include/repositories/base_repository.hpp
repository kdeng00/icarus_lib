#ifndef BASE_REPOSITORIES_H_
#define BASE_REPOSITORIES_H_

#include <string>

#include "models/models.hpp"

namespace icarus_lib::repositories::database::cloud
{
class base_repository
{
public:
    base_repository() = default;
    base_repository(const std::string &path, const std::string &table_name);
    base_repository(const icarus_lib::models::binary_path<> *config);
private:
};
}

#endif