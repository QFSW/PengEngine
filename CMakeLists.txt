cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 23)

if(CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_CONFIGURATION_TYPES "Debug; Development; Release; Master")
  set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING "" FORCE)
endif()

project(PengEngine)

# Setup the different build configurations
add_compile_definitions("$<$<CONFIG:Debug>:         PENG_DEBUGø>")
add_compile_definitions("$<$<CONFIG:Development>:   PENG_DEVELOPMENT>")
add_compile_definitions("$<$<CONFIG:Release>:       PENG_RELEASE;   NO_CHECKS>")
add_compile_definitions("$<$<CONFIG:Master>:        PENG_MASTER;    NO_CHECKS; NO_LOGGING; NO_PROFILING>")

# Set output directory
set(BUILD_OUTPUT ${PROJECT_SOURCE_DIR}/bin/$<CONFIGURATION>)

# Per platform defines, compiler options etc.
if (WIN32)
    add_compile_definitions(ARCH_AMD64 PLATFORM_WIN)
    add_compile_options(/W4)

    set(OPTIMIZATION_FLAG "/O3")

elseif (APPLE)
    add_compile_definitions(ARCH_ARM64 PLATFORM_MAC)
    # add_compile_options(-Wextra)

    set(OPTIMIZATION_FLAG "-o3")
    set(DEBUG_FLAG "-g")
endif()

# Apply compiler flags per build
add_compile_options("$<$<CONFIG:Debug>:${DEBUG_FLAG}>")
add_compile_options("$<$<CONFIG:Development>:${OPTIMIZATION_FLAG}>")
add_compile_options("$<$<CONFIG:Release>:${OPTIMIZATION_FLAG}>")
add_compile_options("$<$<CONFIG:Master>:${OPTIMIZATION_FLAG}>")

add_executable(PengEngine)

set_target_properties(PengEngine PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BUILD_OUTPUT})

include_directories(AFTER SYSTEM 
    "src"
    "src/libs/glew/include"
    "src/libs/glfw/include"
    "src/libs/moodycamel"
    "src/libs/nlohmann"
    "src/libs/openal/include"
    "src/libs/stb"
    "src/libs/superluminal"
)

file(GLOB_RECURSE SRC_CPP CONFIGURE_DEPENDS "src/**/*.cpp")
target_sources(PengEngine PRIVATE "src/main.cpp" ${SRC_CPP})

# OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(PengEngine OpenGL::GL)

# GLEW
find_package(GLEW REQUIRED)
target_link_libraries(PengEngine GLEW::GLEW)

# GLFW
add_subdirectory("src/libs/glfw")
target_link_libraries(PengEngine glfw ${GLFW_LIBRARIES})

# OpenAL
find_package(OpenAL REQUIRED)
target_link_libraries(PengEngine OpenAL::OpenAL)

add_custom_command(TARGET PengEngine POST_BUILD 
  COMMAND "${CMAKE_COMMAND}" -E copy_directory 
    ${CMAKE_SOURCE_DIR}/resources
     ${BUILD_OUTPUT}/resources
  COMMENT "Copying resources to output directory")