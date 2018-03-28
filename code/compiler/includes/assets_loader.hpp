#pragma once
#include "assetsys.h"
#include <string>

class AssetsLoader
{
	assetsys_t *assets;
	int _load(const assetsys_file_t &file, void **buffer) const;
public:
	AssetsLoader();
	~AssetsLoader();
	void mount(const std::string &path) const;
	void unmount(const std::string &path) const;
	int load(const std::string &filePath, void **buffer) const;
	int load(const std::string &filePath, void **buffer, int index) const;
	bool exists(const std::string &filePath) const;
	int refCount(const std::string &filePath) const;
};
