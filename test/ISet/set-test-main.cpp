#include "set-test-main.h"

int main(){
    ILogger *l = ILogger::createLogger(nullptr);
    insert1(l);
    insert2(l);
    insert3(l);

    get1(l);
    get2(l);
    get3(l);

    clear1(l);

    clone1(l);

    erase1(l);
    erase2(l);
    erase3(l);

    l->destroyLogger(nullptr);
    return 0;
}