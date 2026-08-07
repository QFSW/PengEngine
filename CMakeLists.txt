cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 23)

project(PengEngine)

if(CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_CONFIGURATION_TYPES "Debug;Development;Release;Master")
  set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING "" FORCE)
endif()

# Setup the different build configurations
add_compile_definitions("$<$<CONFIG:Debug>:PENG_DEBUG>")
add_compile_definitions("$<$<CONFIG:Development>:PENG_DEVELOPMENT>")
add_compile_definitions("$<$<CONFIG:Release>:PENG_RELEASE;NO_CHECKS>")
add_compile_definitions("$<$<CONFIG:Master>:PENG_MASTER;NO_CHECKS;NO_LOGGING;NO_PROFILING>")

# Use Release config as basis for Development and Master
set(CMAKE_CXX_FLAGS_DEVELOPMENT "${CMAKE_CXX_FLAGS_RELEASE}")
set(CMAKE_CXX_FLAGS_MASTER "${CMAKE_CXX_FLAGS_RELEASE}")
set(CMAKE_EXE_LINKER_FLAGS_DEVELOPMENT "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")
set(CMAKE_EXE_LINKER_FLAGS_MASTER "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")

# Set output directory
set(BUILD_OUTPUT ${PROJECT_SOURCE_DIR}/bin/$<CONFIGURATION>)

# Per platform defines, compiler options etc.
if (WIN32)
    add_compile_definitions(ARCH_AMD64 PLATFORM_WIN UNICODE _UNICODE)
    add_compile_options(/W4 /MP)

    set(OPTIMIZATION_FLAG "/O2")

elseif (APPLE)
    add_compile_definitions(ARCH_ARM64 PLATFORM_MAC)
    # add_compile_options(-Wextra)

    set(OPTIMIZATION_FLAG "-O3")
    set(DEBUG_FLAG "-g")
endif()

# Apply compiler flags per build
add_compile_options("$<$<CONFIG:Debug>:${DEBUG_FLAG}>")
add_compile_options("$<$<CONFIG:Development>:${OPTIMIZATION_FLAG}>")
add_compile_options("$<$<CONFIG:Release>:${OPTIMIZATION_FLAG}>")
add_compile_options("$<$<CONFIG:Master>:${OPTIMIZATION_FLAG}>")

add_executable(PengEngine)

set_target_properties(PengEngine PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BUILD_OUTPUT})

# Visual Studio integration for generated solutions (no-ops elsewhere)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT PengEngine)
set_target_properties(PengEngine PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

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

if (WIN32)
    # Use the libraries vendored in src/libs
    target_link_libraries(PengEngine
        opengl32
        ${PROJECT_SOURCE_DIR}/src/libs/glew/lib/Release/x64/glew32.lib
        ${PROJECT_SOURCE_DIR}/src/libs/glfw/lib-vc2022/glfw3.lib
        ${PROJECT_SOURCE_DIR}/src/libs/openal/lib/Release/OpenAL32.lib
    )

    # Set Master builds to have no console
    set_target_properties(PengEngine PROPERTIES WIN32_EXECUTABLE "$<CONFIG:Master>")
    target_link_options(PengEngine PRIVATE "$<$<CONFIG:Master>:/ENTRY:mainCRTStartup>")

    add_custom_command(TARGET PengEngine POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            ${PROJECT_SOURCE_DIR}/src/libs/glew/bin/Release/x64/glew32.dll
            ${PROJECT_SOURCE_DIR}/src/libs/openal/bin/Release/OpenAL32.dll
            ${BUILD_OUTPUT}
        COMMENT "Copying DLLs to output directory")
else()
    # OpenGL
    find_package(OpenGL REQUIRED)
    target_link_libraries(PengEngine OpenGL::GL)

    # GLEW
    find_package(GLEW REQUIRED)
    target_link_libraries(PengEngine GLEW::GLEW)

    # GLFW
    find_package(GLFW3 REQUIRED)
    target_link_libraries(PengEngine glfw)

    # OpenAL
    find_package(OpenAL REQUIRED)
    target_link_libraries(PengEngine OpenAL::OpenAL)
endif()

add_custom_command(TARGET PengEngine POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources
        ${BUILD_OUTPUT}/resources
    DEPENDS
        ${CMAKE_SOURCE_DIR}/resources
    COMMENT "Copying resources to output directory")
