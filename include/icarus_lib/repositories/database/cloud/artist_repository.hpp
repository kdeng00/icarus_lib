#ifndef ARTISTREPOSITORY_H_
#define ARTISTREPOSITORY_H_

#include <memory>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>

#include <mysql/mysql.h>

#include "icarus_lib/repositories/database/cloud/base_repository.h"
#include "icarus_lib/types/types.hpp"

namespace icarus_lib { namespace database {

class artist_repository : public base_repository
{
public:
    artist_repository(const models::conn_string &conn_str, const std::string &tablename = "Artist");
    artist_repository(const models::binary_path &conn_str, const std::string &tablename = "Artist");


    std::vector<models::artist> retrieveRecords();
    

    std::pair<models::artist, int> retrieveRecordWithSongCount(models::artist& artist, types::artist_filter filter = types::artist_filter::ID);
    

    models::artist retrieveRecord(models::artist& artist, types::artist_filter filter);
    

    bool doesArtistExist(const models::artist& artist, types::artist_filter filter);
    

    void saveRecord(const models::artist& artist);
    

    void deleteArtist(const models::artist& artist, 
            types::artist_filter filter = types::artist_filter::ID);
    
private:

    std::vector<models::artist> parseRecords(MYSQL_STMT *stmt);
    


    std::pair<models::artist, int> parseRecordWithSongCount(MYSQL_STMT *stmt);
    


    std::shared_ptr<MYSQL_BIND> valueBind(models::artist& artist,
            std::tuple<char*>& metadata);
    

    std::shared_ptr<MYSQL_BIND> valueBindWithSongCount(models::artist& artist,
            std::tuple<char*>& metadata, int& songCount);
    


    std::tuple<char*> metadataBuffer();
    


    models::artist parseRecord(MYSQL_STMT *stmt);
};
}}

#endif
