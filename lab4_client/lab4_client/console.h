#pragma once

#include <stdio.h>
#include <string>
#include <iostream>

namespace console
{
	std::string readLine() {
		std::string s;
		std::cin >> s;
		return s;
	}
	char readChar() {
		return getchar();
	}
	void write(std::string s) {
		std::cout << s;
	}
	void writeLine(std::string s) {
		std::cout << s << std::endl;
	}
	std::string prompt(std::string s) {
		write(s);
		return readLine();
	}
}