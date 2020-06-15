project(set)

set(SET_SOURCE ISet.cpp ISet_Impl.cpp)
set(SET_HEADERS ../../include/ISet.h ../../include/IVector.h ../../include/ILogger.h ../../include/RC.h ../../include/lib_Export.h)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON )
set(SET_FILES ${SET_SOURCE} ${SET_HEADERS})

add_library(set SHARED ${SET_FILES})
target_link_libraries(set ${CMAKE_CURRENT_SOURCE_DIR}/../..//bin/Selyankin/libvector.dll.a)
