#include <iostream>
#include "Registration.h"

AuthManager::AuthManager(const std::string& adminUsername, const std::string& adminPassword)
	: admin_username{ adminUsername }, admin_password{ adminPassword } {}

AuthManager::AuthManager() : admin_username{ "Admin" }, admin_password{ "123" } {}

bool AuthManager::setupRegistration() {
	int decision;
	
	while (!userLoggedIn) {
		system("cls");

		std::cout << "**** Payroll System Registration ****\n";
		std::cout << "1 = Login\n";
		std::cout << "2 = Exit\n";
		std::cout << "Input: ";

		if (!(std::cin >> decision)) {
			std::cout << "\nInvalid Input!\n";

			Utils::ClearInputBuffer();
			Utils::Pause();

			continue;
		}
		
		Utils::ClearInputBuffer();

		if (decision == 1) {
			authenticateUser();
		}
		else if (decision == 2) {
			std::cout << "\nExiting Program\n";
			Utils::Pause();

			return false;
		}
		else {
			std::cout << "\nInvalid Input: Number must be between (1 - 2)\n";
			Utils::Pause();
		}
	}

	return true;
}

bool AuthManager::authenticateUser() {
	system("cls");
	std::string username, password;

	std::cout << "Username: ";
	std::getline(std::cin >> std::ws, username);

	std::cout << "Password: ";
	std::getline(std::cin >> std::ws, password);

	userLoggedIn = (username == admin_username && password == admin_password);
	std::cout << '\n' << (userLoggedIn ? "User Authenticated" : "Invalid Credentials") << '\n';

	Utils::Pause();
	return userLoggedIn;
}

bool AuthManager::getLoggedInStatus() { return userLoggedIn; }
void AuthManager::logout() { userLoggedIn = false; }
