cmake_minimum_required(VERSION 3.19)
project(flappy_demo)
include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(THREEPP_BUILD_TESTS OFF)
set(THREEPP_BUILD_EXAMPLES OFF)

# Hent assimp
set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_INSTALL OFF)

# Bygg statiske biblioteker (assimp uten .dll)
set(BUILD_SHARED_LIBS OFF)
set(ASSIMP_BUILD_SHARED_LIBS OFF)

FetchContent_Declare(
        assimp
        GIT_REPOSITORY https://github.com/assimp/assimp.git
        GIT_TAG v6.0.2
)
FetchContent_MakeAvailable(assimp)

# hent threepp
FetchContent_Declare(
        threepp
        GIT_REPOSITORY https://github.com/markaren/threepp.git
        GIT_TAG 619cdaacc2e7dc6ea709d8c7f482e2994af7349d
)
FetchContent_MakeAvailable(threepp)

add_executable(flappy_demo main.cpp
        constants.hpp
        bird.hpp
        pipes.hpp
        collisions.hpp
        game_loop_logic.hpp
)
target_link_libraries(flappy_demo
        PRIVATE
        threepp::threepp
        assimp::assimp
)

target_compile_definitions(flappy_demo PRIVATE
        DATA_FOLDER="${CMAKE_CURRENT_SOURCE_DIR}/data"
)

######### Emscripten #######
if (DEFINED EMSCRIPTEN)

    set(LINK_FLAGS " --bind -sUSE_GLFW=3 -sGL_DEBUG=1 -sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2 \
    -sFULL_ES3 -sASSERTIONS -sALLOW_MEMORY_GROWTH -sWASM=1")

    set_target_properties("for_29"
            PROPERTIES SUFFIX ".html"
            LINK_FLAGS "${LINK_FLAGS}"
    )

endif ()
###########################
