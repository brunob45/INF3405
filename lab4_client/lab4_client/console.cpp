
#include <iostream>
#include "console.h";

using namespace std;

console_t console;

console_t::console_t()
	: x(0)
	, y(1)
{
	// DO NOTHING
}
console_t::~console_t()
{
	// DO NOTHING
}

void console_t::write(const std::string& s)
{
	std::cout << s;
}
void console_t::writeline(const std::string& s)
{
	std::cout << s << std::endl;
}
std::string console_t::read()
{
	std::string ret;
	std::cin >> ret;
	return ret;
}
void console_t::clear()
{
	system("cls");
}