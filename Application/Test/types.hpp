#ifndef TYPES_H
#define TYPES_H

#include <unordered_map>
#include <vector>
#include <string>
#include <QDateTime>

#define ChartMap std::unordered_map<std::string, double>
#define LinePoints std::vector<std::pair<QDateTime, double> >
#define LineMap std::unordered_map<std::string, LinePoints>

#endif // TYPES_H
