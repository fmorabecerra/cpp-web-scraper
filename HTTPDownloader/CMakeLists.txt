# add the library that runs
add_library(HTTPDownloader HTTPDownloader.cpp)

# state that anybody linking to us needs to include the current source dir
# to find MathFunctions.h, while we don't.
target_include_directories(HTTPDownloader
                           INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
                           )


# CURL_INCLUDE_DIRS   - where to find curl/curl.h, etc.
# CURL_LIBRARIES      - List of libraries when using curl.
# CURL_FOUND          - True if curl found.
# CURL_VERSION_STRING - the version of curl found (since CMake 2.8.8)
set(CURL_LIBRARY "-lcurl") 
find_package(CURL REQUIRED) 
include_directories(${CURL_INCLUDE_DIR})
target_link_libraries(HTTPDownloader PUBLIC ${CURL_LIBRARIES})


# # Build working but runtime errors.
# set(TIDY_LIBRARY "-ltidy")
# set(TIDY_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tidy-html5/include")
# include_directories(${TIDY_INCLUDE_DIR})
# set(TIDY_LIBRARIES "/usr/lib/libtidy.so.5")
# target_link_libraries(HTTPDownloader PUBLIC ${TIDY_LIBRARIES})


# Need to add module path. Otherwise cmake will not be able to find Tidy
LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
# Tidy can't find the following two.
# set(TIDY_LIBRARY "-ltidy")
find_package(Tidy REQUIRED)
# set(TIDY_INCLUDE_DIR "usr/include")
include_directories(${TIDY_INCLUDE_DIR})
target_link_libraries(HTTPDownloader PUBLIC ${TIDY_LIBRARIES})