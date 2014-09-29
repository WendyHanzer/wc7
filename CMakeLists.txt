cmake_minimum_required(VERSION 2.8)

project(water)

include_directories("include")

aux_source_directory(src Engine_src)
set(Engine_src ${Engine_src}) #${gui_src})
#add_library(GameEngine ${GameEngine_src})
add_executable(water ${Engine_src})

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") #apple
    message(STATUS "Mac OS Detected")
    include_directories("/usr/local/include/SDL2")
    include_directories("/usr/local/include/freetype2")
    set(LIBRARIES_TO_LINK "SDL2" "boost_program_options" "freeimageplus")
    #set(MAC_FRAMEWORKS "-framework OpenGL")
    set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11 -stdlib=libc++ -O2")

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework OpenGL")

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    message(STATUS "Linux OS Detected")
    set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11")
    include(FindPkgConfig)
    pkg_search_module(SDL2 REQUIRED sdl2)
    pkg_search_module(GLEW REQUIRED glew)
    #pkg_search_module(GDAL REQUIRED gdal)
    #find_package(Boost REQUIRED COMPONENTS program_options python)
    find_package(Boost REQUIRED COMPONENTS program_options)
    #pkg_search_module(PYTHON REQUIRED python2)
    include_directories(${SDL2_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

    set(LIBRARIES_TO_LINK ${SDL2_LIBRARIES} ${GLEW_LIBRARIES} ${Boost_LIBRARIES})

else()
    message(STATUS "Windows OS Detected")
    message(FATAL_ERROR0 "Windows is not supported")

endif()

target_link_libraries(water ${LIBRARIES_TO_LINK})
