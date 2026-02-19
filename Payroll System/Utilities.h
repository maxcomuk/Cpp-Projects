#pragma once

#include <limits>
#include <string>
#include "database/Database.h"

namespace Utils {
	void ClearInputBuffer();
	void Pause();
	void formatDate(std::string& date);
}

namespace dbUtils {
	bool CreateTable(sqlite3* db, const std::string& tableName, const std::string& columns);
}
