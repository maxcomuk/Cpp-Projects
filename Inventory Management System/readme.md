# Inventory-Management-System Documentation

## Project Overview
1. Inventory Management System allows the (user / admin) control the Inventory of a business or warehouse etc. Our goal is to use OOP and create a class with the required methods to control the inventory and get data as well as a separate class that will handle the registration for the user to login into the inventory management system. As we are focusing primarily on the inventory class we will only include simple methods for the registration such as registering an account or logging into an existing account.

2. We will be using sqlite3 to create the database file meaning for the project to work you MUST add `sqlite3.c` and `sqlite3.h` to the `db` folder for it to work and provide the functionalities required for the database. You can do this by downloading the sqlite3.c and .h from the [official SQLite website](https://www.sqlite.org/download.html) and then right-click on the files and select the option **'Include In Project'** — once this is done on each file they will be added to the project and allow the database to use the sqlite3 methods.

### Key Features
- Inventory Registration
- Inventory Management
- Logout / Exit program compatibility

### Things to know
As this is my first time building an inventory management system and using multiple files, I will be including extra code such as declaring the constructor or destructor but it may not be used so you can delete these parts as it was intended for users that may build on this project. It's also recommended to rename the files to your liking such as `db` can be renamed to `database`.

---

## Step 1: Database.h
The `Database` class will contain all the methods that will interact with the database and perform actions such as changing or getting data. The `InventoryItem` struct will be used as a template when we need to modify an item — by using this struct we can pass the desired inventory item data and find it within the database and then change it to the new data. We use this struct to pass data through a function; the benefit of doing this is that we prevent 7+ arguments to a function parameter and just include 1 argument which is the struct itself.

```cpp
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

---

## Step 2: Database.cpp

### 1. Constructor & Destructor
The constructor initializes the `db` pointer to `nullptr`, indicating that no database connection is active.

The destructor ensures proper resource management. If the database was opened (`db` is not `nullptr`), it safely closes the connection using `sqlite3_close(db)` and resets the pointer to `nullptr`. This guarantees that the database connection is properly released when the `Database` object goes out of scope, preventing resource leaks.

---

### 2. OpenDatabase

The `OpenDatabase` method takes a `fileName` parameter and attempts to open (or create, if it does not exist) a SQLite database file using `sqlite3_open()`.

If the operation fails, it prints the error message, closes any partially opened connection, resets the pointer, and returns `false`.

If successful, the `db` pointer now represents a valid database connection and the method returns `true`.

---

### 3. CreateTable

The `CreateTable` method dynamically builds a SQL query using the provided `tableName` and `columns` parameters. It executes a `CREATE TABLE IF NOT EXISTS` statement to ensure the table is created only if it does not already exist.

In this project, it is used to create:

- **USERS** – stores registered users for authentication.
- **INVENTORY** – stores all inventory-related data for the management system.

The method returns `true` if the table is successfully created, otherwise `false`.

---

### 4. ValidateUser

This method verifies whether a user exists in the `USERS` table by matching the provided `username` and `password`.

It uses a prepared statement with parameter binding to prevent SQL injection. The query counts matching rows.

- If the count is greater than 0 → returns `true`.
- Otherwise → returns `false`.

This method is used during the login process.

---

### 5. InsertUser

The `InsertUser` method inserts a new user record into the `USERS` table using parameter binding.

- Returns `true` if insertion is successful (`SQLITE_DONE`).
- Returns `false` if the operation fails (e.g., duplicate user constraint or other database error).

This method supports user registration.

---

### 6. RemoveUser

This method removes a user from the `USERS` table by matching both `username` and `password`.

After execution, it checks `sqlite3_changes(db)` to confirm that a row was actually deleted.

- Returns `true` if a user was removed.
- Returns `false` if no matching user exists or an error occurs.

---

### 7. ValidateItem

The `ValidateItem` method checks whether an inventory item exists in the `INVENTORY` table using the `productName` field from the `InventoryItem` struct.

If a matching row is found:

- The struct is populated with all retrieved database values.
- Default fallback values are assigned if any column is `NULL`.
- Returns `true`.

If no record is found, it returns `false`.

---

### 8. ModifyItem

This method updates an existing inventory item.

It locates the item using `originalProductName` and updates all relevant fields using the new values provided in the `InventoryItem` struct. The `CREATED_AT` column is updated automatically using `CURRENT_TIMESTAMP`.

After execution:

- If at least one row was modified → returns `true`.
- If no matching product exists or an error occurs → returns `false`.

---

### 9. InsertItem

The `InsertItem` method inserts a new record into the `INVENTORY` table using the data stored in the `InventoryItem` struct.

- Returns `true` if the insertion succeeds.
- Returns `false` if the product already exists (constraint failure) or another database error occurs.

This method is used to add new products to the inventory system.

---

### 10. RemoveItem

This method deletes an item from the `INVENTORY` table using the provided `productName`.

After execution, it verifies that a row was deleted by checking `sqlite3_changes(db)`.

- Returns `true` if the product was successfully removed.
- Returns `false` if the product does not exist or if a database error occurs.

---

### 11. GetItem

The `GetItem` method retrieves a specific inventory item by matching both `category` and `productName` from the provided `InventoryItem` struct.

If a matching row is found:

- The remaining fields (`amount`, `price`, `size`, `description`, `createdAt`) are populated from the database.
- Returns `true`.

If no match is found, it prints a message and returns `false`.

This method is useful when searching for a specific product within a known category.

---

### 12. GetAllItems

The `GetAllItems` method retrieves all records from the `INVENTORY` table.

#### Process overview:

- A `SELECT` query is prepared to fetch every column from the table.
- The method iterates through each row using `sqlite3_step()` inside a `while` loop.

For each row:

- A new `InventoryItem` object is created.
- All database fields are extracted and assigned to the object.
- Default fallback values are used if any field is `NULL`.
- The fully populated object is pushed into the `storedItems` vector.

After all rows are processed:

- The prepared statement is finalized.
- The method returns a `std::vector<InventoryItem>` containing every item in the inventory database.

This method is essential for displaying or processing the entire inventory list in the application.

---

## Step 3: Registration.h & Registration.cpp

The `AuthManager` class handles all user-facing registration and login functionality. It maintains a simple login state and exposes a setup method that drives the authentication flow before the user can access the inventory system.

```cpp
#pragma once
#include "db/Database.h"

class AuthManager
{
private:
    bool loggedIn;
    std::string activeUser;
public:
    AuthManager();
    ~AuthManager();

    bool get_login_status() const;
    const std::string& get_active_user() const;
    bool setup_user_registration(Database& db);
};
```

### 1. Constructor & Destructor

The constructor initializes `loggedIn` to `false` and `activeUser` to an empty string, reflecting that no user is authenticated at startup. The destructor performs no special cleanup as the class holds no heap-allocated resources.

---

### 2. get_login_status

Returns the current value of `loggedIn`. Used in `Main.cpp` to decide whether to prompt the registration/login menu before entering the inventory loop.

---

### 3. get_active_user

Returns the username of the currently logged-in user. This can be used by other parts of the system to display or log the active session.

---

### 4. setup_user_registration

This method drives the registration/login menu loop. It presents the user with options to:

- **Register** a new account — calls `db.InsertUser()`
- **Login** to an existing account — calls `db.ValidateUser()`
- **Exit** the program

On successful login, `loggedIn` is set to `true` and `activeUser` is stored. Returns `false` if the user chooses to exit, which signals `Main.cpp` to break the main loop.

---

## Step 4: InvManager.h & InvManager.cpp

The `InventoryManager` class handles all inventory operations exposed to the user. It uses the `Database` class for persistence and the `invUtils` namespace from `Utilities` for validated user input.

```cpp
#pragma once
#include "db/Database.h"
#include "registration/Registration.h"

class InventoryManager
{
public:
    InventoryManager();
    ~InventoryManager();

    bool setup_inventory_manager(Database& db, AuthManager& auth);
};
```

### 1. setup_inventory_manager

This is the main inventory loop. It displays the inventory menu and routes the user to the appropriate action based on their input:

- **Add Item** — collects item details via `invUtils` helpers and calls `db.InsertItem()`
- **Remove Item** — prompts for a product name and calls `db.RemoveItem()`
- **Modify Item** — validates the existing item with `db.ValidateItem()`, collects new values, then calls `db.ModifyItem()`
- **View Item** — prompts for category and product name, then calls `db.GetItem()`
- **View All Items** — calls `db.GetAllItems()` and displays the full inventory list
- **Logout** — resets the auth state and returns to the registration menu
- **Exit** — returns `false` to signal `Main.cpp` to terminate the program

---

## Step 5: Utilities.h & Utilities.cpp

Utilities is split into two namespaces to keep database setup and input handling separate.

```cpp
#pragma once
#include "db/Database.h"

namespace dbUtils
{
    void ClearInputBuffer();
    bool SetupTables(Database& db);
    void registrationMenu();
    void inventoryMenu();
}

namespace invUtils
{
    bool get_product_category(std::string& itemCategory);
    void get_product_name(std::string& itemName);
    bool get_product_amount(int& itemAmount);
    bool get_product_price(double& itemPrice);
    void get_product_size(std::string& itemSize);
    void get_product_description(std::string& itemDesc);
}
```

### dbUtils

| Function | Description |
|---|---|
| `ClearInputBuffer()` | Flushes `std::cin` to prevent leftover input causing issues on the next read |
| `SetupTables(db)` | Creates the `USERS` and `INVENTORY` tables on first run via `db.CreateTable()` |
| `registrationMenu()` | Prints the registration/login options menu to the console |
| `inventoryMenu()` | Prints the inventory options menu to the console |

### invUtils

Each function collects and validates a single field of an `InventoryItem` from the user. Boolean-returning functions return `false` if the user enters a sentinel value to cancel the operation.

| Function | Description |
|---|---|
| `get_product_category()` | Prompts and validates the item category |
| `get_product_name()` | Prompts for the product name |
| `get_product_amount()` | Prompts and validates the stock amount (integer) |
| `get_product_price()` | Prompts and validates the price (double) |
| `get_product_size()` | Prompts for the item size |
| `get_product_description()` | Prompts for the item description |

---

## Step 6: Main.cpp

`Main.cpp` is the entry point. It initialises the three core objects, opens the database, sets up the tables, then enters the main loop.

```cpp
#include <iostream>
#include <limits>
#include "Utilities.h"
#include "db/Database.h"
#include "registration/Registration.h"
#include "inventory/InvManager.h"

int main()
{
    AuthManager auth;
    Database db;
    InventoryManager inv;

    if (!(db.OpenDatabase("Inventory Management System.db")))
        return 1;

    if (!(dbUtils::SetupTables(db)))
        return 1;

    std::cout << "**** Inventory System ****\n";

    while (true)
    {
        if (!(auth.get_login_status()))
            if (!(auth.setup_user_registration(db)))
                break;

        if (!(inv.setup_inventory_manager(db, auth)))
            break;
    }

    return 0;
}
```

The loop works as follows:

- If the user is **not logged in**, `setup_user_registration()` is called. Returning `false` means the user chose to exit — the loop breaks.
- If the user **is logged in**, `setup_inventory_manager()` is called. Returning `false` means the user chose to exit — the loop breaks.
- Logging out from the inventory menu resets the login state without breaking the loop, returning the user to the registration screen.

---

## Installation Guide

**1. Clone the repository**
```bash
git clone https://github.com/Alexandru101/Cpp-Projects.git
cd "Cpp-Projects/Inventory Management System"
```

**2. Get SQLite3 source files**

Download `sqlite3.c` and `sqlite3.h` from [sqlite.org/download](https://www.sqlite.org/download.html) (look for the amalgamation zip) and place both files inside the `db/` folder.

**3. Include SQLite3 in your project** *(Visual Studio)*

Right-click `sqlite3.c` and `sqlite3.h` in the Solution Explorer → **Include In Project** on each file.

**4. Build and run**

- **Visual Studio:** Press `Ctrl+F5`
- **g++ (command line):**
```bash
g++ -std=c++17 Main.cpp Utilities.cpp db/Database.cpp db/sqlite3.c registration/Registration.cpp inventory/InvManager.cpp -o InventorySystem
./InventorySystem
```

The database file `Inventory Management System.db` will be created automatically on first run.
