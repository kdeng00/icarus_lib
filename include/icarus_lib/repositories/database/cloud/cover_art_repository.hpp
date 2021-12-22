#ifndef COVERARTREPOSITORY_H_
#define COVERARTREPOSITORY_H_

#include <memory>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>

#include <mysql/mysql.h>

#include "icarus_lib/models/models.hpp"
#include "icarus_lib/repositories/database/cloud/base_repository.h"
#include "icarus_lib/types/types.hpp"

namespace icarus_lib { namespace database {

class cover_art_repository : public base_repository
{
public:
    cover_art_repository(const models::conn_string &conn_str, const std::string &tablename = "CoverArt");
    cover_art_repository(const models::binary_path &conn_str, const std::string &tablename = "CoverArt");

    std::vector<models::cover_art> retrieveRecords();
    

    models::cover_art retrieve_record(models::cover_art &cover, types::cover_art_filter filter = types::cover_art_filter::ID);
    
    
    bool doesCoverArtExist(const models::cover_art &cover, types::cover_art_filter filter);
    

    void deleteRecord(const models::cover_art &cov, types::cover_art_filter filter = types::cover_art_filter::ID);
    
    void saveRecord(const models::cover_art &cov);
    
    void updateRecord(const models::cover_art &cover);
    
private:
    std::vector<models::cover_art> parseRecords(MYSQL_STMT *stmt);
    

    std::shared_ptr<MYSQL_BIND> valueBind(models::cover_art &cover,
            std::tuple<char*, char*> &metadata);
    

    std::tuple<char*, char*> metadataBuffer();
    

    models::cover_art parseRecord(MYSQL_STMT *stmt);
    
};

}}

#endif
