cmake_minimum_required(VERSION 3.19)
project(flappy_demo)
include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
set(THREEPP_BUILD_TESTS OFF)
set(THREEPP_BUILD_EXAMPLES OFF)

FetchContent_Declare(
        threepp
        GIT_REPOSITORY https://github.com/markaren/threepp.git
        GIT_TAG 619cdaacc2e7dc6ea709d8c7f482e2994af7349d
)
FetchContent_MakeAvailable(threepp)

add_executable(flappy_demo main.cpp
        constants.hpp
        bird.hpp
        points.hpp
        pipes.hpp
        collisions.hpp
)
target_link_libraries(flappy_demo PRIVATE threepp::threepp)

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
