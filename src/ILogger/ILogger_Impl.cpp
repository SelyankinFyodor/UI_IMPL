#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>

#include "ILogger.h"

namespace{
    class ILogger_Impl: public ILogger{
    private:
        std::vector<void *> _clients;
        std::string filename = "implwork.log";
        std::fstream _logfile;

        static std::string _rcTosting(RESULT_CODE rc);
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
            if (!pClient){
                std::cerr << "client could be null ptr";
                return nullptr;
            }
            logger->_clients.push_back(pClient);
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
        if(pClient){
            for (auto i = _clients.begin(); i !=_clients.end(); i++)
            if (*i == pClient){
                _clients.erase(i);
                if (_clients.empty()){
                    delete logger;
                    logger = nullptr;
                }
            }
        }
    }// ILogger_Impl::destroyLogger

    std::string ILogger_Impl::_rcTosting(RESULT_CODE rc) {
        switch(rc){
            case RESULT_CODE::SUCCESS:
                return std::string("SUCCESS");
            case RESULT_CODE::OUT_OF_MEMORY:
                return std::string("OUT_OF_MEMORY");
            case RESULT_CODE::BAD_REFERENCE:
                return std::string("BAD_REFERENCE");
            case RESULT_CODE::WRONG_DIM:
                return std::string("WRONG_DIM");
            case RESULT_CODE::DIVISION_BY_ZERO:
                return std::string("DIVISION_BY_ZERO");
            case RESULT_CODE::NAN_VALUE:
                return std::string("NAN_VALUE");
            case RESULT_CODE::FILE_ERROR:
                return std::string("FILE_ERROR");
            case RESULT_CODE::OUT_OF_BOUNDS:
                return std::string("OUT_OF_BOUNDS");
            case RESULT_CODE::NOT_FOUND:
                return std::string("NOT_FOUND");
            case RESULT_CODE::WRONG_ARGUMENT:
                return std::string("WRONG_ARGUMENT");
            case RESULT_CODE::CALCULATION_ERROR:
                return std::string("CALCULATION_ERROR");
            case RESULT_CODE::MULTIPLE_DEFINITION:
                return std::string("MULTIPLE_DEFINITION");
            default:
                return std::string("");
        }
    } // ILogger_Impl::_rcTosting
}