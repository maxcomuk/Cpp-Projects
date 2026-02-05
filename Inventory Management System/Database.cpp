#include <iostream>
#include "Database.h"

Database::Database() : db(nullptr) {}

Database::~Database()
{
	if (db)
	{
		sqlite3_close(db);
		db = nullptr;
	}
}

bool Database::OpenDatabase(const std::string& fileName)
{
	int response = sqlite3_open(fileName.c_str(), &db);
	if (response != SQLITE_OK)
	{
		std::cerr << "\nFailed to open database: " << sqlite3_errmsg(db) << '\n';

		if (db)
		{
			sqlite3_close(db);
			db = nullptr;
		}

		return false;
	}

	return true;
}

bool Database::CreateTable(const std::string& tableName, const std::string& columns)
{
	std::string SQL = "CREATE TABLE IF NOT EXISTS " + tableName + " (" + columns + ");";

	int response = sqlite3_exec(db, SQL.c_str(), nullptr, nullptr, nullptr);
	if (response != SQLITE_OK)
		std::cerr << "\nFailed to create database table " << tableName << ": " << sqlite3_errmsg(db) << '\n';
	else
		return true;

	return false;
}

bool Database::ValidateUser(const std::string& username, const std::string& password)
{
	const char* SQL = "SELECT count(*) FROM USERS WHERE USERNAME = ? AND PASSWORD = ?";
	sqlite3_stmt* validate_stmt;
	if (sqlite3_prepare_v2(db, SQL, -1, &validate_stmt, nullptr) != SQLITE_OK)
	{
		std::cerr << "\nError Preparing sqlite3_prepare_v2: " << sqlite3_errmsg(db) << '\n';
		return false;
	}

	sqlite3_bind_text(validate_stmt, 1, username.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(validate_stmt, 2, password.c_str(), -1, SQLITE_STATIC);

	bool userValidated = false;
	if (sqlite3_step(validate_stmt) == SQLITE_ROW)
	{
		if (sqlite3_column_int(validate_stmt, 0) > 0)
		{
			userValidated = true;
		}
	}

	sqlite3_finalize(validate_stmt);
	return userValidated;
}

bool Database::InsertUser(const std::string& username, const std::string& password)
{
	const char* SQL = "INSERT INTO USERS (USERNAME, PASSWORD) VALUES (?, ?);";
	sqlite3_stmt* insert_stmt;
	if (sqlite3_prepare_v2(db, SQL, -1, &insert_stmt, nullptr) != SQLITE_OK)
	{
		std::cerr << "\nError Preparing sqlite3_prepare_v2: " << sqlite3_errmsg(db) << '\n';
		return false;
	}

	sqlite3_bind_text(insert_stmt, 1, username.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(insert_stmt, 2, password.c_str(), -1, SQLITE_STATIC);

	int result = sqlite3_step(insert_stmt);
	sqlite3_finalize(insert_stmt);

	if (result == SQLITE_DONE)
		return true;

	if (result == SQLITE_CONSTRAINT)
		std::cerr << "\nFailed to register user: Account arleady exists!\n";
	else
		std::cerr << "\nFailed to register user: " << sqlite3_errmsg(db) << '\n';

	return false;
}

bool Database::RemoveUser(const std::string& username, const std::string& password)
{
	const char* SQL = "DELETE FROM USERS WHERE USERNAME = ? AND PASSWORD = ?;";
	sqlite3_stmt* delete_stmt;
	if (sqlite3_prepare_v2(db, SQL, -1, &delete_stmt, nullptr) != SQLITE_OK)
	{
		std::cerr << "\nError Preparing sqlite3_prepare_v2: " << sqlite3_errmsg(db) << '\n';
		return false;
	}

	sqlite3_bind_text(delete_stmt, 1, username.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(delete_stmt, 2, password.c_str(), -1, SQLITE_STATIC);

	int result = sqlite3_step(delete_stmt);
	sqlite3_finalize(delete_stmt);

	if (result == SQLITE_DONE)
	{
		if (sqlite3_changes(db) > 0)
			return true;
			
		std::cerr << "\nUser Not Found: Please try again!\n";
		return false;
	}

	std::cerr << "\nDatabase Error During Removing User: " << sqlite3_errmsg(db) << '\n';
	return false;
}

bool Database::ValidateItem(InventoryItem& object)
{
	const char* SQL = "SELECT CATEGORY, PRODUCT_NAME, AMOUNT, PRICE, SIZE, DESCRIPTION, CREATED_AT FROM INVENTORY WHERE PRODUCT_NAME = ?;";
	sqlite3_stmt* validate_stmt;
	if (sqlite3_prepare_v2(db, SQL, -1, &validate_stmt, nullptr) != SQLITE_OK)
	{
		std::cerr << "\nError Preparing sqlite3_prepare_v2: " << sqlite3_errmsg(db) << '\n';
		return false;
	}

	sqlite3_bind_text(validate_stmt, 1, object.productName.c_str(), -1, SQLITE_STATIC);

	bool productValidated = false;
	if (sqlite3_step(validate_stmt) == SQLITE_ROW)
	{
		const char* cat = (const char*)sqlite3_column_text(validate_stmt, 0);
		const char* name = (const char*)sqlite3_column_text(validate_stmt, 1);
		const char* size = (const char*)sqlite3_column_text(validate_stmt, 4);
		const char* desc = (const char*)sqlite3_column_text(validate_stmt, 5);
		const char* date = (const char*)sqlite3_column_text(validate_stmt, 6);

		object.category = cat ? cat : "Unknown";
		object.productName = name ? name : "Unknown";
		object.amount = sqlite3_column_int(validate_stmt, 2);
		object.price = sqlite3_column_double(validate_stmt, 3);
		object.size = size ? size : "0kg";
		object.description = desc ? desc : "No Description";
		object.createdAt = date ? date : "N/A";
		
		productValidated = true;
	}

	sqlite3_finalize(validate_stmt);
	return productValidated;
}

bool Database::ModifyItem(const std::string& originalProductName, const InventoryItem& object)
{
	const char* SQL = "UPDATE INVENTORY SET CATEGORY = ?, PRODUCT_NAME = ?, AMOUNT = ?, PRICE = ?, SIZE = ?, DESCRIPTION = ?, CREATED_AT = CURRENT_TIMESTAMP WHERE PRODUCT_NAME = ?;";
	sqlite3_stmt* update_stmt;
	if (sqlite3_prepare_v2(db, SQL, -1, &update_stmt, nullptr) != SQLITE_OK)
	{
		std::cerr << "\nError Preparing sqlite3_prepare_v2: " << sqlite3_errmsg(db) << '\n';
		return false;
	}

	sqlite3_bind_text(update_stmt, 1, object.category.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(update_stmt, 2, object.productName.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_int(update_stmt, 3, object.amount);
	sqlite3_bind_double(update_stmt, 4, object.price);
	sqlite3_bind_text(update_stmt, 5, object.size.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(update_stmt, 6, object.description.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(update_stmt, 7, originalProductName.c_str(), -1, SQLITE_STATIC);

	int result = sqlite3_step(update_stmt);
	sqlite3_finalize(update_stmt);

	if (result == SQLITE_DONE)
	{
		if (sqlite3_changes(db) > 0)
			return true;

		std::cerr << "\nUpdate Failed: Product " << originalProductName << " Not Found!\n";
		return false;
	}

	std::cerr << "\nUpdate Failed: Database Error -> " << sqlite3_errmsg(db) << '\n';
	return false;
}

bool Database::InsertItem(const InventoryItem& object)
{
	const char* SQL = "INSERT INTO INVENTORY (CATEGORY, PRODUCT_NAME, AMOUNT, PRICE, SIZE, DESCRIPTION) VALUES (?, ?, ?, ?, ?, ?);";
	sqlite3_stmt* insert_stmt;
	if (sqlite3_prepare_v2(db, SQL, -1, &insert_stmt, nullptr) != SQLITE_OK)
	{
		std::cerr << "\nError Preparing sqlite3_preprare_v2: " << sqlite3_errmsg(db) << '\n';
		return false;
	}

	sqlite3_bind_text(insert_stmt, 1, object.category.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(insert_stmt, 2, object.productName.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_int(insert_stmt, 3, object.amount);
	sqlite3_bind_double(insert_stmt, 4, object.price);
	sqlite3_bind_text(insert_stmt, 5, object.size.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(insert_stmt, 6, object.description.c_str(), -1, SQLITE_STATIC);

	int result = sqlite3_step(insert_stmt);
	sqlite3_finalize(insert_stmt);

	if (result == SQLITE_DONE)
		return true;

	if (result == SQLITE_CONSTRAINT)
		std::cerr << "\nFailed to add product to inventory: Product arleady exists!\n";
	else
		std::cerr << "\nFailed to add product to inventory: " << sqlite3_errmsg(db) << '\n';

	return false;
}

bool Database::RemoveItem(const std::string& productName)
{
	const char* SQL = "DELETE FROM INVENTORY WHERE PRODUCT_NAME = ?;";
	sqlite3_stmt* delete_stmt;
	if (sqlite3_prepare_v2(db, SQL, -1, &delete_stmt, nullptr) != SQLITE_OK)
	{
		std::cerr << "\nError Preparing sqlite3_prepare_v2: " << sqlite3_errmsg(db) << '\n';
		return false;
	}

	sqlite3_bind_text(delete_stmt, 1, productName.c_str(), -1, SQLITE_STATIC);

	int result = sqlite3_step(delete_stmt);
	sqlite3_finalize(delete_stmt);

	if (result == SQLITE_DONE)
	{
		if (sqlite3_changes(db) > 0)
			return true;

		std::cerr << "\nFailed to remove product from the inventory. Please try again!\n";
		return false;
	}

	std::cerr << "\nDatabase Error During Removing Product: " << sqlite3_errmsg(db) << '\n';
	return false;
}

bool Database::GetItem(InventoryItem& object)
{
	const char* SQL = "SELECT CATEGORY, PRODUCT_NAME, AMOUNT, PRICE, SIZE, DESCRIPTION, CREATED_AT FROM INVENTORY WHERE CATEGORY = ? AND PRODUCT_NAME = ?;";
	sqlite3_stmt* stmt;
	if (sqlite3_prepare_v2(db, SQL, -1, &stmt, nullptr) != SQLITE_OK)
	{
		std::cerr << "\nError Preparing sqlite3_prepare_v2: " << sqlite3_errmsg(db) << '\n';
		return false;
	}

	sqlite3_bind_text(stmt, 1, object.category.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, object.productName.c_str(), -1, SQLITE_STATIC);

	int result = sqlite3_step(stmt);
	bool exists = false;
	
	if (result == SQLITE_ROW)
	{
		const char* size = (const char*)sqlite3_column_text(stmt, 4);
		const char* desc = (const char*)sqlite3_column_text(stmt, 5);
		const char* date = (const char*)sqlite3_column_text(stmt, 6);

		object.amount = sqlite3_column_int(stmt, 2);
		object.price = sqlite3_column_double(stmt, 3);
		object.size = size ? size : "0kg";
		object.description = desc ? desc : "No Description";
		object.createdAt = date ? date : "N/A";

		exists = true;
	}
	else
	{
		std::cout << "\nNo item found with name: " << object.productName << '\n';
	}

	sqlite3_finalize(stmt);
	return exists;
}

std::vector<InventoryItem> Database::GetAllItems()
{
	std::vector<InventoryItem> storedItems;

	const char* SQL = "SELECT CATEGORY, PRODUCT_NAME, AMOUNT, PRICE, SIZE, DESCRIPTION, CREATED_AT FROM INVENTORY;";
	sqlite3_stmt* stmt;
	if (sqlite3_prepare_v2(db, SQL, -1, &stmt, nullptr) == SQLITE_OK)
	{
		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			InventoryItem item;

			const char* cat = (const char*)sqlite3_column_text(stmt, 0);
			const char* name = (const char*)sqlite3_column_text(stmt, 1);
			const char* size = (const char*)sqlite3_column_text(stmt, 4);
			const char* desc = (const char*)sqlite3_column_text(stmt, 5);
			const char* date = (const char*)sqlite3_column_text(stmt, 6);

			item.category = cat ? cat : "Unknown";
			item.productName = name ? name : "Unknown";
			item.amount = sqlite3_column_int(stmt, 2);
			item.price = sqlite3_column_double(stmt, 3);
			item.size = size ? size : "0kg";
			item.description = desc ? desc : "No Description";
			item.createdAt = date ? date : "N/A";

			storedItems.push_back(item);
		}
	}
	else {
		std::cerr << "\nFailed to fetch items: " << sqlite3_errmsg(db) << '\n';
	}

	sqlite3_finalize(stmt);
	return storedItems;
}
