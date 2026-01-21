# Registration-System Documentation

![License](https://github.com/maxcomuk/Cpp-Projects/blob/main/LICENSE)

### Main Goal
Using the sqlite3 library, create a local database that will be used to store the all the registers users and allow users to log in to their accounts by verifiying if their account exists and that their password is valid. For this project we need to execute sql commands that will check the users data within the database and save data however as I am not an expert on sql commands and only now have learnt how to atleast make this project work now after finishing it, I will be using the commands from google and some help from AI.

## Step 1: Including Libraries
Import both sqlite3.h and sqlite3.c from --> [Download Link](https://sqlite.org/2026/sqlite-amalgamation-3510200.zip)

Once you have both libraries and your compiler doesnt error when you try to build your cpp file with the sqlite3.h included then it is working
```
#include <iostream>
#include <string>
#include <limits>
#include "sqlite3.h"
```

## Step 2: Clearing Input Stream
Clearing the input stream of any leftover whitespaces and any console errors
```
void clearInputStream()
{
	if (std::cin.fail())
		std::cin.clear();

	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
```

## Step 3: Prompting User With Registration Decision
Locking the user in a infinite while loop until the user has finished selecting an option.

We take a bool (named menu_navigation) to determain wether the user has just started the program or is logged in to his account. Then we prompt the user with the context-specific menu: if logged in, they can switch accounts or view details; otherwise, they are limited to initial login and registration. Depending on the users input we assign the decision then exit the loop.
```
void registrationType(int& decision, bool menu_navigation)
{
	while (true)
	{
		if (menu_navigation)
		{
			std::cout << "1 = Login Into Another Account || 2 = Register Another Account || 3 = Get Account Details\n";

			if (std::cin >> decision && (decision == 1 || decision == 2 || decision == 3))
				break;
		}
		else
		{
			std::cout << "\n1 = Login Account || 2 = Register An Account\n";

			if (std::cin >> decision && (decision == 1 || decision == 2))
				break;
		}

		std::cout << "\nInvalid Input\n";
		clearInputStream();
	}
}
```
