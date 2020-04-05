#ifndef INVESTMENT_H
#define INVESTMENT_H

#include <iostream>
#include <string>
#include <unordered_map>

#define StockMap std::unordered_map<std::string, double>

class Investment
{

public:
    Investment();
    ~Investment();

    double getSavings() const;
    double getCD() const;
    double getMutual() const;
    double getBond() const;
    double getTreasury() const;
    StockMap * getStocks() const;

private:
    double savings;
    double cd;
    double mutual;
    double bonds;
    double treasury;
    StockMap * stocks;
};

#endif // INVESTMENT_H
