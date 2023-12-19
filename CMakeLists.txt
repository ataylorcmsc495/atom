cmake_minimum_required(VERSION 3.22)
project(atom)

set(CMAKE_CXX_STANDARD 23)

add_executable(atom main.cpp
        # what.h
        # what.cpp
        # this.h
        # that.cpp
)

# Custom command to copy background.png to build directory post-build
#add_custom_command(TARGET atom POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy
#        ${CMAKE_SOURCE_DIR}/assets/background.png
#        $<TARGET_FILE_DIR:fika>)

configure_file(assets/vga.bmp ${CMAKE_CURRENT_BINARY_DIR}/vga.bmp COPYONLY)

# set(SFML_STATIC_LIBRARIES TRUE)

# set(SFML_DIR c:/ctools/sfml64/lib/cmake/SFML)
set(SFML_DIR ~/cpp/SFML/lib/cmake/SFML)

find_package(SFML COMPONENTS system window graphics audio network REQUIRED)

# include_directories(c:/ctools/sfml64/include)
include_directories(~/cpp/SFML/include)

set(SFML_LIBS sfml-system sfml-window sfml-graphics sfml-audio sfml-network)

target_link_libraries(atom ${SFML_LIBS})
