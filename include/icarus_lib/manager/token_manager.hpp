#ifndef TOKEN_MANAGER_H_
#define TOKEN_MANAGER_H_

#include <string>
#include <sstream>
#include <string_view>
#include <utility>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <chrono>

#include <cpr/cpr.h>
// #include <icarus_lib/icarus.h>
#include <jwt-cpp/jwt.h>
#include <nlohmann/json.hpp>

// #include "manager/managers.hpp"
#include "icarus_lib/manager/base_manager.hpp"
#include "icarus_lib/manager/directory_manager.hpp"
#include "icarus_lib/models/models.hpp"
#include "icarus_lib/repositories/database/cloud/token_repository.hpp"
#include "icarus_lib/types/types.hpp"

using icarus_lib::manager::base_manager;

namespace icarus_lib::manager
{

template<class Token>
class token_manager : public base_manager<models::binary_path>
{
public:
    token_manager() = default;
    token_manager(models::binary_path &config) : base_manager(config)
    {
    }

    Token create_token(const models::binary_path &config, const models::user& usr)
    {
        std::cout << "Fetching icarus key config\n";
        auto t = directory_manager::keyConfigContent(config);

        std::string private_key_path(t["rsa_private_key_path"]);
        std::string public_key_path(t["rsa_public_key_path"]);

        auto private_key = directory_manager::contentOfPath(private_key_path);
        auto public_key = directory_manager::contentOfPath(public_key_path);

        auto current_time = std::chrono::system_clock::now();

        auto scopes = all_scopes<std::string_view>();
        auto ss = all_scopes_spaced<std::string_view>(scopes);

        Token token;
        token.issued = current_time;
        token.expires = current_time + hours(hours_till_expire());

        auto tok = jwt::create()
            .set_issuer("icarus")
            .set_type("JWS")
            .set_issued_at(token.issued)
            .set_expires_at(token.expires)
            .set_payload_claim("scope", jwt::claim(ss))
            .set_payload_claim("username", jwt::claim(usr.username))
            .sign(jwt::algorithm::rs256(public_key, private_key, "", ""));

        token.access_token = tok;

        database::token_repository repo(config);
        repo.create_token(token, usr);


        return token;
    }


    template<typename scope = types::scope, template <typename, typename> class token_result = std::pair>
    auto is_token_valid(Token &tokn, scope scp)
    {
        token_result<bool, std::string> result;

        auto decoded = is_token_verified<jwt::decoded_jwt, std::pair>(tokn);

        if (!decoded.first)
        {
            result.first = false;
            result.second = "Invalid";

            return result;
        }

        for (auto &payload : decoded.second.get_payload_claims())
        {
            if (payload.first.compare("iat") == 0)
            {
                auto issued_val = payload.second;
            }
            else if (payload.first.compare("exp") == 0)
            {
                auto expired_val = payload.second;
            }
            else if (payload.first.compare("scope") == 0)
            {
            }
        }

        result.first = true;
        result.second = "Valid token";

        return result;
    }

