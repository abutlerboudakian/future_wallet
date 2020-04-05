#ifndef WAGE_H
#define WAGE_H

#include <iostream>
#include <string>

class Wage
{

public:
    Wage();

    double getSalary() const;
    double getHourlySalary() const;
    double getWeeklyHours() const;
    std::string getIndustry() const;

private:
    double salary;
    double salary_hourly;
    double hours;
    std::string industry;
};

#endif // WAGE_H
