cmake_minimum_required(VERSION 3.0.0)
project(web-scraper VERSION 0.1.0)

include(CTest)
enable_testing()

# add the MathFunctions library
# add_subdirectory(HTTPDownloader)

add_executable(web-scraper main.cpp)
# target_link_libraries(web-scraper PUBLIC HTTPDownloader)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)


# CURL_INCLUDE_DIRS   - where to find curl/curl.h, etc.
# CURL_LIBRARIES      - List of libraries when using curl.
# CURL_FOUND          - True if curl found.
# CURL_VERSION_STRING - the version of curl found (since CMake 2.8.8)
set(CURL_LIBRARY "-lcurl") 
find_package(CURL REQUIRED) 
include_directories(${CURL_INCLUDE_DIR})
target_link_libraries(web-scraper PUBLIC ${CURL_LIBRARIES})

# Need to add module path. Otherwise cmake will not be able to find Tidy
LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/modules/")
find_package(CURLpp REQUIRED)
include_directories(${CURLPP_INCLUDE_DIR})
target_link_libraries(web-scraper PUBLIC ${CURLPP_LIBRARIES})

find_package(libxml++ REQUIRED)
include_directories(${libxml++_INCLUDE_DIR})
include_directories("/usr/lib/x86_64-linux-gnu/libxml++-2.6/include")
include_directories("/usr/include/libxml2")
target_link_libraries(web-scraper PUBLIC ${libxml++_LIBRARIES})
include_directories(${glibmm_INCLUDE_DIR})
include_directories("/usr/lib/x86_64-linux-gnu/glibmm-2.4/include")
target_link_libraries(web-scraper PUBLIC ${glibmm_LIBRARIES})
include_directories(${glib_INCLUDE_DIR})
include_directories("/usr/lib/x86_64-linux-gnu/glib-2.0/include/")

find_package(PahoMqttC REQUIRED)
target_link_libraries(web-scraper PUBLIC ${PAHO_MQTT_C_LIBRARIES})
target_link_libraries(web-scraper PUBLIC "/usr/local/lib/libpaho-mqttpp3.so")