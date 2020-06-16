#include <iostream>
#include "ILogger.h"
#include "ILogger_Impl.cpp"

ILogger *ILogger::createLogger(void *pClient) {
    return ILogger_Impl::getInstance(pClient);
}

ILogger::~ILogger()
= default;