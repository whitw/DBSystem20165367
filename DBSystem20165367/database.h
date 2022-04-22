#pragma once
#include <string>
#include "table.h"
using std::string;

namespace DBConst {
	const int BLOCK_SIZE = 4096;
	const string EXTENSION = ".db";
};

class Database {
	string name="";
	vector<string> tableList;	
	bool done_init = false;
	vector<string> split(string s, char delimeter);
		//string.split(delimeter)
public:
	Database() : name(""), tableList(vector<string>()), done_init(false) {};
	bool create(string name);
		//Create name.db file, if it does not exist.
		//return false if the file already exists,
		//throw error if(any) error occurs while opening file.
	bool open(string name);
		//open name.db file and read contents of the file.
		//file stream will be closed right after reading all contents of the file
		//and the contents will stay on main memory
		//until before the instance gets removed.
	bool read();
		//same as open(string name), but without using 
	vector<string> getTableList() { return tableList; };
		//returns the list of the table.
		//It does NOT check if the file had been updated or not.
		//Update the table list by void read() or void open(string filename) method.
	bool contains(string tablename);
		//check if there is a Table named tablename.
		//It does NOT check if the file had been updated after you read the file.
	bool insert(Table T);
		//insert Table T onto the database.
		//open file, insert single item, and close the file.
	bool insert(vector<Table> T);
		//insert Tables onto the database.
		//open file, insert multiple items, and close the file.
	bool remove(string tableName);
		//remove table from the database, and remove table too.
		//it DO remove the table itself and it CANNOT be undone.
	bool removeDB();
		//remove database file and all of the tables that 
};