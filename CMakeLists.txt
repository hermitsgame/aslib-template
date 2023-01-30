cmake_minimum_required (VERSION 3.16)

project (hello
  VERSION 0.0.1
  LANGUAGES CXX	
)

include (CMakeDependentOption)
include (CMakePackageConfigHelpers)
include (CMakePushCheckState)
include (CTest)
include (GNUInstallDirs)

set (CPACK_PACKAGE_NAME "lib${PROJECT_NAME}-dev")
set (CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set (CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set (CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set (CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set (CPACK_GENERATOR "DEB")
set (CPACK_DEBIAN_PACKAGE_NAME ${CPACK_PACKAGE_NAME})
set (CPACK_DEBIAN_PACKAGE_MAINTAINER "Ted")
set (CPACK_DEBIAN_PACKAGE_VERSION ${PROJECT_VERSION})
set (CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
set (CPACK_PACKAGE_FILE_NAME "${CPACK_DEBIAN_PACKAGE_NAME}-${CPACK_DEBIAN_PACKAGE_VERSION}-${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")

include (CPack)

list (APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set (_internal_CMake_BINDIR ${CMAKE_INSTALL_BINDIR})
set (_internal_CMake_INCLUDE_DIR ${CMAKE_INSTALL_INCLUDEDIR})
set (_internal_CMake_LIBDIR ${CMAKE_INSTALL_LIBDIR})
set (_internal_CMake_INSTALLDIR ${_internal_CMake_LIBDIR}/cmake/${PROJECT_NAME})

option (WITH_PKGCONFIG "Enable pkg-config support" ON)

if (WITH_PKGCONFIG)
  set (VERSION ${PROJECT_VERSION})
  set (prefix ${CMAKE_INSTALL_PREFIX})
  set (exec_prefix ${CMAKE_INSTALL_FULL_BINDIR})
  set (libdir ${CMAKE_INSTALL_FULL_LIBDIR})
  set (includedir ${CMAKE_INSTALL_FULL_INCLUDEDIR})

  configure_file (
    "${PROJECT_SOURCE_DIR}/lib${PROJECT_NAME}.pc.in"
    "${PROJECT_BINARY_DIR}/lib${PROJECT_NAME}.pc"
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

set (DYNAMIC_TARGET ${PROJECT_NAME})
set (STATIC_TARGET ${PROJECT_NAME}_static)

add_library (${DYNAMIC_TARGET} SHARED ${SRCS})
add_library (${STATIC_TARGET} STATIC ${SRCS})
set_target_properties (${STATIC_TARGET} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
set_target_properties (${DYNAMIC_TARGET} PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION 1)
set_target_properties (${DYNAMIC_TARGET} PROPERTIES PUBLIC_HEADER "${PUBLIC_H}")

configure_package_config_file (${PROJECT_NAME}-config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
  INSTALL_DESTINATION ${_internal_CMake_INSTALLDIR}
  NO_CHECK_REQUIRED_COMPONENTS_MACRO)

write_basic_package_version_file (
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
  COMPATIBILITY SameMajorVersion)

export (TARGETS ${DYNAMIC_TARGET} FILE ${PROJECT_NAME}-targets.cmake)
export (PACKAGE ${PROJECT_NAME})

install (TARGETS ${DYNAMIC_TARGET} ${STATIC_TARGET}
  EXPORT ${PROJECT_NAME}-targets
  PUBLIC_HEADER DESTINATION ${_internal_CMake_INCLUDE_DIR}/${PROJECT_NAME}
  LIBRARY DESTINATION ${_internal_CMake_LIBDIR}
  ARCHIVE DESTINATION ${_internal_CMake_LIBDIR})

install (FILES
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
  DESTINATION ${_internal_CMake_INSTALLDIR})

if (WITH_PKGCONFIG)
  install (
    FILES "${PROJECT_BINARY_DIR}/lib${PROJECT_NAME}.pc"
    DESTINATION "${_internal_CMake_LIBDIR}/pkgconfig"
  )
endif (WITH_PKGCONFIG)

install (EXPORT ${PROJECT_NAME}-targets DESTINATION ${_internal_CMake_INSTALLDIR})

# test
find_package(GTest REQUIRED)

add_executable(hello_test
  test/hello_test.cc
  test/vec_test.cc)

target_link_libraries (hello_test ${GTEST_BOTH_LIBRARIES})
target_compile_options (hello_test PUBLIC ${GTEST_CFLAGS})

add_test (mytest hello_test)
enable_testing ()

