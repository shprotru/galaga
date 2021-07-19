cmake_minimum_required(VERSION 2.6)
project(MyProject)

# Use our modified FindSDL2* modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${MyProject_SOURCE_DIR}/cmake")
set(BIN_DIR ${MyProject_SOURCE_DIR}/bin)

# Bump up warning levels appropriately for clang, gcc & msvc and build in debug mode
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++17")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
	if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	endif()
endif()

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

####################################################################################################
# This function converts any file into C/C++ source code.
# Example:
# - input file: data.dat
# - output file: data.h
# - variable name declared in output file: DATA
# - data length: sizeof(DATA)
# embed_resource("data.dat" "data.h" "DATA")
####################################################################################################

function(embed_resource resource_file_name source_file_name variable_name)
    file(READ ${resource_file_name} hex_content HEX)
    string(REPEAT "[0-9a-f]" 32 column_pattern)
    string(REGEX REPLACE "(${column_pattern})" "\\1\n" content "${hex_content}")
    string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1, " content "${content}")
    string(REGEX REPLACE ", $" "" content "${content}")
    set(array_definition "const unsigned char ${variable_name}[] =\n{\n${content}\n};")
    set(header ${source_file_name})
    string(REGEX REPLACE ".*/" "" header "${header}")
    string(REGEX REPLACE "_.*.[cC][pP][pP]" ".h" header "${header}")
    set(source "#include \"${header}\"\n\n// Auto generated file.\n${array_definition}\n")
    file(WRITE "${source_file_name}" "${source}")
endfunction()

# embed_resource("assets/main.bmp" "src/assets_main.cpp" "gameAssets")
# embed_resource("assets/bg.bmp" "src/assets_bg.cpp" "gameBg")
add_executable( MyProject
    src/main.cpp
    src/galaga.cpp
    src/assets_bg.cpp
    src/assets_main.cpp
    src/loader.cpp
    src/spriteInfo.cpp
    src/timer.cpp
    src/models/model.cpp
    src/models/basic.cpp
    src/models/enemy1/enemy1.cpp
    src/models/enemy2/enemy2.cpp
    src/models/spaceship/spaceship.cpp
    src/models/background/background.cpp
)
target_link_libraries(MyProject ${SDL2_LIBRARY})
install(TARGETS MyProject RUNTIME DESTINATION ${BIN_DIR})
