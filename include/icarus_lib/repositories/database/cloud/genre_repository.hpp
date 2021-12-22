#ifndef GENREREPOSITORY_H_
#define GENREREPOSITORY_H_

#include <memory>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>

#include <mysql/mysql.h>

#include "icarus_lib/repositories/database/cloud/base_repository.h"
#include "icarus_lib/models/models.hpp"
#include "icarus_lib/types/types.hpp"

namespace icarus_lib { namespace database {

class genre_repository : public base_repository
{
public:
    genre_repository(const models::conn_string &details, const std::string &tablename = "Genre");
    genre_repository(const models::binary_path &details, const std::string tablename = "Genre");


    std::vector<models::genre> retrieveRecords();
    

    std::pair<models::genre, int> retrieveRecordWithSongCount(
            models::genre& genre, types::genre_filter filter = types::genre_filter::ID); 

    models::genre retrieveRecord(models::genre& genre, types::genre_filter filter); 

    bool doesGenreExist(const models::genre& genre, types::genre_filter filter); 

    void saveRecord(const models::genre& genre); 

    void deleteRecord(const models::genre& genre, 
            types::genre_filter filter = types::genre_filter::ID); 
private:


    std::vector<models::genre> parseRecords(MYSQL_STMT *stmt); 

    std::pair<models::genre, int> parseRecordWithSongCount(MYSQL_STMT *stmt); 


    std::shared_ptr<MYSQL_BIND> valueBind(models::genre& genre,
            std::tuple<char*>& metadata); 

    std::shared_ptr<MYSQL_BIND> valueBindWithSongCount(models::genre& genre,
            std::tuple<char*>& metadata, int& songCount); 


    std::tuple<char*> metadataBuffer(); 


    models::genre parseRecord(MYSQL_STMT *stmt); 
};

}}

#endif
