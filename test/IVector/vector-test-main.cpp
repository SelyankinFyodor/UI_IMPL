#include "ILogger.h"
#include "vector-test-main.h"


int main() {
    ILogger *l = ILogger::createLogger(nullptr);
    add1(l);
    add2(l);
    add3(l);
    sub1(l);
    sub2(l);
    sub3(l);
    clone1(l);
    mulscalar1(l);
    mulscalar2(l);
    mulvector1(l);
    mulvector2(l);
    mulvector3(l);
    l->destroyLogger(nullptr);
    return 0;
}