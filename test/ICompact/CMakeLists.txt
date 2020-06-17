project(test-compact)

set(COMPACT_TEST_HEADERS
        compact-test-main.h
        ../../include/IVector.h
        ../../include/ICompact.h
        ../../include/ISet.h
        ../../include/ILogger.h
        )
set(COMPACT_TEST_SOURCES
        compact-test-main.cpp
        )

set(COMPACT_TEST_FILES ${COMPACT_TEST_SOURCES} ${COMPACT_TEST_HEADERS})

add_executable(compact-test ${COMPACT_TEST_FILES})
target_link_libraries(compact-test ${CMAKE_CURRENT_SOURCE_DIR}/../..//bin/Selyankin/libvector.dll.a)
target_link_libraries(compact-test ${CMAKE_CURRENT_SOURCE_DIR}/../..//bin/Selyankin/liblogger.dll.a)
target_link_libraries(compact-test ${CMAKE_CURRENT_SOURCE_DIR}/../..//bin/Selyankin/libset.dll.a)
target_link_libraries(compact-test ${CMAKE_CURRENT_SOURCE_DIR}/../..//bin/Selyankin/libcompact.dll.a)
set_target_properties(compact-test PROPERTIES PREFIX "test-" OUTPUT_NAME "compact")
