#include <fstream>
#include "database.h"
using std::fstream;
using std::ifstream;
using std::ofstream;
using std::ios_base;

bool Database::create(string name) {
	fstream i;
	i.open(name + DBConst::EXTENSION, ios_base::in);
	if (!i.is_open()) {
		//No database named name found.
		//create file.
		i.open(name + DBConst::EXTENSION, ios_base::out);
		if (!i.is_open()) {
			return false;
		}
		i.close();
		this->name = name;
		return true;
	}
	else {
		i.close();
		return false;
	}
	this->done_init = true;
}

bool Database::open(string name) {
	this->name = name;
	this->done_init = true;
	return read();
}

vector<string> Database::split(string s, char delimeter) {
	size_t beginIdx = 0, endIdx = 0;
	vector<string> list;
	while ((endIdx = s.find(delimeter, beginIdx)) != string::npos) {
		list.push_back(s.substr(beginIdx, endIdx - beginIdx));
		beginIdx = endIdx + 1;
		//s[endIdx] == delimeter, need to check from endIdx+1 -th index
	}
	return list;
}

bool Database::read() {
	ifstream ifs;
	char block[DBConst::BLOCK_SIZE];
	this->tableList = vector<string>();
	if (!done_init) {
		throw "read() on unopened database";
		return false;
	}
	ifs.open(this->name + DBConst::EXTENSION, ios_base::in);
	if (!ifs.is_open()) {
		return create(name);
	}
	while (true) {
		ifs.read(block, DBConst::BLOCK_SIZE);
		if (ifs.eof()) break;
		string read;
		if (ifs.gcount() + read.length() == DBConst::BLOCK_SIZE) {
			//db file is bigger than DBConst::BLOCK_SIZE bytes.
			//I really don't think that it will happen but...
			read = block;
			size_t lastIdx = read.find_last_of('\n');
			vector<string> s = split(read.substr(0, lastIdx), '\n');
			tableList.insert(tableList.end(), s.begin(), s.end());
			read = read.substr(lastIdx);
			for (size_t i = 0; i < read.length(); i++) {
				block[i] = read[i];
			}
			ifs.read(block + read.length(), DBConst::BLOCK_SIZE - read.length());
		}
	}
	vector<string> s = split(block, '\n');
	tableList.insert(tableList.end(), s.begin(), s.end());
	return true;
}

/*
bool old_read() {
	//this method runs fine if it is emplaced as
	//bool Database::read() method.
	//yet, it calls ifs.getline too much times and it is
	//truely inefficient even if the dbfile is not that big.
	//I will NOT use this method. It is only for the backup case.
	//why not github it? well, just because. Tiresomeness matter.
	while (true) {
		ifs.getline(block, DBConst::BLOCK_SIZE);
		if (ifs.eof()) break;
		stream = block;
		if (ifs.gcount() == DBConst::BLOCK_SIZE) {
			while (ifs.gcount() != DBConst::BLOCK_SIZE) {
				//db file is bigger than DBConst::BLOCK_SIZE bytes.
				//I really don't think that it will happen but...
				ifs.getline(block, DBConst::BLOCK_SIZE);
				stream += string(block);
			}
		}
		//getline done.
		//split string by space and make vector out of it.
		size_t begIdx = 0;
		size_t endIdx = 0;
		while ((endIdx = stream.find(' ', begIdx)) != string::npos) {
			tableList.push_back(stream.substr(begIdx, endIdx - begIdx));
			begIdx = endIdx;
		}
		tableList.push_back(stream.substr(begIdx));
	}
	ifs.close();
	return true;
}
*/

bool Database::contains(string tablename) {
	if (!done_init) {
		throw "contains(" + tablename + ") on unopened db";
	}
	for (string s : this->tableList) {
		if (s == tablename) {
			return true;
		}
	}
	return false;
}

bool Database::insert(Table T) {
	if (!done_init) {
		throw "insert(" + T.getName() + ") on unopened db";
	}
	ofstream fs;
	fs.open(this->name + DBConst::EXTENSION, ios_base::app);
	if (fs.is_open()) {
		fs.write((T.getName() + "\n").c_str(), T.getName().length() + 1);
	}
	else {
		return false;
	}
	fs.close();
	return true;
}

bool Database::insert(vector<Table> TT) {
	if (!done_init) {
		throw "insert(vector<Table>) on unopened db";
	}
	ofstream fs;
	fs.open(this->name + DBConst::EXTENSION, ios_base::app);
	if (fs.is_open()) {
		string s = "";
		for (Table T : TT) {
			s += T.getName() + "\n";
		}
		fs.write(s.c_str(), s.length());
	}
	else {
		return false;
	}
	fs.close();
	return true;
}

bool Database::remove(string tableName) {
	if (read() == false) return false;
	for (size_t i = 0; i < tableList.size(); i++) {
		if (tableList[i] == tableName) {
			Table t;
			t.open(tableName);
			//TODO try-catch
			//table can refuse deleting file.(PK problem or something I guess.)
			t.deleteTable();
			tableList[i] = tableList[tableList.size() - 1];
			tableList.pop_back();
			break;
		}
	}
	ofstream fs;
	fs.open(this->name + DBConst::EXTENSION, ios_base::out);
	if (!fs.is_open()) {
		return false;
	}
	string s = "";
	for (string i : tableList) {
		s += i + "\n";
	}
	fs.write(s.c_str(), s.length());
	fs.close();
	return true;
}

bool Database::removeDB() {
	if (!done_init) {
		return false;
	}
	read();
	//TODO try-catch
	for (string table : tableList) {
		remove(table);
	}
	std::remove((name+DBConst::EXTENSION).c_str());
	done_init = false;
	name = "";
	tableList = vector<string>();
	return true;
}