project(logger)

set(LOGGER_SOURCE ILogger.cpp ILogger_Impl.cpp)
set(LOGGER_HEADERS ../../include/ILogger.h ../../include/RC.h)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON )
set(LOGGER_FILES ${LOGGER_SOURCE} ${LOGGER_HEADERS})

add_library(logger SHARED ${LOGGER_FILES})
set_target_properties(logger PROPERTIES PREFIX "" OUTPUT_NAME "logger")
