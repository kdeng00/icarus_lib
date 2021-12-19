#ifndef PASSWORDENCRYPTION_H_
#define PASSWORDENCRYPTION_H_

#include "cryptopp/sha.h"
#include "cryptopp/filters.h"
#include "cryptopp/base64.h"

#include "models/models.hpp"

namespace icarus_lib::utility
{

class password_encryption
{
public:
    models::pass_sec hash_password(const models::user &user)
    {
        std::string digest;
        CryptoPP::SHA256 hash;

        CryptoPP::StringSource hashing(user.password, true,
            new CryptoPP::HashFilter(hash,
                new CryptoPP::Base64Encoder(
                    new CryptoPP::StringSink(digest))));

        models::pass_sec pass;
        pass.hash_password = digest;
        pass.user_id = user.id;
        pass.salt = "None";

        return pass;
    }

    bool is_password_valid(const models::user &user, const models::pass_sec &salt)
    {
        const auto hashed = hash_password(user);

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