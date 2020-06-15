project(compact)

set(COMPACT_SOURCE ICompact.cpp ICompact_Impl.cpp)
set(COMPACT_HEADERS ../../include/ICompact.h ../../include/ILogger.h )

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON )
set(COMPACT_FILES ${COMPACT_SOURCE} ${COMPACT_HEADERS})

add_library(compact SHARED ${COMPACT_FILES})
