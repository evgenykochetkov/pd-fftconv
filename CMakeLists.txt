cmake_minimum_required(VERSION 3.18)
project(pd-fftconv LANGUAGES C CXX)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "" FORCE)

include(${CMAKE_SOURCE_DIR}/pd.cmake)


if(APPLE)
    set(AUDIOFFT_APPLE_ACCELERATE ON)
endif()
# TODO: enable optimisations for other platforms

add_subdirectory(FFTConvolver)

pd_add_external(fftconv~ src/fftconv~.cpp)
target_link_libraries(fftconv_tilde PRIVATE FFTConvolver)

