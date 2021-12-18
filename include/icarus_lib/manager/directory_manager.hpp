#ifndef DIRECTORY_MANAGER_H_
#define DIRECTORY_MANAGER_H_

#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <filesystem>

#include "models/models.hpp"
#include <nlohmann/json.hpp>

#include "types/types.hpp"

namespace icarus_lib::manager {

class directory_manager {
public:
    directory_manager() = default;

    static std::string createDirectoryProcess(const models::song &song, const std::string &rootPath)
    {
        auto currPath = std::filesystem::path(rootPath);

        if (std::filesystem::exists(currPath)) {
            std::cout << "path exists\n";
        } else {
            std::cout << "creating path\n";
            std::filesystem::create_directory(currPath);
        }

        auto artPath = std::filesystem::path(currPath.string() + song.artist);
        if (std::filesystem::exists(artPath)) {
            std::cout << "artist path exists\n";
        } else {
            std::cout << "creating artist path\n";
            std::filesystem::create_directory(artPath);
        }

        auto albPath = std::filesystem::path(artPath.string() + "/" + song.album_artist);
        if (std::filesystem::exists(albPath)) {
            std::cout << "album path exists\n";
        } else {
            std::cout << "creating album path\n";
            std::filesystem::create_directory(albPath);
        }

        auto discPath = DirectoryManager().relativeDiscSongPathFilesystem(albPath, song);

        if (std::filesystem::exists(discPath)) {
            std::cout << "disc path exists\n";
        } else {
            std::cout << "creating disc path\n";
            std::filesystem::create_directory(discPath);
        }

        return discPath.string() + "/";
    }

    static std::string createDirectoryProcess(const models::song &song, 
            const models::binary_path &bConf, types::path_type pType)
    {
        auto path = pathConfigContent(bConf)[retrievePathType(pType)];
        auto rootPath = path.get<std::string>();
        auto currPath = std::filesystem::path(rootPath);

        if (std::filesystem::exists(currPath)) {
            std::cout << "path exists\n";
        } else {
            std::cout << "creating root music path\n";
            std::filesystem::create_directory(currPath);
        }

        auto artPath = std::filesystem::path(currPath.string() + song.album_artist);
        if (std::filesystem::exists(artPath)) {
            std::cout << "artist path exists\n";
        } else {
            std::cout << "creating artist path\n";
            std::filesystem::create_directory(artPath);
        }

        auto albPath = std::filesystem::path(artPath.string() + "/" + song.album);
        if (std::filesystem::exists(albPath)) { 
            std::cout << "album path exists\n";
        } else {
            std::cout << "creating album path\n";
            std::filesystem::create_directory(albPath);
        }

        auto discPath = DirectoryManager().relativeDiscSongPathFilesystem(albPath, song);
        if (std::filesystem::exists(discPath)) {
            std::cout << "disc path exists\n";
        } else {
            std::cout << "creating disc path\n";
            std::filesystem::create_directory(discPath);
        }

        return discPath.string() + "/";
    }

    static std::string configPath(std::string_view path)
    {
        return std::filesystem::canonical(path).parent_path().string();
    }

    static std::string configPath(const models::binary_path &bConf)
    {
        return std::filesystem::canonical(bConf.path).parent_path().string();
    }

    static std::string contentOfPath(const std::string &path)
    {
        std::std::filesystemtream a(path, std::ios::in);
        std::stringstream s;
        s << a.rdbuf();
        a.close();

        return s.str();
    }

    static std::string retrievePathType(types::path_type pType)
    {
        std::string path;
        switch (pType) {
            case type::PathType::music:
                path = "root_music_path";
                break;
            case type::PathType::archive:
                path = "archive_root_path";
                break;
            case type::PathType::temp:
                path = "temp_root_path";
                break;
            case type::PathType::coverArt:
                path = "cover_root_path";
                break;
            default:
                break;
        }

        return path;
    }

