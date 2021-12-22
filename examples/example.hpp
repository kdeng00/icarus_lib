#ifndef EXAMPLE_H_
#define EXAMPLE_H_

#include <iostream>
#include <string>
#include <memory>
#include <vector>


#include "icarus_lib/icarus.hpp"

using icarus_lib::conn_string;

using namespace std;


namespace example {


template<typename ConnString>
ConnString test_connection_string(char **argv)
{
    icarus_lib::binary_path bin_path(argv[1]);
    const auto conn_str_json = icarus_lib::directory_manager::raw_database_config_content(bin_path.path);

    ConnString conn_str;
    conn_str.host = conn_str_json["server"].get<string>();
    conn_str.database = conn_str_json["database"].get<string>(); 
    conn_str.username = conn_str_json["username"].get<string>();
    conn_str.password = conn_str_json["password"].get<string>();

    std::cout << conn_str.host << " " << conn_str.database << " " << conn_str.username << " ";
    std::cout << conn_str.password << "\n";

    return conn_str;
}

void count_check(int argc, const std::string &name)
{
    std::cout << name << "\n";

    if (argc < 2)
    {
        std::cout << "Pass path containing database connection details like so:\n";
        std::cout << "./" << name << " \"database.json\"\n";
        std::cout << "Terminating...\n";

        std::terminate();
    }
}

}

#endif
