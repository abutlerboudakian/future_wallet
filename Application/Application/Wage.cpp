#include "Wage.h"

Wage::Wage()
{
    salary = 0;
    salary_hourly = 0;
    hours = 0;
    industry = "";
}

double Wage::getSalary() const
{
    return salary;
}

double Wage::getHourlySalary() const
{
    return salary_hourly;
}

double Wage::getWeeklyHours() const
{
    return hours;
}

std::string Wage::getIndustry() const
{
    return industry;
}
