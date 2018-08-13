#define ASSETSYS_IMPLEMENTATION
#define STRPOOL_IMPLEMENTATION
#include "assetsys.h"
#include "assets_loader.hpp"

void replaceAll(std::string& str, const std::string& from, const std::string& to) {
	if (from.empty())
		return;
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
}

AssetsLoader::AssetsLoader()
{
	assets = assetsys_create(nullptr);
}

AssetsLoader::~AssetsLoader()
{
	assetsys_destroy(assets);
}

void AssetsLoader::mount(const std::string &path) const
{
	assetsys_mount(assets, path.c_str(), "/");
}

void AssetsLoader::unmount(const std::string &path) const
{
	assetsys_dismount(assets, path.c_str(), "/");
}

int AssetsLoader::load(const std::string &filePath, void **buffer) const
{
	assetsys_file_t file;
	std::string copy = "//" + filePath;
	replaceAll(copy, "\\", "/");
	auto error = assetsys_file(assets, copy.c_str(), &file);

	if (error == ASSETSYS_ERROR_FILE_NOT_FOUND)
	{
		*buffer = nullptr;
		return -1;
	}

	return _load(file, buffer);
}

int AssetsLoader::load(const std::string& filePath, void** buffer, int index) const
{
	assetsys_file_t file;
	auto error = assetsys_file_by_index(assets, ("//" + filePath).c_str(), &file, index);

	if (error == ASSETSYS_ERROR_FILE_NOT_FOUND)
	{
		*buffer = nullptr;
		return -1;
	}

	return _load(file, buffer);
}

int AssetsLoader::_load(const assetsys_file_t &file, void **buffer) const
{
	int fileSize = assetsys_file_size(assets, file);
	*buffer = new char[fileSize + 1];

	int readSize = 0;
	assetsys_file_load(assets, file, &readSize, *buffer, fileSize);
	static_cast<char*>(*buffer)[fileSize] = '\0';

	return readSize;
}

bool AssetsLoader::exists(const std::string& filePath) const
{
	assetsys_file_t file;
	auto error = assetsys_file(assets, ("//" + filePath).c_str(), &file);

	return error == ASSETSYS_SUCCESS;
}

int AssetsLoader::refCount(const std::string& filePath) const
{
	return assetsys_file_ref_count(assets, ("//" + filePath).c_str());
}
