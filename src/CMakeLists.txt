
find_package(glog REQUIRED)

aux_source_directory (. SRC_LIST)
add_library (hello SHARED ${SRC_LIST})

add_library (hello_static STATIC ${SRC_LIST})
set_target_properties (hello_static PROPERTIES OUTPUT_NAME "hello")

set (VERSION "${_VERSION_MAJOR}.${_VERSION_MINOR}.${_VERSION_PATCH}")

set_target_properties (hello PROPERTIES VERSION ${VERSION} SOVERSION 1)

install (TARGETS hello hello_static LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
install (FILES hello.h DESTINATION include/hello)

