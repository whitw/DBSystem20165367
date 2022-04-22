#pragma once
#include <vector>
#include "coltype.h"
#include "table.h"

class TableBuilder {
	int numCols;
	vector<ColType> c;
	
public:
	Table create();
};