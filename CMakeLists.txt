cmake_minimum_required(VERSION 2.8)

list(APPEND CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_LIST_DIR}/cmake/Modules
    ${CMAKE_CURRENT_LIST_DIR}/share/doc/quickbook/cmake/Modules
)
set(CMAKE_USER_MAKE_RULES_OVERRIDE ${CMAKE_CURRENT_LIST_DIR}/cmake/cmake-tools/DefaultBuildFlags.cmake)

project(Versionning NONE)

# Set project version
include(version.cmake)

#############
#  Sources  #
#############

set(PROJECT_INCLUDE_DIR
    ${PROJECT_SOURCE_DIR}/include
)

set(${PROJECT_NAME}_INCLUDE_DIR ${PROJECT_INCLUDE_DIR} CACHE INTERNAL "")

set(PROJECT_FILES
    ${PROJECT_INCLUDE_DIR}/${PROJECT_NAME}/Version.h
    ${PROJECT_INCLUDE_DIR}/${PROJECT_NAME}/VersionException.h
)

add_custom_target(
    ${PROJECT_NAME}
    COMMAND ""
    SOURCES ${PROJECT_FILES}
)

######################
#  Global variables  #
######################

# Set install path
include(GNUInstallDirs)
set(CMAKE_INSTALL_CMAKEDIR ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)
set(CMAKE_INSTALL_FULL_CMAKEDIR ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_CMAKEDIR})

# Build output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(${PROJECT_NAME}_PACKAGING_ENABLED TRUE CACHE INTERNAL "")
else()
    set(${PROJECT_NAME}_PACKAGING_ENABLED FALSE CACHE INTERNAL "")
endif()

#############
#  Testing  #
#############

enable_testing()

####################
#  Subdirectories  #
####################

add_subdirectory(share)
add_subdirectory(examples)
add_subdirectory(tests)

###############
#  Packaging  #
###############

# Following code is enabled only if Versionning is the top level project
if(${PROJECT_NAME}_PACKAGING_ENABLED)

    # Package include directory
    install(DIRECTORY
        ${PROJECT_INCLUDE_DIR}/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        COMPONENT dev
        PATTERN "*~" EXCLUDE
    )

    ###########
    #  CPack  #
    ###########

    find_package(Git)
    if(GIT_FOUND)
        execute_process(
            COMMAND ${GIT_EXECUTABLE} describe --always --dirty
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE ${PROJECT_NAME}_VERSION_STRING
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        string(FIND "${${PROJECT_NAME}_VERSION_STRING}" "${${PROJECT_NAME}_VERSION}" substring_found)
        if(substring_found EQUAL -1)
            message(FATAL_ERROR "Version mismatch: ${${PROJECT_NAME}_VERSION_STRING} != ${${PROJECT_NAME}_VERSION}")
        endif()
    else()
        set(${PROJECT_NAME}_VERSION_STRING "v${${PROJECT_NAME}_VERSION}")
    endif()

    if(WIN32)
        set(CPACK_GENERATOR "ZIP")
        set(CPACK_SOURCE_GENERATOR "ZIP")
    elseif(UNIX)
        set(CPACK_GENERATOR "TGZ")
        set(CPACK_SOURCE_GENERATOR "TGZ")
    endif()
    set(CPACK_COMPONENTS_ALL dev doc test)
    set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Simple portable C++ data structure to store versionning information of a project.")
    set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE_1_0.txt")
    set(CPACK_PACKAGE_VERSION_MAJOR ${${PROJECT_NAME}_MAJOR})
    set(CPACK_PACKAGE_VERSION_MINOR ${${PROJECT_NAME}_MINOR})
    set(CPACK_PACKAGE_VERSION_PATCH ${${PROJECT_NAME}_PATCH})
    set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}_${${PROJECT_NAME}_VERSION_STRING}")
    set(CPACK_SOURCE_PACKAGE_FILE_NAME "${PROJECT_NAME}_${${PROJECT_NAME}_VERSION_STRING}-src")
    set(CPACK_SOURCE_IGNORE_FILES "/build/;/install/;CMakeLists.txt.user;.git;.gitignore;.gitmodules;~$")

    include(CPack)

endif()

mark_as_advanced(
    CMAKE_CODEBLOCKS_EXECUTABLE
    CMAKE_INSTALL_PREFIX
)
