cmake_minimum_required (VERSION 3.16)

project (HELLOLIB
  VERSION 0.0.1
  LANGUAGES CXX	
)

set (CPACK_PACKAGE_NAME "libhello-dev")
set (CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set (CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set (CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set (CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set (CPACK_GENERATOR "DEB")

list (APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set (_hello_CMake_BINDIR ${CMAKE_INSTALL_BINDIR})
set (_hello_CMake_INCLUDE_DIR ${CMAKE_INSTALL_INCLUDEDIR})
set (_hello_CMake_LIBDIR ${CMAKE_INSTALL_LIBDIR})
set (_hello_CMake_INSTALLDIR ${_hello_CMake_LIBDIR}/cmake/hello)

include (CMakeDependentOption)
include (CMakePackageConfigHelpers)
include (CMakePushCheckState)
include (CPack)
include (CTest)
include (GNUInstallDirs)

option (WITH_PKGCONFIG "Enable pkg-config support" ON)

if (WITH_PKGCONFIG)
  set (VERSION ${PROJECT_VERSION})
  set (prefix ${CMAKE_INSTALL_PREFIX})
  set (exec_prefix ${CMAKE_INSTALL_FULL_BINDIR})
  set (libdir ${CMAKE_INSTALL_FULL_LIBDIR})
  set (includedir ${CMAKE_INSTALL_FULL_INCLUDEDIR})

  configure_file (
    "${PROJECT_SOURCE_DIR}/libhello.pc.in"
    "${PROJECT_BINARY_DIR}/libhello.pc"
    @ONLY
  )

  unset (VERSION)
  unset (prefix)
  unset (exec_prefix)
  unset (libdir)
  unset (includedir)
endif (WITH_PKGCONFIG)

configure_package_config_file (hello-config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/hello-config.cmake
  INSTALL_DESTINATION ${_hello_CMake_INSTALLDIR}
  NO_CHECK_REQUIRED_COMPONENTS_MACRO)

write_basic_package_version_file (
  ${CMAKE_CURRENT_BINARY_DIR}/hello-config-version.cmake
  COMPATIBILITY SameMajorVersion)

# src
find_package(glog REQUIRED)

set (PUBLIC_H
  src/hello.h
)

set (SRCS
  ${PUBLIC_H}
  src/hello.cc
)
add_library (hello SHARED ${SRCS})
add_library (hello_static STATIC ${SRCS})
set_target_properties (hello_static PROPERTIES OUTPUT_NAME "hello")
set_target_properties (hello PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION 1)
set_target_properties (hello PROPERTIES PUBLIC_HEADER "${PUBLIC_H}")

install (TARGETS hello hello_static 
  PUBLIC_HEADER DESTINATION ${_hello_CMake_INCLUDE_DIR}/hello
  LIBRARY DESTINATION ${_hello_CMake_LIBDIR}
  ARCHIVE DESTINATION ${_hello_CMake_LIBDIR})

install (FILES
  ${CMAKE_CURRENT_BINARY_DIR}/hello-config.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/hello-config-version.cmake
  DESTINATION ${_hello_CMake_INSTALLDIR})

if (WITH_PKGCONFIG)
  install (
    FILES "${PROJECT_BINARY_DIR}/libhello.pc"
    DESTINATION "${_hello_CMake_LIBDIR}/pkgconfig"
  )
endif (WITH_PKGCONFIG)

# test
find_package(GTest REQUIRED)

add_executable(hello_test
  test/hello_test.cc
  test/vec_test.cc)

target_link_libraries (hello_test ${GTEST_BOTH_LIBRARIES})
target_compile_options (hello_test PUBLIC ${GTEST_CFLAGS})

add_test (mytest hello_test)
enable_testing ()

