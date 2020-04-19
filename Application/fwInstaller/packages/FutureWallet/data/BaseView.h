#ifndef BASEVIEW_H
#define BASEVIEW_H

class Controller; // Forward declaration to resolve circular dependency
#include "Controller.h"

class BaseView
{
    protected:
        Controller * controller;
};

#endif // BASEVIEW_H
