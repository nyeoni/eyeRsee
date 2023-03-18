# Project version
cmake_minimum_required(VERSION 3.5)
cmake_policy(SET CMP0048 NEW)

# set the project name and version
project(ft_irc LANGUAGES CXX VERSION 0.1)

# 라이브러리 파일은 빌드 디렉토리 안에 lib 폴더에 출력.
# CMAKE_BINARY_DIR -> build
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 실행 파일은 빌드 디렉토리 안에 bin 폴더에 출력.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Create ft_irc_lib interface
add_library(ft_irc_lib STATIC src/entity/Channel.cpp src/entity/Client.cpp src/handler/EventHandler.cpp src/core/Server.cpp src/core/Socket.cpp src/controller/ChannelController.cpp src/controller/ClientController.cpp include/controller/Executor.hpp src/controller/Executor.cpp src/core/Udata.cpp src/core/Parser.cpp src/core/utility.cpp include/handler/ErrorHandler.hpp)
# Include ft_irc hpp files
target_include_directories(ft_irc_lib PUBLIC include)
# Set ft_irc_lib compile option
set_target_properties(ft_irc_lib PROPERTIES INTERFACE_LINK_LIBRARIES -Wall -Wextra -Werror -std=c++98 -g3)
#set_target_properties(ft_irc_lib PROPERTIES INTERFACE_LINK_LIBRARIES -std=c++98)

# Add ft_irc exec
add_executable(ft_irc src/main/main.cpp)
# Include containers hpp files
target_link_libraries(ft_irc PUBLIC ft_irc_lib)
# Set ft_containers_lib compile option
set_target_properties(ft_irc PROPERTIES INTERFACE_LINK_LIBRARIES -Wall -Wextra -Werror -std=c++98 -g3)
#set_target_properties(ft_irc PROPERTIES INTERFACE_LINK_LIBRARIES -std=c++98)

add_subdirectory(test)