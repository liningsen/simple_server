set(INN_INC ${PROJECT_SOURCE_DIR}/include)
set(EXT_INC ${PROJECT_SOURCE_DIR}/external/usr/local/include)
include_directories(${INN_INC} ${EXT_INC})
message("INC_PATH: ${INN_INC} | ${EXT_INC}")
set(EXT_LIB ${PROJECT_SOURCE_DIR}/external/usr/local/lib)
link_directories(${EXT_LIB})
message("LIB_PATH: ${EXT_LIB}")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_executable(CliTest testClient.cpp)
add_executable(SrvTest SimpleServer.cpp LogWrapper.cpp AsyncChannelEx.cpp SrvHandler.cpp)
target_link_libraries(CliTest PocoFoundationd PocoNetd)
target_link_libraries(SrvTest PocoFoundationd PocoUtild PocoXMLd PocoJSONd PocoNetd)

