#pragma once

#include <iostream>
#include <limits>
#include "database/Database.h"

namespace Utils {
	void ClearInputBuffer();
	void Pause();
	void ClearConsoleLinesFrom(int starting_line);
}

namespace dbUtils {
	bool createTable(sqlite3* db, const std::string& tableName, const std::string& columns);
}
