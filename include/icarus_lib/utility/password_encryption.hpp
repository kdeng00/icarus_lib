#ifndef PASSWORDENCRYPTION_H_
#define PASSWORDENCRYPTION_H_

#include "icarus_lib/models/models.hpp"

namespace icarus_lib::utility
{

class password_encryption
{
public:
    password_encryption() = default;

    models::pass_sec hash_password(const models::user &user);
    

    bool is_password_valid(const models::user &user, const models::pass_sec &salt)
    {
        const auto hashed = this->hash_password(user);

        return salt.hash_password.compare(hashed.hash_password) == 0 ? true : false;
    }

    constexpr auto salt_size() noexcept
    {
        return 10000;
    }
private:
};

}


#endif