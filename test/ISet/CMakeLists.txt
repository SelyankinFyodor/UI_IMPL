project(test-set)

set(SET_TEST_HEADERS
        set-test-main.h
        ../../include/IVector.h
        ../../include/ISet.h
        ../../include/ILogger.h
        ../../include/RC.h
        ../../include/lib_Export.h
        )
set(SET_TEST_SOURCES
        set-test-main.cpp
        set-test-insert.cpp
        set-test-get.cpp
        set-test-clear.cpp
        set-test-clone.cpp
        set-test-erase.cpp
        )

set(SET_TEST_FILES ${SET_TEST_SOURCES} ${SET_TEST_HEADERS})

add_executable(set-test ${SET_TEST_FILES})
target_link_libraries(set-test ${CMAKE_CURRENT_SOURCE_DIR}/../..//bin/Selyankin/libset.dll.a)
target_link_libraries(set-test ${CMAKE_CURRENT_SOURCE_DIR}/../..//bin/Selyankin/libvector.dll.a)
target_link_libraries(set-test ${CMAKE_CURRENT_SOURCE_DIR}/../..//bin/Selyankin/libLogger.dll.a)
set_target_properties(set-test PROPERTIES PREFIX "test-" OUTPUT_NAME "set")
