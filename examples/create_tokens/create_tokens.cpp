#include "example.hpp"
#include "icarus_lib/icarus.hpp"

template<class Token, typename Container = std::vector<Token>>
Container test_tokens()
{
    Container elements;
    constexpr auto amount = 3;
    
    for (auto i = 0; i < amount; ++i)
    {
        Token token;
        token.access_token = "dsfz3gbv23rfwb346453b23d;'";
        token.token_type = "Bearer";
        token.user_id = 1;

        elements.emplace_back(std::move(token));
    }


    return elements;
}


int main(int argc, char **argv)
{
    const string name("create_tokens");
    example::count_check(argc, name);
    const auto conn_str = example::test_connection_string<icarus_lib::conn_string>(argv);

    auto token_repo = icarus_lib::token_repository(conn_str);
    const auto tokens = test_tokens<icarus_lib::token>();

    for (const auto &token : tokens)
    {
        token_repo.create_token(token);
    }

    return 0;
}
