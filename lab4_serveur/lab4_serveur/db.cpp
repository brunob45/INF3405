#include <fstream>
#include "db.h"

using namespace std;

db::db()
{
}

db::~db()
{
}

bool db::open(string file)
{
	fstream stream;
	stream.open(file, fstream::in | fstream::out);

	if (!stream.is_open())
	{
		return false;
	}

	filename = file;

	while (stream.peek() != EOF)
	{
		char buf[200];
		stream.getline(buf, 200, '\n');
		string s = buf;
		int index = s.find(':');
		add(s.substr(0, index), s.substr(index + 1, s.length() - index - 1));
	}
	stream.close();
	return true;
}

string db::fetch(string key)
{
	if (contains(key))
	{
		return data[key];
	}
	return "";
}

bool db::contains(string key)
{
	return data.find(key) != data.end();
}

bool db::save()
{
	fstream stream;
	stream.open(filename, fstream::in | fstream::out);
	if (!stream.is_open())
		return false;

	stream.clear();
	map<string, string>::iterator it = data.begin();
	while (it != data.end())
	{
		string s = it->first + ":" + it->second;
		stream << s << endl;
	}
	stream.close();
}

bool db::add(string key, string value, bool do_save)
{
	data.insert(pair<string, string>(key, value));
	if (do_save)
	{
		return save();
	}
	return true;
}