#include <iostream>
#include "Utilities.h"

namespace dbUtils
{
	void ClearInputBuffer()
	{
		if (std::cin.fail())
			std::cin.clear();

		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	bool SetupTables(Database& db)
	{
		if (!(db.CreateTable("USERS", "ID INTEGER PRIMARY KEY AUTOINCREMENT, USERNAME TEXT NOT NULL UNIQUE, PASSWORD TEXT NOT NULL")))
		{
			return false;
		}

		if (!(db.CreateTable("INVENTORY", "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
			"CATEGORY TEXT NOT NULL, "
			"PRODUCT_NAME TEXT NOT NULL UNIQUE, "
			"AMOUNT INTEGER NOT NULL DEFAULT 0, "
			"PRICE REAL NOT NULL DEFAULT 0.0, "
			"SIZE TEXT, "
			"DESCRIPTION TEXT, "
			"CREATED_AT DATETIME DEFAULT CURRENT_TIMESTAMP")))
		{
			return false;
		}

		return true;
	}

	void registrationMenu()
	{
		std::cout << "\n1 = Login\n";
		std::cout << "2 = Register\n";
		std::cout << "3 = Delete Account\n";
		std::cout << "4 = Exit\n";
		std::cout << "Input: ";
	}

	void inventoryMenu()
	{
		std::cout << "1 = Hot Meals\n";
		std::cout << "2 = Cold Meals\n";
		std::cout << "3 = Drinks\n";
		std::cout << "4 = Extra\n";
		std::cout << "5 = Display All Items\n";
		std::cout << "6 = Update Item Information\n";
		std::cout << "7 = Add Item To Inventory\n";
		std::cout << "8 = Remove Item From Inventory\n";
		std::cout << "9 = LogOut\n";
		std::cout << "10 = Exit\n\n";
		std::cout << "Input: ";
	}
}

namespace invUtils
{
	bool get_product_category(std::string& itemCategory)
	{
		int userInput;

		std::cout << "**** Category ****\n";
		std::cout << "1 = Hot Meal\n";
		std::cout << "2 = Cold Meal\n";
		std::cout << "3 = Drink\n";
		std::cout << "4 = Extra\n\n";
		std::cout << "Input: ";
		if (!(std::cin >> userInput))
		{
			std::cout << "\nInvalid Input\n";
			std::cout << "\nPress Enter To Continue . . .";
			
			dbUtils::ClearInputBuffer();
			std::cin.get();

			return false;
		}

		itemCategory = (userInput == 1) ? "Hot Meal" : (userInput == 2) ? "Cold Meal" : (userInput == 3) ? "Drink" : (userInput == 4) ? "Extra" : "Unkown";

		return true;
	}

	void get_product_name(std::string& itemName)
	{
		std::cout << "\nItem Name: ";
		std::getline(std::cin >> std::ws, itemName);
	}

	bool get_product_amount(int& itemAmount)
	{
		std::cout << "Item Amount: ";
		if (!(std::cin >> itemAmount))
		{
			std::cout << "\nInvalid Input\n";
			std::cout << "\nPress Enter To Continue . . .";

			dbUtils::ClearInputBuffer();
			std::cin.get();

			return false;
		}

		return true;
	}

	bool get_product_price(double& itemPrice)
	{
		std::cout << "Item Price: ";
		if (!(std::cin >> itemPrice))
		{
			std::cout << "\nInvalid Input\n";
			std::cout << "\nPress Enter To Continue . . .";

			dbUtils::ClearInputBuffer();
			std::cin.get();

			return false;
		}

		return true;
	}

	void get_product_size(std::string& itemSize)
	{
		std::cout << "Item Size (kg/grams/tonnes): ";
		std::getline(std::cin >> std::ws, itemSize);
	}

	void get_product_description(std::string& itemDesc)
	{
		std::cout << "Item Description (Tip: keep it simple): ";
		std::getline(std::cin >> std::ws, itemDesc);
	}
}
