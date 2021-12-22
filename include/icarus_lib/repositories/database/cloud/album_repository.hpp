#ifndef ALBUMREPOSITORY_H_
#define ALBUMREPOSITORY_H_

#include <iostream>
#include <memory>
#include <utility>
#include <vector>
#include <string>
#include <sstream>
#include <cstring>

#include "mysql/mysql.h"

#include "icarus_lib/repositories/database/cloud/base_repository.h"
#include "icarus_lib/repositories/database/cloud/repository_utility.h"
#include "icarus_lib/types/types.hpp"

namespace icarus_lib::database {

class album_repository : public base_repository
{
public:
    album_repository(const models::conn_string &conn_str, const std::string table = "Album");

    album_repository(const models::binary_path &conn_str, const std::string table = "Album");

    std::vector<models::album> retrieveRecords();

    std::pair<models::album, int> retrieveRecordWithSongCount(models::album &album, types::album_filter filter);

    models::album retrieveRecord(models::album &album, types::album_filter filter);

    bool doesAlbumExists(const models::album &album, types::album_filter filter);

    void saveAlbum(const models::album &album);
    void deleteAlbum(const models::album &album, types::album_filter filter);
private:
    std::vector<models::album> parseRecords(MYSQL_STMT *stmt);

    std::vector<std::pair<models::album, int>> parse_records_with_song_count(MYSQL_STMT *stmt);


    std::shared_ptr<MYSQL_BIND> valueBind(models::album &album, 
            std::tuple<char*, char*> &metadata);

    std::shared_ptr<MYSQL_BIND> valueBindWithSongCount(models::album &album, 
            std::tuple<char*, char*> &metadata, int &songCount);

    std::tuple<char*, char*> metadataBuffer();
};

}

#endif
