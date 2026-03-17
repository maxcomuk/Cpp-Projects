#pragma once

#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <chrono>
#include <iomanip>

#include "../database/Database.h"

class BankManager
{
private:
	const std::string account_username;
	const std::string account_password;

	Database* db;
public:
	BankManager(Database* db, const std::string& username, const std::string& password);
	BankManager();

	bool setup_bank_system();
	void deposit();
	void withdraw();

	void display_account_info();
	void render_account_info(const bool& show_password, const double& balance);

	void transfer();
};
