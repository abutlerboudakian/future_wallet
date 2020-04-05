#ifndef BUDGETDATA_H
#define BUDGETDATA_H

#include <QDataStream>
#include <iostream>
#include "types.hpp"

class BudgetData
{
    /* The data structure representing a budget
     *
     * data contains pairs of <std::string, double>
     *                       category name, [0, 1]
     *                                       ^^-decimal percent
     *
     * rep invariant: the category names must be unique
     *              : the sum of all the doubles must be <= 1
     */
public:
    BudgetData();
    ~BudgetData();

    friend QDataStream & operator<<(QDataStream &out, const BudgetData &s);
    friend QDataStream & operator>>(QDataStream &in, BudgetData &s);

    bool addCategory(QString name, double val);
    bool removeCategory(QString name);

    QString getBudgetString() const;
    const ChartMap * getBudgetChartMap() const;

private:
    ChartMap * data;

    void checkRep();
};

#endif // BUDGETDATA_H
