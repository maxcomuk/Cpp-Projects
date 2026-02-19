#pragma once

#include <string>

struct Employee {
    int id;

    std::string firstName;
    std::string lastName;
    std::string department;
    std::string employmentType;
    std::string hireDate;

    double hourlyRate;
    bool isActive{ false };
};