    bool isTokenValid(std::string &auth, types::scope scope)
    {
        auto authPair = fetchAuthHeader(auth);

        if (!std::get<0>(authPair)) {
            std::cout << "no Bearer found\n";

            return std::get<0>(authPair);
        }

        auto authHeader = std::get<1>(authPair);

        auto token = authHeader.at(authHeader.size()-1);

        auto scopes = extractScopes(jwt::decode(token));

        switch (scope)
        {
        case types::scope::UPLOAD:
            return tokenSupportsscope(scopes, "upload:songs");
        case types::scope::DOWNLOAD:
            return tokenSupportsscope(scopes, "download:songs");
        case types::scope::STREAM:
            return tokenSupportsscope(scopes, "stream:songs");
        case types::scope::DELETE_SONG:
            return tokenSupportsscope(scopes, "delete:songs");
        case types::scope::UPDATE_SONG:
            return tokenSupportsscope(scopes, "update:songs");
        case types::scope::RETRIEVE_SONG:
            return tokenSupportsscope(scopes, "read:song_details");
        case types::scope::RETRIEVE_ALBUM:
            return tokenSupportsscope(scopes, "read:albums");
        case types::scope::RETRIEVE_ARTIST:
            return tokenSupportsscope(scopes, "read:artists");
        case types::scope::RETRIEVE_GENRE:
            return tokenSupportsscope(scopes, "read:genre");
        case types::scope::RETRIEVE_YEAR:
            return tokenSupportsscope(scopes, "read:year");
        case types::scope::DOWNLOAD_COVER_ART:
            return tokenSupportsScope(scopes, "download:cover_art");
        default:
            break;
        }

        return false;
    }
private:
    template<typename str_type = std::string, typename container = std::vector<str_type>,
                typename jwt_decoded = jwt::decoded_jwt>
    container extractScopes(jwt_decoded &&decoded)
    {
        container scopes;
        auto payload_claims = decoded.get_payload_claims();
        scopes.reserve(payload_claims.size());

        for (auto &d : payload_claims)
        {
            if (d.first.compare("scope") == 0) {
                std::cout << "found scope\n";
                auto scopes_js = d.second.to_json();
                auto scs = scopes_js.template get<str_type>();
                str_type allScopes(scs);
                std::istringstream iss(allScopes);

                scopes.assign(std::istream_iterator<str_type>(iss), std::istream_iterator<str_type>());
            }
        }

        return scopes;
    }


    template<typename str_type = std::string, typename container = std::vector<str_type>,
                typename pair = std::pair<bool, container>>
    pair fetchAuthHeader(const str_type &auth)
    {
        std::istringstream iss(auth);
        container authHeader{std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>()};

        bool foundBearer = false;

        auto lamb = std::any_of(authHeader.begin(), authHeader.end(), [&](std::string_view word)
            { 
                return (word.compare("Bearer") == 0); 
            });

        if (lamb)
        {
            std::cout << "Bearer found\n";
            foundBearer = true;
        }

        return std::make_pair(foundBearer, authHeader);
    }


    template<typename decode, template <typename,typename> class pair>
    pair<bool, decode> is_token_verified(const Token &tokn)
    {
        auto t = directory_manager::keyConfigContent(m_config);

        std::string private_key_path(t["rsa_private_key_path"]);
        std::string public_key_path(t["rsa_public_key_path"]);

        auto private_key = directory_manager::contentOfPath(private_key_path);
        auto public_key = directory_manager::contentOfPath(public_key_path);

        auto verify = jwt::verify()
            .allow_algorithm(jwt::algorithm::rs256(public_key, private_key, "", ""
                        ))
            .with_issuer("icarus");
        auto decoded = jwt::decode(tokn.access_token);

        try
        {
            verify.verify(decoded);
        }
        catch (jwt::signature_verification_exception &e)
        {
            auto s = e;
            std::cout << "Something happend: " << e.what() << "\n";
            return std::make_pair(false, jwt::decoded_jwt(tokn.access_token));
        }

        return std::make_pair(true, decoded);
    }


    template<typename val_t, typename array_t = std::array<val_t, 11>>
    constexpr array_t all_scopes() noexcept
    {
        constexpr array_t scopes{"upload:songs",
                                    "download:songs",
                                    "stream:songs",
                                    "delete:songs",
                                    "update:songs",
                                    "read:song_details",
                                    "read:albums",
                                    "read:artists",
                                    "read:genre",
                                    "read:year",
                                    "download:cover_art"};

        return scopes;
    }

    template<typename val_t, typename array_t = std::array<val_t, 11>>
    auto all_scopes_spaced(array_t &scopes)
    {
        std::stringstream ss;
        ss << scopes[0];

        std::for_each(scopes.begin() + 1, scopes.end(), [&ss](val_t v)
                {
                    ss << " " << v;
                });

        return ss.str();
    }


    constexpr auto hours_till_expire() noexcept
    {
        return 4;
    }

    // Hours in which a refreshed token will expire
    constexpr auto refresh_token_expiration() noexcept
    {
        return 24;
    }


    template<typename str_val = std::string, typename container = std::vector<str_val>>
    auto tokenSupportsScope(const container &scopes, str_val &&scope)
    {
        return std::any_of(scopes.begin(), scopes.end(), [&scope](std::string_view foundScope)
            {
                return (foundScope.compare(scope) == 0);
            });
    }
};

}

#endif