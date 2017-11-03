
#include "bd.h"
#include <string>
#include <fstream>

using namespace std;

BD::BD(string filename)
	: m_filename(filename)
	, m_file(filename)
{
}

BD::~BD()
{
	if (m_file.is_open())
	{
		m_file.close();
	}
}

bool BD::is_open()
{
	return m_file.is_open();
}

bool BDUser::add(string username, string password)
{
	if (!this->is_open())
	{
		return false;
	}

	m_file.write(username.c_str(), username.size());
	m_file.write(" ", 1);
	m_file.write(password.c_str(), password.size());
	m_file.write("\n", 1);

	return true;
}

bool BDUser::contains(string username, string password)
{
	if (!this->is_open())
	{
		return false;
	}

	m_file.clear();
	m_file.seekg(0);
	while (!m_file.eof())
	{
		string name, pass;
		//m_file.read >> name >> pass;
		if (name == username)
		{
			return pass == password;
		}
	}

	this->add(username, password);
	return true;
}

BDMessage::BDMessage(string filename)
	:BD(filename)
{
	if (!this->is_open())
	{
		return;
	}

	while (!m_file.eof())
	{
		string m;
		m_file >> m;
		m_messages.push(m);
	}
}

bool BDMessage::add(string message)
{
	m_messages.pop();
	m_messages.push(message);

	return true;
}