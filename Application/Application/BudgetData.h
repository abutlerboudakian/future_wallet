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

    void setName(QString budgetName);
    void setDollar(double val);
    const QString getName() const;
    const double getDollar() const;

    bool addCategory(QString name, double val);
    bool removeCategory(QString name);
    double getCategoryValue(QString name) const;

    double getRemaining() const;
    QString getBudgetString() const;
    const ChartMap * getBudgetChartMap() const;

    bool isEmpty() const;


private:
    ChartMap * data;
    double totalAmount;
    QString name;

};

#endif // BUDGETDATA_H
