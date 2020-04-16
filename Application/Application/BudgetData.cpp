#include "BudgetData.h"

BudgetData::BudgetData()
{
    this->data = new ChartMap;
    this->name = QString("");
    this->totalAmount = 0;
}

BudgetData::~BudgetData()
{
    delete data;
}

/*QDataStream & operator<<(QDataStream &out, const BudgetData &b)
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
    double sum = 0;
    std::string x = "";
    for (ChartMap::const_iterator i = data->begin(); i != data->end(); i++)
    {
        sum += (i->second * this->totalAmount);
        x += i->first + "\t\t" + QString::number(i->second * 100).toStdString() +
            "% |\t" + QString::number(i->second * this->totalAmount).toStdString() + "\n";
    }
    x += "\t Total Amount of Income: " + QString::number(this->totalAmount).toStdString() + "\n";
    x += "\t Total Allocated Income: " + QString::number(sum).toStdString() + "\n";
    x += "\t Total Remaining Income: " + QString::number(this->totalAmount - sum).toStdString() + "\n";
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
 * @requires val to be between [0..1]
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
    //std::cout<<sum<<std::endl;
    if (sum > 1.0)
    {
        //std::cout<<"Faield"<<std::endl;
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

/* FUnction used to get the value of a category
 * @param name is the name of the category
 * @returns the value of the category if it exists
 *          -1 if it does not exist.
 */
double BudgetData::getCategoryValue(QString name) const
{
    ChartMap::const_iterator i = data->find(name.toStdString());
    if (i != data->end())
    {
        return i->second;
    }
    return -1;
}

/* Function used to get the remaining percentage we can allocate
 * @returns 100 - sum of all the percentages of the current categories
 */
double BudgetData::getRemaining() const
{
    double sum = 0;
    for (ChartMap::const_iterator i = data->begin(); i != data->end(); i++)
    {
        sum += i->second;
    }
    return (1.0 - sum) * 100;
}

// Check rep function
void BudgetData::checkRep()
{
    // Each category name must be unique

    // Sum of all doubles must be <= 100%
}

/* Function to set budget name
 * @param budgetName is the name of the budget to pass in
 * @modifies this->name
 * @effect this->name = budgetName
 */
void BudgetData::setName(QString budgetName)
{
    this->name = budgetName;
}

/* Function to set budget total dollar amount
 * @param val is the total dollar amount for the budget
 * @modifies this->totalAmount
 * @effect this->totalAmount = val
 */
void BudgetData::setDollar(double val)
{
    this->totalAmount = val;
}

/* Function to get Budget Name
 * @returns the name of the budget
 */
const QString BudgetData::getName() const
{
    return this->name;
}

/* Function to get Budget total dollar amount
 * @returns the total dollar amount
 */
const double BudgetData::getDollar() const
{
    return this->totalAmount;
}

/* Function to check if the budget has no categories
 */
bool BudgetData::isEmpty()
{
    return this->data->empty();
}
