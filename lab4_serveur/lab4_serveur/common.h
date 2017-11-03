#pragma once
#ifndef __COMMON_H__
#define __COMMON_H__

#include <queue>
#include <vector>
#include <string>
#include <winsock2.h>

struct message_t
{
	std::string corps;
	std::queue<SOCKET> dests;
};

struct user_t
{
	SOCKET sd;
	std::string username;
	std::string password;
};

extern std::queue<message_t> boite_denvoi;
extern std::vector<user_t> liste_connexions;
extern SOCKET ServerSocket;

#endif
