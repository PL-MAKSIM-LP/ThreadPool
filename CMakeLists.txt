cmake_minimum_required(VERSION 3.16)
project(FromJuniorToMiddle LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включение предупреждений компилятора
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# === Основной исполняемый файл ===
add_executable(FromJuniorToMiddle
    main.cpp
    TaskQueue.h
    ThreadPool.h
    TypeSafeTask.h
    LockFreeQueue.h
)

find_package(Threads REQUIRED)
target_link_libraries(FromJuniorToMiddle PRIVATE Threads::Threads)

# Установка
include(GNUInstallDirs)
install(TARGETS FromJuniorToMiddle
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# === Настройка тестирования ===
enable_testing()

# Поиск GTest
find_package(GTest REQUIRED)
message(STATUS "Found GTest: ${GTEST_LIBRARIES}")

# Для Linux, чтобы не было проблем с CRT
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Тестовый исполняемый файл
add_executable(ThreadPoolTest
    ThreadPoolTest.cpp
    ThreadPool.h
)

# Связывание тестов с библиотеками
target_link_libraries(ThreadPoolTest PRIVATE
    GTest::GTest
    GTest::Main
    Threads::Threads
)

# Автоматическое обнаружение тестов
include(GoogleTest)
gtest_discover_tests(ThreadPoolTest
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
)

# Дополнительные цели для удобства
add_custom_target(run_tests
    COMMAND ${PROJECT_BINARY_DIR}/ThreadPoolTest
    DEPENDS ThreadPoolTest
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    VERBATIM
)
