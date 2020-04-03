#include "BudgetData.h"


QDataStream & operator<<(QDataStream &out, const BudgetData &b)
{
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
}
