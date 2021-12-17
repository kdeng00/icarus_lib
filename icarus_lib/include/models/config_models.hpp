#ifndef CONFIG_MODELS_H_
#define CONFIG_MODELS_H_


namespace icarus_lib::models
{
    template<typename str_val>
    class database_connection_type {
    public:
        str_val server;
        str_val username;
        str_val password;
        str_val database;
        int port;
    };

    template<typename str_val>
    class binary_path_type {
    public:
        binary_path_type() = default;
        binary_path_type(const char *p) : path(p) { }
        binary_path_type(const str_val& p) : path(p) { }
        binary_path_type(str_val&& p) : path(std::move(p)) { }

        str_val path;
    };
}

#endif