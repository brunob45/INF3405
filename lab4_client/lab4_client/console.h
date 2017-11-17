#pragma once

#include <string>
#include <vector>

class console_t
{
private:
	std::vector<std::string> buf;
	bool buf_dirty;
	int x, y;
public:
	console_t();
	~console_t();
	void write(const std::string& s);
	void writeline(const std::string& s);
	std::string read();
	void clear();
};

extern console_t console;