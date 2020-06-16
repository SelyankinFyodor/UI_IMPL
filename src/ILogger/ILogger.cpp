#include <iostream>
#include "ILogger.h"
#include "ILogger_Impl.cpp"

ILogger *ILogger::createLogger(void *pClient) {
    if (!pClient){
        std::cerr << "could not create logger from nullptr client";
        return nullptr;
    }

    return ILogger_Impl::getInstance(pClient);
}

ILogger::~ILogger()
= default;