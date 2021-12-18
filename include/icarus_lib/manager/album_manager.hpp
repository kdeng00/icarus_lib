#ifndef ALBUMMANAGER_H_
#define ALBUMMANAGER_H_

#include <memory>

// #include "icarus_lib/icarus.h"
#include "manager/base_manager.hpp"
#include "models/models.hpp"
#include "repositories/database/cloud/album_repository.hpp"

namespace icarus_lib::manager {

template<class Album>
class album_manager : public base_manager<models::binary_path> {
public:
    album_manager(const models::binary_path &bConf) : base_manager(bConf)
    {
    }

    Album retrieveAlbum(Album&)
    {
        database::AlbumRepository albRepo(m_bConf);
        album = std::move(albRepo.retrieveRecord(album, type::AlbumFilter::title));

        return album;
    }
    Album saveAlbum(const icarus_lib::song&);

    void deleteAlbum(const icarus_lib::song&);
    void updateAlbum(icarus_lib::song&, const icarus_lib::song&);

    static void printAlbum(const Album&);
private:
    icarus_lib::binary_path  m_bConf;
};

}

#endif
