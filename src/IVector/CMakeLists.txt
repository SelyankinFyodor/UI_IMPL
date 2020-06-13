project(IVector_LIB)                            # Название проекта

set(SOURCES IVector_Impl.cpp IVector.cpp)                 # Установка переменной со списком исходников
set(HEADERS ../../include/IVector.h ../../include/ILogger.h ../../include/lib_Export.h)
set ( CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON )
set(FILES ${SOURCES} ${HEADERS})
add_library(IVector_LIB SHARED ${FILES})
target_include_directories(IVector_LIB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
install(TARGETS IVector_LIB DESTINATION DLib-bin)
add_executable(main main.cpp)
target_link_libraries(main IVector_LIB)