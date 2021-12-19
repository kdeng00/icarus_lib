#ifndef REPO_UTIL_H_
#define REPO_UTIL_H_

#include <iostream>
#include <memory>
#include <string>
#include <cstring>

#include <mysql/mysql.h>

namespace icarus_lib::repositories::database::cloud {

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
        params.get()[pos].buffer = (char *)buffer;
        params.get()[pos].buffer_length = length;
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

};

}


#endif
