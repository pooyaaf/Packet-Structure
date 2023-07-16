cmake_minimum_required(VERSION 3.10)
project(PacketStructure)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE HFiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
                                  ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
                                  ${CMAKE_CURRENT_SOURCE_DIR}/include/UDP/*.h
                                  ${CMAKE_CURRENT_SOURCE_DIR}/include/TCP/*.h)
file(GLOB_RECURSE CppFiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/client/*.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/server/*.cpp)
file(GLOB_RECURSE JsonFiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
                                      ${CMAKE_CURRENT_SOURCE_DIR}/resources/*.txt)


# Add main source file
set(MAIN_SOURCES
    src/main.cpp
    src/client/UDPClient.cpp
    src/server/UDPServer.cpp
)
# Create main executable
add_executable(PacketStructure ${MAIN_SOURCES} ${HFiles} ${CppFiles} ${JsonFiles})

# Set the output directory for the executables
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/output)
