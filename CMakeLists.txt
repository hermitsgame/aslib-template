cmake_minimum_required (VERSION 3.16)

project (hello
  VERSION 0.0.1
  LANGUAGES CXX	
)

include (CMakeDependentOption)
include (CMakePackageConfigHelpers)
include (CMakePushCheckState)
include (CPack)
include (CTest)
include (GNUInstallDirs)

set (PROJECT hello)

set (CPACK_PACKAGE_NAME "lib${PROJECT}-dev")
set (CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set (CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set (CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set (CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set (CPACK_GENERATOR "DEB")

list (APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set (_internal_CMake_BINDIR ${CMAKE_INSTALL_BINDIR})
set (_internal_CMake_INCLUDE_DIR ${CMAKE_INSTALL_INCLUDEDIR})
set (_internal_CMake_LIBDIR ${CMAKE_INSTALL_LIBDIR})
set (_internal_CMake_INSTALLDIR ${_internal_CMake_LIBDIR}/cmake/${PROJECT})

option (WITH_PKGCONFIG "Enable pkg-config support" ON)

if (WITH_PKGCONFIG)
  set (VERSION ${PROJECT_VERSION})
  set (prefix ${CMAKE_INSTALL_PREFIX})
  set (exec_prefix ${CMAKE_INSTALL_FULL_BINDIR})
  set (libdir ${CMAKE_INSTALL_FULL_LIBDIR})
  set (includedir ${CMAKE_INSTALL_FULL_INCLUDEDIR})

  configure_file (
    "${PROJECT_SOURCE_DIR}/lib${PROJECT}.pc.in"
    "${PROJECT_BINARY_DIR}/lib${PROJECT}.pc"
    @ONLY
  )

  unset (VERSION)
  unset (prefix)
  unset (exec_prefix)
  unset (libdir)
  unset (includedir)
endif (WITH_PKGCONFIG)

# src
find_package(glog REQUIRED)

set (PUBLIC_H
  src/hello.h
)

set (SRCS
  ${PUBLIC_H}
  src/hello.cc
)

set (DYNAMIC_TARGET ${PROJECT})
set (STATIC_TARGET ${PROJECT}_static)

add_library (${DYNAMIC_TARGET} SHARED ${SRCS})
add_library (${STATIC_TARGET} STATIC ${SRCS})
set_target_properties (${STATIC_TARGET} PROPERTIES OUTPUT_NAME ${PROJECT})
set_target_properties (${DYNAMIC_TARGET} PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION 1)
set_target_properties (${DYNAMIC_TARGET} PROPERTIES PUBLIC_HEADER "${PUBLIC_H}")

configure_package_config_file (${PROJECT}-config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT}-config.cmake
  INSTALL_DESTINATION ${_internal_CMake_INSTALLDIR}
  NO_CHECK_REQUIRED_COMPONENTS_MACRO)

write_basic_package_version_file (
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT}-config-version.cmake
  COMPATIBILITY SameMajorVersion)

export (TARGETS ${DYNAMIC_TARGET} FILE ${PROJECT}-targets.cmake)
export (PACKAGE ${PROJECT})

install (TARGETS ${DYNAMIC_TARGET} ${STATIC_TARGET}
  EXPORT ${PROJECT}-targets
  PUBLIC_HEADER DESTINATION ${_internal_CMake_INCLUDE_DIR}/${PROJECT}
  LIBRARY DESTINATION ${_internal_CMake_LIBDIR}
  ARCHIVE DESTINATION ${_internal_CMake_LIBDIR})

install (FILES
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT}-config.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT}-config-version.cmake
  DESTINATION ${_internal_CMake_INSTALLDIR})

if (WITH_PKGCONFIG)
  install (
    FILES "${PROJECT_BINARY_DIR}/lib${PROJECT}.pc"
    DESTINATION "${_internal_CMake_LIBDIR}/pkgconfig"
  )
endif (WITH_PKGCONFIG)

install (EXPORT ${PROJECT}-targets DESTINATION ${_internal_CMake_INSTALLDIR})

# test
find_package(GTest REQUIRED)

add_executable(hello_test
  test/hello_test.cc
  test/vec_test.cc)

target_link_libraries (hello_test ${GTEST_BOTH_LIBRARIES})
target_compile_options (hello_test PUBLIC ${GTEST_CFLAGS})

add_test (mytest hello_test)
enable_testing ()

