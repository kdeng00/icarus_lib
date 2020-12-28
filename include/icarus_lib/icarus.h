#ifndef ICARUS_H_
#define ICARUS_H_

#include <string>

namespace icarus_lib
{
    class song
    {
    public:
        song(const int id);


        int id;
        std::string title;
        std::string artist;
        std::string album;
    };
};

#endif
