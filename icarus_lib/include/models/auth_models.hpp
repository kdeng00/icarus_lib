#ifndef AUTH_MODELS_H_
#define AUTH_MODELS_H_

namespace icarus_lib::models
{
template<typename str_val, typename int_val, typename time_p>
class token_type {
public:
    token_type() = default;
    token_type(const str_val &access_token) :
            access_token(access_token) { }
    token_type(const str_val &access_token, const str_val &token_type, 
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
class login_result_type {
    int_val user_id;
    str_val username;
    str_val access_token;
    str_val token_type;
    str_val message;
    int_val expiration;
};

template<typename str_val>
class register_result_type {
public:
    register_result_type() = default;
    register_result_type(const str_val &username, bool registered,
                    const str_val &message) : 
                        username(username), registered(registered),
                        message(message)
    {
    }
    register_result_type(str_val &&username, bool registered,
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
struct user_type {
    int id;
    str_val firstname;
    str_val lastname;
    str_val email;
    str_val phone;
    str_val username;
    str_val password;
    // TODO: New fields. No columns have been created in the DB table
    str_val datecrated;
};

template<typename str_val, typename int_val>
struct pass_sec_type {
    int_val id;
    str_val hash_password;
    str_val salt;
    int_val user_id;
};

template<typename str_val>
struct auth_credentials_type {
    str_val domain;
    str_val api_identifier;
    str_val client_id;
    str_val client_secret;
    str_val uri;
    str_val endpoint;
};
}


#endif
