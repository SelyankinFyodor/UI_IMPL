project(test-vector)

set(VECTOR_TEST_HEADERS
        vector-test-main.h
        ../../include/IVector.h
        ../../include/ILogger.h
        ../../include/RC.h
        ../../include/lib_Export.h
        )
set(VECTOR_TEST_SOURCES
        vector-test-main.cpp
        vector-test-add.cpp
        vector-test-sub.cpp
        vector-test-clone.cpp
        vector-test-mul-scalar.cpp
        vector-test-mul-vector.cpp
        )

set(VECTOR_TEST_FILES ${VECTOR_TEST_SOURCES} ${VECTOR_TEST_HEADERS})

add_executable(vector-test ${VECTOR_TEST_FILES})
target_link_libraries(vector-test ${CMAKE_CURRENT_SOURCE_DIR}/../..//bin/Selyankin/libvector.dll.a)

set_target_properties(vector-test PROPERTIES PREFIX "test-" OUTPUT_NAME "vector")
