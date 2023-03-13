# Download googletest
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.0
)

FetchContent_MakeAvailable(googletest)

# Add test files
add_executable(ft_irc_test main.cpp parsing_test.cpp)
#target_compile_options(ft_irc_test PRIVATE -std=c++98 -Wall -Wextra -Werror)
target_compile_options(ft_irc_test PRIVATE -std=c++98)

target_link_libraries(ft_irc_test PUBLIC ft_irc_lib PRIVATE gtest gmock gtest_main)