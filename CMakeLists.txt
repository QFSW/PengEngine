cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 23)

project(PengEngine)

# PENG_DEBUG config
add_compile_options(-g3 -fsanitize=address)
add_link_options(-fsanitize=address)
add_compile_definitions(PENG_DEBUG)

if (WIN32)
    add_compile_definitions(ARCH_AMD64 PLATFORM_WIN)
elseif (APPLE)
    add_compile_definitions(ARCH_ARM64 PLATFORM_MAC)
endif()

add_executable(PengEngine)

set_target_properties(PengEngine PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

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