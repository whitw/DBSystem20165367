#pragma once
#include <string>
using std::string;

class ColType {};

class Char : public ColType {
	string data;
	int len;
public:
	Char(int length);
	string get();
	size_t length();
	size_t size();
	void set(string);
};

class VarChar : public ColType {
	string data;
public:
	VarChar() {};
	string get();
	size_t length();
	void set(string);
};

//Types that can be a type of Table Column.