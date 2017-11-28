#pragma once

#include <string>
#include <map>
#include <fstream>

class db
{
private:
	std::string filename;
	std::map<std::string, std::string> data;
public:
	db();
	~db();
	bool open(std::string file);
	std::string fetch(std::string key);
	bool contains(std::string key);
	bool save();
	bool add(std::string key, std::string value, bool do_save = false);
};