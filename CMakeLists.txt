cmake_minimum_required(VERSION 3.15)

project(UI_IMPL)

include_directories(include)

add_subdirectory(src)

add_executable(main main.cpp include/IVector.h include/ILogger.h include/RC.h)
link_directories(bin)
target_link_libraries(main ${CMAKE_CURRENT_SOURCE_DIR}/bin/Selyankin/libvector.dll.a)