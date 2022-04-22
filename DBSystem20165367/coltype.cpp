#include "coltype.h"
#include <stdexcept>

Char::Char(int length) {
	if (length > 255) {
		throw std::domain_error("Length of the Type Char cannot be bigger than 255");
	}
	this->len = length;
}
string Char::get() {
	return data;
}

size_t Char::length() {
	return data.length();
}

size_t Char::size() {
	return this->len;
}

void Char::set(string s) {
	if (s.length() >= this->len - 1) {
		//-1 for trailling 0
		throw std::domain_error("Length of string is longer than the size of the type Char");
	}
	else data = s;
}

string VarChar::get() {
	return data;
}

size_t VarChar::length() {
	return data.length();
}
void VarChar::set(string s) {
	if (s.length() > 255 - 1) {
		throw std::domain_error("Length of the string is bigger than 255");
	}
	data = s;
}
