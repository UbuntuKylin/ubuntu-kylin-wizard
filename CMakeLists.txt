project (ubuntu-kylin-wizard)
cmake_minimum_required (VERSION 2.8.9)

include (GNUInstallDirs)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set (CMAKE_CXX_FLAGS_DEBUG "-g3 -DWIZARD_DEBUG_BUILD")
set (CMAKE_CXX_FLAGS_RELEASE "")

set (SRC_LIST main.cpp
              blur.cpp
              draw.cpp
              style.cpp
              rawpixel.cpp
              EMConverter.cpp
)

set (PREFIXPATH "${CMAKE_INSTALL_PREFIX}")
set (WIZARDDATADIR "${CMAKE_INSTALL_FULL_DATADIR}/${PROJECT_NAME}")
set (PKGDATADIR "${WIZARDDATADIR}/images")
set (SOURCEDATADIR "${CMAKE_CURRENT_SOURCE_DIR}/resources")

configure_file (${CMAKE_SOURCE_DIR}/config.h.cmake ${CMAKE_BINARY_DIR}/config.h)
include_directories(${CMAKE_BINARY_DIR})

# XDG Autostart

set(
  WIZARD_XDG_AUTOSTART
  "${CMAKE_CURRENT_BINARY_DIR}/ubuntu-kylin-wizard.desktop"
)

configure_file(
  "ubuntu-kylin-wizard.desktop.in"
  ${WIZARD_XDG_AUTOSTART}
  @ONLY
)

install(
  FILES "${WIZARD_XDG_AUTOSTART}"
  DESTINATION "/etc/xdg/autostart"
)

# Resources
file (GLOB _datafiles "${CMAKE_CURRENT_SOURCE_DIR}/resources/*")
install (FILES ${_datafiles} DESTINATION ${PKGDATADIR})

find_package (PkgConfig REQUIRED)
set (WIZARD_DEPS
     cairo>=1.13.1
     gtk+-3.0>=3.1
     x11
#     xext
     xtst
)
pkg_check_modules (CACHED_WIZARD_DEPS REQUIRED ${WIZARD_DEPS})
include_directories(${CACHED_WIZARD_DEPS_INCLUDE_DIRS})
add_definitions(${CACHED_WIZARD_DEPS_CFLAGS_OTHER})
add_executable(ubuntu-kylin-wizard ${SRC_LIST})
target_link_libraries(ubuntu-kylin-wizard ${CACHED_WIZARD_DEPS_LIBRARIES})

install (TARGETS ubuntu-kylin-wizard DESTINATION "bin")
