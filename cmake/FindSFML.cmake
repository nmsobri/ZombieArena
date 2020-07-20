#Set list of search path
set(
        SEARCH_DIRS
        /usr/local
        /opt/local
        ./vendor/sfml
)

#Set sfml components
set(
        SFML_COMPONENTS
        System
        Audio
        Graphics
        Network
        Window
)

#Set header include dir
find_path(
        SFML_INCLUDE_DIR SFML/Config.hpp
        PATH_SUFFIXES include
        PATHS ${SEARCH_DIRS}
)

#Set library dll dir
find_path(
        SFML_LIBRARY_DIR openal32.dll
        PATH_SUFFIXES bin
        PATHS ${SEARCH_DIRS}
)

#Set library
foreach (COMPONENT ${SFML_COMPONENTS})
    string(TOUPPER ${COMPONENT} LIBRARY_PATH)
    string(TOLOWER ${COMPONENT} LIBRARY_NAME)

    #Need different variable name cause find_library cache the value,
    #so if not using unique variable name, cmake wont bother to execute
    #find_library since the value already there
    find_library(
            SFML_LIBRARY_${LIBRARY_PATH} NAMES sfml-${LIBRARY_NAME}
            PATH_SUFFIXES lib64 lib
            PATHS ${SEARCH_DIRS}
    )

    if (SFML_LIBRARY_${LIBRARY_PATH})
        list(APPEND SFML_LIBRARY ${SFML_LIBRARY_${LIBRARY_PATH}})
        list(REMOVE_DUPLICATES SFML_LIBRARY)
    endif (SFML_LIBRARY_${LIBRARY_PATH})
endforeach (COMPONENT)

set(SFML_FOUND NO)

if (SFML_LIBRARY)
    set(SFML_FOUND YES)
endif (SFML_LIBRARY)

#Handle arguements
include(FindPackageHandleStandardArgs)

#Make sure all the variable is being sent, otherwise error out using DEFAULT_MSG
find_package_handle_standard_args(SFML DEFAULT_MSG SFML_LIBRARY SFML_INCLUDE_DIR SFML_LIBRARY_DIR)

message("########################")
message(${SFML_INCLUDE_DIR})
message(${SFML_LIBRARY_DIR})
message(${SFML_LIBRARY})
message("########################")