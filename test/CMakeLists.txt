find_package(GTest REQUIRED)
find_package(GMock REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}")
set(PROJECT_TEST_NAME ${PROJECT_NAME}_test)
file(GLOB_RECURSE TEST_SOURCE_FILES ${PROJECT_SOURCE_DIR}/test/*.cpp)
include_directories("${PROJECT_SOURCE_DIR}/include" ${GTEST_INCLUDE_DIRS})
add_executable(${PROJECT_TEST_NAME} ${TEST_SOURCE_FILES})
target_link_libraries(
        ${PROJECT_TEST_NAME}
        ${GTEST_BOTH_LIBRARIES}
        ${GMOCK_BOTH_LIBRARIES}
        ${PROJECT_NAME})
