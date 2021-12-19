#ifndef METADATA_RETRIEVER_H_
#define METADATA_RETRIEVER_H_

#include <iostream>
#include <string>

#include <taglib/attachedpictureframe.h>
#include <taglib/textidentificationframe.h>
#include <taglib/fileref.h>
#include <taglib/mpegfile.h>
#include <taglib/tag.h>

#include "image_file.hpp"
#include "models/models.hpp"
#include "manager/managers.hpp"

namespace icarus_lib::utility {

class metadata_retriever {
public:
    models::song retrieve_metadata(models::song &song)
    {
        TagLib::MPEG::File sameFile(song.song_path.c_str());
		auto tag = sameFile.ID3v2Tag();
		song.title = tag->title().toCString(true);
		song.artist = tag->artist().toCString(true);
		song.album = tag->album().toCString(true);
		song.genre = tag->genre().toCString(true);
		song.year = tag->year();
		song.track = tag->track();

		song.duration = sameFile.audioProperties()->lengthInSeconds();

		constexpr auto id3DiscName = "TPOS";
		constexpr auto id3AlbumArtistName = "TPE2";

		auto discFrame = tag->frameList(id3DiscName);
		auto cgFrame = tag->frameList(id3AlbumArtistName);
		if (discFrame.isEmpty()) {
		    constexpr auto discDefaultVal = "1";
		    TagLib::ID3v2::TextIdentificationFrame *emptyFrame = 
		            new TagLib::ID3v2::TextIdentificationFrame(id3DiscName);
		    tag->addFrame(emptyFrame);
		    emptyFrame->setText(discDefaultVal);
		    song.disc = std::atoi(discDefaultVal);
		    sameFile.save();
		} else {
		    song.disc = std::stoi(discFrame.front()->toString().toCString());
		}

		if (cgFrame.isEmpty()) {
		    TagLib::ID3v2::TextIdentificationFrame *emptyFrame = 
		            new TagLib::ID3v2::TextIdentificationFrame(id3AlbumArtistName);
		    tag->addFrame(emptyFrame);
		    emptyFrame->setText(song.artist.c_str());
		    sameFile.save();
		} else {
		    song.album_artist = cgFrame.front()->toString().toCString(true);
		}

		return song;
    }
    
    models::cover update_coverart(const models::song &song, models::cover &cov, const std::string &stockCoverPath)
    {
        TagLib::MPEG::File sngF(song.song_path.c_str());
		auto tag = sngF.ID3v2Tag();
		auto frameList = tag->frameListMap()["APIC"];

		if (frameList.isEmpty()) {
		    cov.image_path.append("CoverArt.png");
        
		    if (!fs::exists(cov.image_path)) {
		        std::cout << "copying stock cover path\n";
		        fs::copy(stockCoverPath, cov.image_path);
		    }

		    image_file stockImg(cov.image_path.c_str());

		    TagLib::ID3v2::AttachedPictureFrame *pic = 
				    new TagLib::ID3v2::AttachedPictureFrame;
		    pic->setPicture(stockImg.data());
		    pic->setType(TagLib::ID3v2::AttachedPictureFrame::FrontCover);

		    tag->addFrame(pic);

		    sngF.save();
		    std::cout << "applied stock cover art\n";
		} else {
		    auto frame = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame*>(
		            frameList.front());
        
		    auto imgPath = manager::DirectoryManager::createDirectoryProcess(song, cov.image_path);
		    imgPath.append(song.title);
		    imgPath.append(".png");
		    cov.image_path = imgPath;

		    std::fstream imgSave(cov.image_path, std::ios::out | 
		            std::ios::binary);
		    imgSave.write(frame->picture().data(), frame->picture().size());
		    imgSave.close();
		    std::cout << "saved to " << cov.image_path << "\n";
		}

		return cov;
    }

    models::cover apply_stock_coverart(const models::song &song, models::cover &cov, const std::string &stockCoverPath)
    {
        TagLib::MPEG::File songFile(song.song_path.c_str());
		auto tag = songFile.ID3v2Tag();

		image_file stockImg(cov.image_path.c_str());
		TagLib::ID3v2::AttachedPictureFrame *pic = 
				new TagLib::ID3v2::AttachedPictureFrame;

		pic->setPicture(stockImg.data());
		pic->setType(TagLib::ID3v2::AttachedPictureFrame::FrontCover);

		tag->addFrame(pic);

		songFile.save();
		std::cout << "applied stock cover art\n";

		delete pic;

		return cov;
    }

    models::cover apply_coverart(const models::song &song, models::cover &cov)
    {
        TagLib::MPEG::File songFile(song.song_path.c_str());
		auto tag = songFile.ID3v2Tag();
		auto frameList = tag->frameListMap()["APIC"];

		auto frame = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame*>(
				frameList.front());
        
		std::fstream imgSave(cov.image_path, std::ios::out | 
				std::ios::binary);
		imgSave.write(frame->picture().data(), frame->picture().size());
		imgSave.close();

		std::cout << "saved to " << cov.image_path << "\n";

		return cov;
    }

    bool song_contains_coverart(const models::song &song)
    {
        TagLib::MPEG::File songFile(song.song_path.c_str());
		auto tag = songFile.ID3v2Tag();
		auto frameList = tag->frameListMap()["APIC"];

		return !frameList.isEmpty();
    }

    void update_metadata(models::song &sngUpdated, const models::song &sngOld)
    {
        std::cout << "updating metadata\n";
		TagLib::MPEG::File file(sngOld.song_path.c_str());
		auto tag = file.ID3v2Tag();
    
		if (sngUpdated.title.size() > 0) {
		    file.tag()->setTitle(sngUpdated.title);
		}
		if (sngUpdated.artist.size() > 0) {
		    file.tag()->setArtist(sngUpdated.artist);
		}
		if (sngUpdated.album_artist.size() > 0) {
		    constexpr auto frameId = "TPE2";
		    auto cgFrame = tag->frameList(frameId);
		    if (cgFrame.isEmpty()) {
		        TagLib::ID3v2::TextIdentificationFrame *frame =
		                new TagLib::ID3v2::TextIdentificationFrame(frameId);
		        frame->setText(sngUpdated.album_artist.c_str());
		        tag->addFrame(frame);
		    } else {
		        cgFrame.front()->setText(sngUpdated.album_artist.c_str());
		    }

		    file.save();
		}
		if (sngUpdated.album.size() > 0) {
		    file.tag()->setAlbum(sngUpdated.album);
		}
		if (sngUpdated.genre.size() > 0) {
		    file.tag()->setGenre(sngUpdated.genre);
		}
		if (sngUpdated.year > 0) {
		    file.tag()->setYear(sngUpdated.year);
		}

        // TODO: functionality to update the track number and disc number

		file.save();
    }
private:
};

}

#endif