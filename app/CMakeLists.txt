cmake_minimum_required(VERSION 3.10)
project(RenderscriptGradle64 VERSION 0.0.28 LANGUAGES C CXX)

option(BUILD_SHARED_LIBS "Build as shared library" ON)

include(FindPackageHandleStandardArgs)

find_path(Android_INCLUDE_DIR android/api-level.h HINTS ${ANDROID_NDK}/sysroot/usr/include)
find_library(Android_LIBRARY NAMES android)
find_package_handle_standard_args(Android DEFAULT_MSG Android_LIBRARY Android_INCLUDE_DIR)
if(Android_FOUND)
  set(Android_LIBRARIES ${Android_LIBRARY})
  set(Android_INCLUDE_DIRS ${Android_INCLUDE_DIR})
  add_library(Android::Android SHARED IMPORTED)
  set_target_properties(Android::Android PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${Android_INCLUDE_DIR}
    IMPORTED_LOCATION ${Android_LIBRARY})
else()
  message(FATAL_ERROR "Could not find Android internal library.")
endif()

find_path(AndroidLog_INCLUDE_DIR log.h HINTS ${ANDROID_NDK}/sysroot/usr/include/android)
find_library(AndroidLog_LIBRARY NAMES log)
find_package_handle_standard_args(AndroidLog DEFAULT_MSG AndroidLog_LIBRARY AndroidLog_INCLUDE_DIR)
if(AndroidLog_FOUND)
  set(AndroidLog_LIBRARIES ${AndroidLog_LIBRARY})
  set(AndroidLog_INCLUDE_DIRS ${AndroidLog_INCLUDE_DIR})
  add_library(Android::Log SHARED IMPORTED)
  set_target_properties(Android::Log PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${AndroidLog_INCLUDE_DIR}
    IMPORTED_LOCATION ${AndroidLog_LIBRARY})
else()
  message(FATAL_ERROR "Could not find Android logging library.")
endif()

find_path(RSCpp_INCLUDE_DIR rsDefines.h HINTS
    ${ANDROID_NDK}/toolchains/renderscript/prebuilt/${ANDROID_HOST_TAG}/platform/rs/)
find_library(RSCpp_LIBRARY NAMES RScpp_static HINTS
    ${ANDROID_NDK}/toolchains/renderscript/prebuilt/${ANDROID_HOST_TAG}/platform/${ANDROID_SYSROOT_ABI})
find_package_handle_standard_args(RSCpp DEFAULT_MSG RSCpp_LIBRARY RSCpp_INCLUDE_DIR)
if(RSCpp_FOUND)
  set(RSCpp_LIBRARIES ${RSCpp_LIBRARY})
  set(RSCpp_INCLUDE_DIRS ${RSCpp_INCLUDE_DIR} ${RSCpp_INCLUDE_DIR}/cpp ${RSCpp_INCLUDE_DIR}/scriptc)
  add_library(RS::Cpp STATIC IMPORTED)
  set_target_properties(RS::Cpp PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${RSCpp_INCLUDE_DIR}
    IMPORTED_LOCATION ${RSCpp_LIBRARY})
  set_property(TARGET RS::Cpp APPEND PROPERTY
    INTERFACE_INCLUDE_DIRECTORIES ${RSCpp_INCLUDE_DIR}/cpp)
  set_property(TARGET RS::Cpp APPEND PROPERTY
    INTERFACE_INCLUDE_DIRECTORIES ${RSCpp_INCLUDE_DIR}/scriptc)
else()
  message(FATAL_ERROR "Could not find RS Cpp library.")
endif()

# Assemble user defined RS script implementation
set(rs_output_folder ${CMAKE_CURRENT_SOURCE_DIR}/build/generated/renderscript_source_output_dir)
if(GradleTask MATCHES "Release")
  set(gen_rs_folder ${rs_output_folder}/release/compileReleaseRenderscript/out/)
else()
  set(gen_rs_folder ${rs_output_folder}/debug/compileDebugRenderscript/out/)
endif()

file(GLOB GEN_RS_SOURCES ${gen_rs_folder}/*.cpp)
add_library(example_rs ${GEN_RS_SOURCES})
add_library(Example::RS ALIAS example_rs)

set_target_properties(example_rs
  PROPERTIES LINKER_LANGUAGE CXX)

target_compile_options(example_rs PRIVATE -Ofast -Wall -Wextra -pedantic)

target_include_directories(example_rs PUBLIC $<BUILD_INTERFACE:${gen_rs_folder}>)

target_link_libraries(example_rs
  PUBLIC RS::Cpp
  PRIVATE Android::Log)

add_library(example src/main/cpp/example.cc)

target_include_directories(example PUBLIC $<BUILD_INTERFACE:${gen_rs_folder}>)

target_link_libraries(example PRIVATE Example::RS Android::Android Android::Log)
