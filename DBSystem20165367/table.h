#pragma once
#include <string>
#include <vector>
#include <tuple>
#include "coltype.h"
using std::string;
using std::vector;
using std::tuple;

class Table {
	const string name;
	string fileDir;
	const vector<string> columnName;
	const vector<ColType> columnType;
	const vector<int> columnSize;
	const vector<int> PK;
	const vector<vector<int>> unique;
	const vector<tuple<int, string, int>> FK;
public:
	bool open(string) { return true; };
	string getName() { return name; };
	vector<string> getColumnNames();
	vector<int> getPK();
	tuple<string, ColType, int> getColumn(string);
	void insert(vector<string> row);
	void remove(vector<string> primary_key) {};
	bool deleteTable() { return true; };
};