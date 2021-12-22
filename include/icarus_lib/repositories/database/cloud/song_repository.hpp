#ifndef SONGREPOSITORY_H_
#define SONGREPOSITORY_H_

#include <memory>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>

#include <mysql/mysql.h>

#include "icarus_lib/repositories/database/cloud/base_repository.h"
#include "icarus_lib/repositories/database/cloud/repository_utility.h"

#include "icarus_lib/models/models.hpp"
#include "icarus_lib/types/types.hpp"

namespace icarus_lib { namespace database {

// TODO: Start using the construct_params_* functions from the repository_utility class
// Cuts down on code size
class song_repository : public base_repository
{
public:
    song_repository(const models::conn_str &conn_str, const std::string table = "Song");
    song_repository(const models::binary_path &conn_str, const std::string table = "Song");


    std::vector<models::song> retrieveRecords();


    models::song retrieveRecord(const models::song& song, types::song_filter filter);


    bool doesSongExist(const models::song& song, types::song_filter filter);

    bool deleteRecord(const models::song& song);

    void saveRecord(const models::song& song);

    void updateRecord(const models::song& song) ;

private:
    std::shared_ptr<MYSQL_BIND> valueBind(models::song& song,
            std::tuple<char*, char*, char*, char*, char*, char*>& metadata);


    std::tuple<char*, char*, char*, char*, char*, char*> metadataBuffer();


    std::vector<models::song> parseRecords(MYSQL_STMT *stmt);


    models::song parseRecord(MYSQL_STMT *stmt);

    void parse_record_string_field(models::song &song, std::tuple<char *, char *, char *, char *, char *, char *> metaBuff);
    
};

}}

#endif
