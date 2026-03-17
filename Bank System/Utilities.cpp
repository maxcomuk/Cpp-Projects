#include "Utilities.h"

void Utils::ClearInputBuffer() {
	if (std::cin.fail()) {
		std::cin.clear();
	}

	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void Utils::Pause() {
	std::cout << "Press Enter To Continue . . .";
	std::cin.get();
}

void Utils::ClearConsoleLinesFrom(int starting_line) {
	std::cout << "\033[" << starting_line << "H"; // Moves Cursor To Starting Line
	std::cout << "\033[J"; // Clears Everything Below Starting Line
	std::cout << "\033[" << (starting_line + 1) << "H"; // Moves Cursor Back To Starting Line
}

bool dbUtils::createTable(sqlite3* db, const std::string& tableName, const std::string& columns) {
	const std::string SQL = "CREATE TABLE IF NOT EXISTS " + tableName + " (" + columns + ");";
	
	int response = sqlite3_exec(db, SQL.c_str(), nullptr, nullptr, nullptr);
	if (response != SQLITE_OK) {
		std::cerr << "\nError Creating Table: " << tableName << "\nSQL Error Message: " << sqlite3_errmsg(db) << '\n';

		return false;
	}

	return true;
}
