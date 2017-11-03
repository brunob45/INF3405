#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <queue>

class BD {
protected :
	std::string m_filename;
	std::fstream m_file;


public :
	BD(std::string filename);
	~BD();
	bool is_open();
};

class BDUser : public BD
{
public:
	BDUser(std::string filename) :BD(filename) {}
	~BDUser() {}

	bool contains(std::string username, std::string password = "\0");

private:
	bool add(std::string username, std::string password);
};

class BDMessage : public BD
{
private:
	std::queue<std::string> m_messages;

public:
	BDMessage(std::string filename);
	~BDMessage() {}

	bool add(std::string message);
	std::vector<std::string> get(int nb = 15);
};
