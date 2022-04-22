#include <iostream>
#include "database.h"
using namespace std;

int main() {
	Database db;
	if (db.create("first") == false) {
		cout << "cannot create file first.db";
	}
	db.open("first");
	db.read();
	vector<string> vs = db.getTableList();
	for (string i : vs) {
		cout << '[' << i << ']';
	}
	return 0;
}