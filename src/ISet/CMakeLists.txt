project(set)

set(SET_SOURCE ISet.cpp ISet_Impl.cpp)
set(SET_HEADERS ../../include/ISet.h ../../include/IVector.h ../../include/ILogger.h ../../include/RC.h)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON )
set(SET_FILES ${SET_SOURCE} ${SET_HEADERS})

add_library(set SHARED ${SET_FILES})
target_link_libraries(set ${CMAKE_CURRENT_SOURCE_DIR}/../..//bin/Selyankin/libvector.dll.a)
target_link_libraries(set ${CMAKE_CURRENT_SOURCE_DIR}/../..//bin/Selyankin/liblogger.dll.a)
set_target_properties(set PROPERTIES PREFIX "" OUTPUT_NAME "set")
