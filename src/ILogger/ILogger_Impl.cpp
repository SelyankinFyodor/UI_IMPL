#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include <set>

#include "ILogger.h"

namespace{
    class ILogger_Impl: public ILogger{
    private:
        std::set<void *> _clients;
        std::string filename = "implwork.log";
        std::fstream _logfile;

        const static char * _rcTosting(RESULT_CODE rc);
    public:
        ILogger_Impl();

        void destroyLogger(void *pClient) override;

        void log(char const *pMsg, RESULT_CODE err) override;

        RESULT_CODE setLogFile(char const *pLogFile) override;

        static ILogger* getInstance(void *pClient);
    protected:
        ~ILogger_Impl() override;
    };

    ILogger_Impl * logger = nullptr;

    ILogger_Impl::ILogger_Impl():
            _logfile(nullptr)
    {}

    ILogger *ILogger_Impl::getInstance(void *pClient) {
        if (!logger){
            logger = new (std::nothrow)ILogger_Impl();
            if (!logger){
                std::cerr << "could not create logger instance";
                return nullptr;
            }
            if (logger->_clients.find(pClient) != logger->_clients.end()){
                logger->_clients.insert(pClient);
            }
            logger->_clients.insert(pClient);
        }

        return logger;
    } // ILogger_Impl::getInstance

    ILogger_Impl::~ILogger_Impl() {
        if (_logfile.is_open()){
            _logfile.flush();
            _logfile.close();
        }
    } // ILogger_Impl::~ILogger_Impl

    RESULT_CODE ILogger_Impl::setLogFile(char const *pLogFile) {
        if (!pLogFile){
            return RESULT_CODE::BAD_REFERENCE;
        }

        if (_logfile.is_open()){
            return RESULT_CODE::MULTIPLE_DEFINITION;
        }

        filename = std::string(pLogFile);

        return RESULT_CODE::SUCCESS;
    } // ILogger_Impl::setLogFile

    void ILogger_Impl::log(char const *pMsg, RESULT_CODE err) {

        time_t seconds = time(nullptr);
        tm* timeInfo = localtime(&seconds);
        if (!logger){
            std::cerr << "log - logger was not created";
            return;
        }
        std::string fullmsg = asctime(timeInfo);
        if (pMsg){
            fullmsg += std::string("|| message:") + pMsg;
        }

        if (!_logfile.is_open()){
            _logfile.open(filename, std::ios::out);
            if (!_logfile.is_open()){
                std::cerr << "log - could not open the log file for writing";
            }
        }

        fullmsg += std::string("|| result cod: ") + _rcTosting(err);

        _logfile << fullmsg << std::endl;
    } // ILogger_Impl::log

    void ILogger_Impl::destroyLogger(void *pClient) {
        if (!logger){
            std::cerr << "could not delete nullptr logger";
            return;
        }
        auto todelete = _clients.find(pClient);
        if (todelete != _clients.end()){
            _clients.erase(todelete);
        }
    }// ILogger_Impl::destroyLogger

    const char * ILogger_Impl::_rcTosting(RESULT_CODE rc) {
        char const* code_msg[] = {
                "SUCCESS",
                "OUT_OF_MEMORY",
                "BAD_REFERENCE",
                "WRONG_DIM",
                "DIVISION_BY_ZERO",
                "NAN_VALUE",
                "FILE_ERROR",
                "OUT_OF_BOUNDS",
                "NOT_FOUND",
                "WRONG_ARGUMENT",
                "CALCULATION_ERROR",
                "MULTIPLE_DEFINITION"
        };
        size_t msglen = 80;
        switch(rc){
            case RESULT_CODE::SUCCESS:
                return code_msg[0];
            case RESULT_CODE::OUT_OF_MEMORY:
                return code_msg[1];
            case RESULT_CODE::BAD_REFERENCE:
                return code_msg[2];
            case RESULT_CODE::WRONG_DIM:
                return code_msg[3];
            case RESULT_CODE::DIVISION_BY_ZERO:
                return code_msg[4];
            case RESULT_CODE::NAN_VALUE:
                return code_msg[5];
            case RESULT_CODE::FILE_ERROR:
                return code_msg[6];
            case RESULT_CODE::OUT_OF_BOUNDS:
                return code_msg[7];
            case RESULT_CODE::NOT_FOUND:
                return code_msg[8];
            case RESULT_CODE::WRONG_ARGUMENT:
                return code_msg[9];
            case RESULT_CODE::CALCULATION_ERROR:
                return code_msg[10];
            case RESULT_CODE::MULTIPLE_DEFINITION:
                return code_msg[11];
            default:
                return nullptr;
        }
    } // ILogger_Impl::_rcTosting
}