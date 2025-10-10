# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "C:/dev/flappy_demo/cmake-build-debug-visual-studio/_deps/threepp-src")
  file(MAKE_DIRECTORY "C:/dev/flappy_demo/cmake-build-debug-visual-studio/_deps/threepp-src")
endif()
file(MAKE_DIRECTORY
  "C:/dev/flappy_demo/cmake-build-debug-visual-studio/_deps/threepp-build"
  "C:/dev/flappy_demo/cmake-build-debug-visual-studio/_deps/threepp-subbuild/threepp-populate-prefix"
  "C:/dev/flappy_demo/cmake-build-debug-visual-studio/_deps/threepp-subbuild/threepp-populate-prefix/tmp"
  "C:/dev/flappy_demo/cmake-build-debug-visual-studio/_deps/threepp-subbuild/threepp-populate-prefix/src/threepp-populate-stamp"
  "C:/dev/flappy_demo/cmake-build-debug-visual-studio/_deps/threepp-subbuild/threepp-populate-prefix/src"
  "C:/dev/flappy_demo/cmake-build-debug-visual-studio/_deps/threepp-subbuild/threepp-populate-prefix/src/threepp-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/dev/flappy_demo/cmake-build-debug-visual-studio/_deps/threepp-subbuild/threepp-populate-prefix/src/threepp-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/dev/flappy_demo/cmake-build-debug-visual-studio/_deps/threepp-subbuild/threepp-populate-prefix/src/threepp-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
