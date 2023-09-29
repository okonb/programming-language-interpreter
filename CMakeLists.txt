cmake_minimum_required(VERSION 3.14)
project(TKOM_Projekt)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "")
set(CMAKE_CXX_EXTENSIONS OFF)
#set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=*,-llvm*,-readability-identifier-length,-fuchsia*,-altera*,-modernize-use-trailing-return-type,-readability-function-cognitive-complexity,-misc-no-recursion,-google-readability-todo")
set(GENERAL_FLAGS   -Wall -Wextra -Wpedantic -Wshadow -Werror -Wconversion )
# -fsanitize=undefined -fsanitize-minimal-runtime
# add_link_options("-fsanitize=undefined")

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

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

target_include_directories(tkom_proj_lib PUBLIC include include/fwd)


add_library(tkom_proj_lib_gcov STATIC EXCLUDE_FROM_ALL ${proj_LIB})

target_include_directories(tkom_proj_lib_gcov PUBLIC include include/fwd)

target_compile_options(tkom_proj_lib_gcov PUBLIC ${GENERAL_FLAGS} --coverage -g3 -gdwarf-4 -O0)


add_executable(main src/main.cpp)

target_compile_options(main PUBLIC ${GENERAL_FLAGS} -O3)


target_link_libraries(main PUBLIC tkom_proj_lib)



add_executable(main_callgrind EXCLUDE_FROM_ALL src/main.cpp)

target_compile_options(main_callgrind PUBLIC ${GENERAL_FLAGS} -O3)

target_link_libraries(main_callgrind PUBLIC tkom_proj_lib)


# testing

enable_testing()


add_executable(
  tests
  test/tests.cpp
)

target_include_directories(tests PUBLIC include include/fwd)

target_compile_options(tests PUBLIC ${GENERAL_FLAGS} --coverage -g3 -gdwarf-4 -O0)

target_link_libraries(
  tests
  tkom_proj_lib_gcov
  gtest_main
)

target_link_options(tests PUBLIC --coverage)

include(GoogleTest)
gtest_discover_tests(tests)

add_custom_target(callgrind DEPENDS main_callgrind
  COMMAND   rm callgrind.out.*
  COMMAND   valgrind --tool=callgrind ./main_callgrind ../program_testowy.cccp > /dev/null
)

set_target_properties(callgrind PROPERTIES EXCLUDE_FROM_ALL 1)
