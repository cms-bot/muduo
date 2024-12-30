
#include "logger.h"
int main()
{
    Logger::GetInstance().Init();

    UVLOG_INFO("channel handle revents %d\n",0);
    return 0;
}