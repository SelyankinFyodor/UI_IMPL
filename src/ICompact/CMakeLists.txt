project(compact)

set(COMPACT_SOURCE ICompact.cpp ICompact_Impl.cpp)
set(COMPACT_HEADERS ../../include/ICompact.h ../../include/ILogger.h ../../include/IVector.h)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON )
set(COMPACT_FILES ${COMPACT_SOURCE} ${COMPACT_HEADERS})

add_library(compact SHARED ${COMPACT_FILES})
target_link_libraries(compact ${CMAKE_CURRENT_SOURCE_DIR}/../..//bin/Selyankin/libvector.dll.a)
target_link_libraries(compact ${CMAKE_CURRENT_SOURCE_DIR}/../..//bin/Selyankin/liblogger.dll.a)
set_target_properties(compact PROPERTIES PREFIX "" OUTPUT_NAME "compact")
