set (SRC_LIST main.cpp
              blur.cpp
              EMConverter.cpp
              Page.cpp
              RawPixel.cpp
              Wizard.cpp
              WizardSettings.cpp
)

include_directories(${CACHED_WIZARD_DEPS_INCLUDE_DIRS})
add_definitions(${CACHED_WIZARD_DEPS_CFLAGS_OTHER})
add_executable(ubuntu-kylin-wizard ${SRC_LIST})
target_link_libraries(ubuntu-kylin-wizard ${CACHED_WIZARD_DEPS_LIBRARIES})

install (TARGETS ubuntu-kylin-wizard DESTINATION "bin")
