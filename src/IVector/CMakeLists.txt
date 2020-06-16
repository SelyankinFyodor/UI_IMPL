project(vector)

set(VECTOR_SOURCES IVector_Impl.cpp IVector.cpp)
set(VECTOR_HEADERS ../../include/IVector.h ../../include/ILogger.h ../../include/RC.h ../../include/lib_Export.h)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON )
set(VECTOR_FILES ${VECTOR_SOURCES} ${VECTOR_HEADERS})

add_library(vector SHARED ${VECTOR_FILES})
target_link_libraries(vector ${CMAKE_CURRENT_SOURCE_DIR}/../..//bin/Selyankin/liblogger.dll.a)
set_target_properties(vector PROPERTIES PREFIX "" OUTPUT_NAME "vector")
