# Inventory-Management-System Documentation

![License](https://github.com/maxcomuk/Cpp-Projects/blob/main/LICENSE) <br></br>
[Direct Exe Download](https://limewire.com/d/9aydG#i1zSbooxiD)

## Project Overview
1. Inventory Management System allows the (user / admin) control the Inventory of a buisness or warehouse etc. Our goal is to use oop and create a class with the required methods to control the inventory and get data aswell as a seperate class that will handle the registration for the user to login into the inventory management system. As we are focusing primarly on the inventory class we will only include simple methods for the registration such as registering an account or logging into an existing account.

2. We will be using sqlite3 to create the database file meaning for the project to work you MUST add sqlite3.c and sqlite3.h to the 'db' folder for it to work and provide the functionalities required for the database. You can do this by downloading the sqlite3.c and .h from this github or from the offical SQL website and then right-click on the files and select the option 'include in project' once this is done on each file they will be added to the project and allow the database to use the sqlite3 methods.

### Key Features
- Inventory Registration
- Inventory Management
- Logout / Exit program compatibility

### Things to know
As this is my first time building an inventory management system and using multiple files, I will be including extra code such as declaring the constructor or deconstructor but it may not be used so you can delete these parts as it was intended for users that may build on this project. Its also recommended to rename the files to your likings such as 'db' can be renamed to database.

## Step 1: Database.h

```
#pragma once

#include <string>
#include <vector>
#include "sqlite3.h"

struct InventoryItem
{
	std::string category;
	std::string productName;
	int amount;
	double price;
	std::string size;
	std::string description;
	std::string createdAt;
};

class Database
{
private:
	sqlite3* db;
public:
	Database();
	~Database();

	bool OpenDatabase(const std::string& fileName);
	bool CreateTable(const std::string& tableName, const std::string& columns);
	bool ValidateUser(const std::string& username, const std::string& password);
	bool InsertUser(const std::string& username, const std::string& password);
	bool RemoveUser(const std::string& username, const std::string& password);

	bool ValidateItem(InventoryItem& object);
	bool ModifyItem(const std::string& productName, const InventoryItem& object);
	bool InsertItem(const InventoryItem& object);
	bool RemoveItem(const std::string& productName);

	bool GetItem(InventoryItem& object);
	std::vector<InventoryItem> GetAllItems();
};
```
