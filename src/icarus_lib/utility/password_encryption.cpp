#include "icarus_lib/utility/password_encryption.hpp"

#include "cryptopp/sha.h"
#include "cryptopp/filters.h"
#include "cryptopp/base64.h"



namespace icarus_lib::utility
{

models::pass_sec password_encryption::hash_password(const models::user &user)
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

}