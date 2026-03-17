#include "Registration.h"

AuthManager::AuthManager(Database* database) : db{ database } {
	if (!db) {
		throw std::runtime_error("Error: Database Null or Invalid\n");
	}
}

AuthManager::AuthManager() : db{ nullptr } {
	throw std::runtime_error("Error: Database Not Initialized\n");
}

bool AuthManager::setup_registration() {
	int userInput;

	while (1) {
		system("cls");

		std::cout << "**** Account Registration ****\n\n";
		std::cout << "1 = Register Account\n";
		std::cout << "2 = Login Account\n";
		std::cout << "3 = Delete An Existing Account\n";
		std::cout << "4 = Exit Registration\n";
		std::cout << "Input: ";

		if (!(std::cin >> userInput)) {
			std::cout << "\nInvalid Input\n";

			Utils::ClearInputBuffer();
			Utils::Pause();

			continue;
		}

		Utils::ClearInputBuffer();

		switch (userInput) {
			case 1:
				register_account();

				break;
			case 2:
				if (authenticate_account())
					return true;

				break;
			case 3:
				delete_account();

				break;
			case 4:
				std::cout << "\nPreparing To Close Program";
				std::this_thread::sleep_for(std::chrono::milliseconds(300));
				std::cout << "\nExiting Program . . .";
				std::this_thread::sleep_for(std::chrono::milliseconds(150));

				return false;
			default:
				std::cout << "\nInvalid Input: Number Must Be Between (1 - 3)\n";
				Utils::Pause();
		}
	}

	return false;
}

void AuthManager::register_account() {
	system("cls");

	std::cout << "**** Registration System ****\n\n";
	std::cout << "Username: ";
	std::getline(std::cin, username_input);

	std::cout << "Password: ";
	std::getline(std::cin, password_input);

	bool account_registerd = db->insert_user(username_input, password_input);
	if (account_registerd) { std::cout << "\nSuccessfully Registerd User\n"; }
	
	std::cout << '\n';
	Utils::Pause();
}

bool AuthManager::authenticate_account() {
	system("cls");

	std::cout << "**** Registration System ****\n\n";
	std::cout << "Username: ";
	std::getline(std::cin, username_input);

	std::cout << "Password: ";
	std::getline(std::cin, password_input);

	bool credentials_validated = db->validate_user(username_input, password_input);

	std::cout << "\nValidating Credentials . . .";
	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	if (!credentials_validated) {
		std::cout << "\nInvalid Credentials\n\n";
		Utils::Pause();
	}
	else {
		std::cout << "\nCredentials Validated\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

	return credentials_validated;
}

void AuthManager::delete_account() {
	system("cls");

	std::cout << "**** Account Removal Registration System ****\n\n";
	std::cout << "Username: ";
	std::getline(std::cin, username_input);
	
	std::cout << "Password: ";
	std::getline(std::cin, password_input);

	std::cout << "\nValidating Account . . .";
	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	bool credentials_validated = db->validate_user(username_input, password_input);
	if (!credentials_validated) {
		std::cout << "\n\nCredentials Invalid: Account Not Found !\n\n";
		Utils::Pause();

		return;
	}

	std::cout << "\nAccount Validated";
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	std::cout << "\nRemoving Account From Database . . .";
	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	if (db->delete_user(username_input, password_input)) {
		std::cout << "\nUser Successfully Deleted From The Database\n\n";
	}

	Utils::Pause();
}

std::vector<std::string> AuthManager::get_user_logged_in() {
	std::vector<std::string> credentials;

	credentials.push_back(username_input);
	credentials.push_back(password_input);

	return credentials;
}
