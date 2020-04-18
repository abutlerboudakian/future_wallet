#include "TestBudgetData.h"
#include "TestRequests.h"

int main(int argc, char** argv)
{
   int status = 0;
   {
      TestBudgetData tc;
      status |= QTest::qExec(&tc, argc, argv);
   }
   {
      TestRequests tc;
      status |= QTest::qExec(&tc, argc, argv);
   }
   return status;
}