    static nlohmann::json credentialConfigContent(const models::binary_path &bConf)
    {
        auto path = configPath(bConf);
        path.append("/authcredentials.json");

        return nlohmann::json::parse(contentOfPath(path));
    }

    static nlohmann::json databaseConfigContent(const models::binary_path &bConf)
    {
        auto path = configPath(bConf);
        path.append("/database.json");

        return nlohmann::json::parse(contentOfPath(path));
    }

    template<typename json = nlohmann::json,
                typename config_t = models::binary_path>
    static json keyConfigContent(const config_t &config)
    {
        auto path = configPath(config);
        path.append("/");
        path.append(icarusKeyConfigName());

        return json::parse(contentOfPath(path));
    }

    static nlohmann::json pathConfigContent(const models::binary_path &bConf)
    {
        auto path = configPath(bConf);
        path.append("/paths.json");

        return nlohmann::json::parse(contentOfPath(path));
    }

    static constexpr auto icarusKeyConfigName() noexcept
    {
        return "icarus_keys.json";
    }

    static void deleteDirectories(models::song song, const std::string &rootPath)
    {
        std::cout << "checking for empty directories to delete\n";
        const std::string art(rootPath + std::string("/") + song.album_artist);
        const std::string alb(art + "/" + song.album);
        const std::string disc(alb + "/" + std::to_string(song.disc) + "/");

        auto discPath = std::filesystem::path(disc);

        if (std::filesystem::exists(discPath)) {
            std::cout << "disc directory does not exist\n";
        } else if (std::filesystem::is_empty(discPath)) {
            std::filesystem::remove(discPath);
        }

        auto albPath = std::filesystem::path(alb);
    
        if (!std::filesystem::exists(albPath)) {
            std::cout << "directory does not exists\n";
        } else if (std::filesystem::is_empty(albPath)) {
            std::filesystem::remove(albPath);
        }

        auto artPath = std::filesystem::path(art);
    
        if (!std::filesystem::exists(artPath)) {
            std::cout << "directory does not exists\n";
            return;
        } else if (std::filesystem::is_empty(artPath)) {
            std::filesystem::remove(artPath);
        }

        std::cout << "deleted empty directory or directories\n";
    }

    void deleteCoverArtFile(const std::string &covPath, const std::string &stockCoverPath)
    {
        if (covPath.compare(stockCoverPath) == 0) {
            std::cout << "cover has stock cover art, will not deleted\n";
        } else {
            std::cout << "deleting song path\n";
            auto cov = std::filesystem::path(covPath);
            std::filesystem::remove(cov);
        }
    }

private:
    std::filesystem::path relativeDiscSongPathFilesystem(const std::filesystem::path &albPath, 
            const models::song &song)
    {
        std::string albPathStr(albPath.string() + "/disc");
        if (song.disc >= 10) {
            albPathStr.append(std::to_string(song.disc));
        } else {
            albPathStr.append("0");
            albPathStr.append(std::to_string(song.disc));
        }

        albPathStr.append("/");
        auto relPath = std::filesystem::path(albPathStr.c_str());
        
        return relPath;
    }

    std::string relativeDiscSongPath(const std::filesystem::path &albPath, const models::song &song)
    {
        std::string albPathStr(albPath.string() + "/disc");
        if (song.disc >= 10) {
            albPathStr.append(std::to_string(song.disc));
        } else {
            albPathStr.append("0");
            albPathStr.append(std::to_string(song.disc));
        }

        albPathStr.append("/");
        return albPathStr;
    }

    void deleteSong(const models::song song)
    {
        std::cout << "deleting song\n";
        auto songPath = std::filesystem::path(song.song_path);

        if (!std::filesystem::exists(songPath)) {
            std::cout << "song does not exists\n";
            return;
        }

        std::filesystem::remove(songPath);
        std::cout << "deleted song" << "\n";
    }
};

}

#endif