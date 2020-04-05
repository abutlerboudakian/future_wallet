#include "BudgetData.h"

BudgetData::BudgetData()
{
    this->data = new ChartMap;
}

BudgetData::~BudgetData()
{
    delete data;
}
/*
QDataStream & operator<<(QDataStream &out, const BudgetData &b)
{
    //hashmap of [category] -> percent as a double
     //   checkrep is (sigma percent) <= 100

    //out << s.ID << s.Name;
    std::cout<<"wrote out"<<std::endl;
    return out;
}

QDataStream & operator>>(QDataStream &in, BudgetData &b)
{
    std::cout<<"read in"<<std::endl;
    //s = Student();
    //in >> s.ID >> s.Name;
    return in;
}*/

/* Function converts BudgetData to a QString formated for DashBoard
 * @returns a QString formated representation of this->data
 */
QString BudgetData::getBudgetString() const
{
    std::string x;
    for (ChartMap::const_iterator i = data->begin(); i != data->end(); i++)
    {
        x += i->first + "\t\t" + QString::number(i->second * 100).toStdString() + "%" + '\n';
    }
    return QString::fromStdString(x);
}

/* Function returns this->data
 * @returns a const of this->data
 */
const ChartMap * BudgetData::getBudgetChartMap() const
{
    return this->data;
}

/* Function to add a category to the budget
 * @requires name != nullptr && name != ""
 * @param name is the name of the category
 * @param val is the value for the category
 * @modifies this->data
 * @effect this->data contains a new category, if it is successfully added
 * @returns bool based on whether the category can be added to the budget
 */
bool BudgetData::addCategory(QString name, double val)
{
    // To add, name must be unqiue and the sum of the values
    // including the addition must be < 100
    std::string category = name.toStdString();
    double sum = val;
    for (ChartMap::const_iterator i = data->begin(); i != data->end(); i++)
    {
        if (i->first == category)
        {
            return false;
        }
        sum += i->second;
    }
    std::cout<<sum<<std::endl;
    if (sum > 1.0)
    {
        std::cout<<"Faield"<<std::endl;
        return false;
    }
    data->insert(std::pair<std::string, double>(category, val));
    checkRep();
    return true;
}

/* Function to remove a category from the budget
 * @requires name != nullptr && name != "" && name is unique
 * @param name is the name of the category to remove
 */
bool BudgetData::removeCategory(QString name)
{
    std::string category = name.toStdString();
    for (ChartMap::const_iterator i = data->begin(); i != data->end(); i++)
    {
        if (i->first == category)
        {
            data->erase(i);
            return true;
        }
    }
    return false;
    checkRep();
}

// Check rep function
void BudgetData::checkRep()
{
    // Each category name must be unique

    // Sum of all doubles must be <= 100%
}
