cmake_minimum_required(VERSION 3.16)
project(FromJuniorToMiddle LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

add_executable(FromJuniorToMiddle
    main.cpp
)

find_package(Threads REQUIRED)
target_link_libraries(FromJuniorToMiddle PRIVATE Threads::Threads)

# Установка
include(GNUInstallDirs)
install(TARGETS FromJuniorToMiddle
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

enable_testing()
find_package(GTest REQUIRED)
message(STATUS "Found GTest: ${GTEST_LIBRARIES}")
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# === Tests Thread Pool ===
add_executable(ThreadPoolTest
    TaskQueue.h
    ThreadPool.h
    TypeSafeTask.h
    LockFreeQueue.h
    ThreadPoolTest.cpp
)

target_link_libraries(ThreadPoolTest PRIVATE
    GTest::GTest
    GTest::Main
    Threads::Threads
)

include(GoogleTest)
gtest_discover_tests(ThreadPoolTest
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
)

add_custom_target(run_tests
    COMMAND ${PROJECT_BINARY_DIR}/ThreadPoolTest
    DEPENDS ThreadPoolTest
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    VERBATIM
)
