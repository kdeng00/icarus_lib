#ifndef REPO_UTIL_H_
#define REPO_UTIL_H_

#include <iostream>
#include <memory>
#include <string>
#include <cstring>
#include <sstream>

#include "mysql/mysql.h"

namespace icarus_lib::database {

class repository_utility
{
public:
    // long maps to C int
    template<typename Bind = std::shared_ptr<MYSQL_BIND>, typename MySQLDataType = enum_field_types, 
        typename type = int>
    static void construct_param_long(Bind params, MySQLDataType buffer_type, const type &buffer, int pos)
    {
        params.get()[pos].buffer_type = buffer_type;
        params.get()[pos].buffer = (char *)&buffer;
        params.get()[pos].length = 0;
        params.get()[pos].is_null = 0;
    }

    template<typename Bind = std::shared_ptr<MYSQL_BIND>, typename MySQLDataType = enum_field_types, 
        typename type = std::string, typename SizeType = long unsigned int>
    static void construct_param_string(Bind params, MySQLDataType buffer_type, const type &buffer, int pos, 
        SizeType &length)
    {
        params.get()[pos].buffer_type = buffer_type;
        params.get()[pos].buffer = (char *)buffer.c_str();
        params.get()[pos].length = &length;
        params.get()[pos].is_null = 0;
    }


    template<typename Bind = std::shared_ptr<MYSQL_BIND>, typename MySQLDataType = enum_field_types, 
        typename type = int>
    static void construct_user_insert_long(Bind params, MySQLDataType buffer_type, type &buffer, int pos) 
    {
        params.get()[pos].buffer_type = buffer_type;
        params.get()[pos].buffer = (char *)&buffer;
    }

    template<typename Bind = std::shared_ptr<MYSQL_BIND>, typename MySQLDataType = enum_field_types, 
        typename type = std::string>
    static void construct_user_insert_string(Bind params, MySQLDataType buffer_type, type &buffer, int pos, 
        long unsigned int &length)
    {
        params.get()[pos].buffer_type = buffer_type;
        params.get()[pos].buffer = (char *)buffer.c_str();
        params.get()[pos].length = &length;
    }

    template<typename Bind = std::shared_ptr<MYSQL_BIND>, typename MySQLDataType = enum_field_types, 
        typename type = MYSQL_TIME>
    static void construct_insert_datetime(Bind params, MySQLDataType buffer_type, type &buffer, int pos, 
        long unsigned int &length)
    {
        params.get()[pos].buffer_type = buffer_type;
        params.get()[pos].buffer = (char *)&buffer;
        params.get()[pos].is_null = 0;
        params.get()[pos].length = 0;
    }

    // long maps to C int
    template<typename Bind = std::shared_ptr<MYSQL_BIND>, typename MySQLDataType = enum_field_types, 
        typename type = int>
    static void construct_param_bind_long(Bind params, MySQLDataType buffer_type, const type &buffer, int pos)
    {
        params.get()[pos].buffer_type = buffer_type;
        params.get()[pos].buffer = (char *)&buffer;
    }

    template<typename Bind = std::shared_ptr<MYSQL_BIND>, typename MySQLDataType = enum_field_types, 
        typename type = char *>
    static void construct_param_bind_cstring(Bind params, MySQLDataType buffer_type, type buffer, int pos, 
        long unsigned int &length)
    {
        params.get()[pos].buffer_type = buffer_type;
        params.get()[pos].buffer = buffer;
        params.get()[pos].buffer_length = length;
    }

    template<typename Bind = std::shared_ptr<MYSQL_BIND>, typename MySQLDataType = enum_field_types, 
        typename type = MYSQL_TIME>
    static void construct_param_bind_datetime(Bind params, MySQLDataType buffer_type, type &buffer, int pos, 
        long unsigned int &length)
    {
        params.get()[pos].buffer_type = buffer_type;
        params.get()[pos].buffer = (char *)&buffer;
        /**
        params.get()[pos].length = &length;
        params.get()[pos].is_null = &length;
        params.get()[pos].error = &length;
        */
    }

    template<typename Bind = std::shared_ptr<MYSQL_BIND>, typename MySQLDataType = enum_field_types, 
        typename type = std::string> 
    static void construct_param_bind_string(Bind params, MySQLDataType buffer_type, std::string &buffer, int pos, 
        long unsigned int &length)
    {
        params.get()[pos].buffer_type = buffer_type;
        params.get()[pos].buffer = (char *)buffer.c_str();
        params.get()[pos].buffer_length = length;
    }


    template<typename Date, typename str_val>
    static str_val convert_mysql_time_to_str(const Date &tm)
    {
        std::stringstream buff;
        buff << tm.year << "-";

        if (tm.month < 10)
        {
            buff << "0" << tm.month;
        }
        else
        {
            buff << tm.month;
        }

        buff << "-";

        if (tm.day < 10)
        {
            buff << "0" << tm.minute;
        }
        else
        {
            buff << tm.month;
        }

        buff << " ";

        if (tm.hour < 10)
        {
            buff << "0" << tm.hour;
        }
        else
        {
            buff << tm.hour;
        }

        buff << ":";

        if (tm.minute < 10)
        {
            buff << "0" << tm.minute;
        }
        else
        {
            buff << "" << tm.minute;
        }

        buff << ":";

        if (tm.second < 10)
        {
            buff << "0" << tm.second;
        }
        else
        {
            buff << "" << tm.second;
        }

        return buff.str();
    }

};

}


#endif
