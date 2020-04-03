#ifndef BUDGETDATA_H
#define BUDGETDATA_H

#include <QDataStream>
#include <iostream>

class BudgetData
{
    /* The data structure representing a budget
     */
public:
    BudgetData();
    ~BudgetData();

    friend QDataStream & operator<<(QDataStream &out, const BudgetData &s);
    friend QDataStream & operator>>(QDataStream &in, BudgetData &s);


private:
    // Internal data structure

    bool checkRep();
};

#endif // BUDGETDATA_H
