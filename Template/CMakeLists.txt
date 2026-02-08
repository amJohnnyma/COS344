#What you want to do                  Command                                      Dir
#-----------------------------------------------------------------------------------------------
#Configure project                    cmake ..                                     build/
#Rebuild everything                   make    or    make -j$(nproc)                build/
#Clean build files                    make clean                                   build/
#Super clean (nuclear option)         rm -rf *    or delete build/ folder          build/ or root
#Run the program                      make run                                     build/
#Run directly                         ./bin/MyOpenGLProject                        build/
#Reconfigure + build + run            cmake .. && make -j && make run              build/

cmake_minimum_required(VERSION 3.10...3.28)
project(MyOpenGLProject LANGUAGES C CXX)

set(CMAKE_C_STANDARD   11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Collect sources (non-recursive – good for flat src/)
file(GLOB SOURCES
    "${CMAKE_SOURCE_DIR}/src/*.c"
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.cc"
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_SOURCE_DIR}/include"
)

# Warnings
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall -Wextra -Wpedantic -Wshadow -Wconversion
    )
endif()

# Dependencies – use pkg-config (very reliable on Ubuntu/Xubuntu)
find_package(PkgConfig REQUIRED)

pkg_check_modules(GLFW REQUIRED glfw3)
pkg_check_modules(GLEW  REQUIRED glew)

find_package(OpenGL REQUIRED)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenGL::GL
    ${GLFW_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${CMAKE_DL_LIBS}    # for dlopen / glad
    pthread
    m                   # math
)

# Extra X11 stuff (pkg-config usually pulls most, but explicit helps)
if(UNIX AND NOT APPLE)
    find_package(X11 REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        X11::X11 X11::Xrandr X11::Xinerama X11::Xcursor X11::Xi
    )
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/src/shaders"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders"
    COMMENT "Copying shaders folder..."
)

# Optional: run target
add_custom_target(run
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    COMMENT "Running ${PROJECT_NAME} ..."
    USES_TERMINAL
)

add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/cmake_clean.cmake
    COMMENT "Removing build files, bin, lib folders..."
)
