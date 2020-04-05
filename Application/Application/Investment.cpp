#include "Investment.h"

Investment::Investment()
{
    savings = 0;
    cd = 0;
    mutual = 0;
    bonds = 0;
    treasury = 0;
    stocks = new StockMap;
}

Investment::~Investment()
{
    delete stocks;
}

double Investment::getSavings() const
{
    return savings;
}

double Investment::getCD() const
{
    return cd;
}

double Investment::getMutual() const
{
    return mutual;
}

double Investment::getBond() const
{
    return bonds;
}

double Investment::getTreasury() const
{
    return treasury;
}

StockMap * Investment::getStocks() const
{
    return this->stocks;
}

