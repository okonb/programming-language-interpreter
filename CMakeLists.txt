cmake_minimum_required(VERSION 3.14)
project(TKOM_Projekt)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


file(GLOB proj_LIB CONFIGURE_DEPENDS "src/lib/*.h" "src/lib/*.cpp" "src/lib/*.hpp")

add_library(tkom_proj_lib STATIC ${proj_LIB})

target_include_directories(tkom_proj_lib PUBLIC include)

add_executable(main src/main.cpp)


target_link_libraries(main PUBLIC tkom_proj_lib)

# testing

enable_testing()


add_executable(
  tests
  test/tests.cpp
)

target_include_directories(tests PUBLIC include)

target_link_libraries(
  tests
  tkom_proj_lib
  gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)