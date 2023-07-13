cmake_minimum_required(VERSION 3.10) 
project(PacketStructure) 

set(CMAKE_CXX_STANDARD 17) 

# set(main1 
#     main1.cpp 

# ) 

# Add server source files 
set(UDP_SERVER_SOURCES 
    src/server/UDPServer.cpp 

) 
set(TCP_SERVER_SOURCES 
    src/server/TCPServer.cpp 

) 

# Add client source files 
set(UDP_CLIENT_SOURCES 
    src/client/UDPClient.cpp 
  
) 
set(TCP_CLIENT_SOURCES 
    src/client/TCPClient.cpp 
  
) 

#set(TXT_FILES 
#resources/Packet.txt 
#Packet.txt
#) 

# add_executable(main1 ${main1} ) 


# # Create server executables 
add_executable(UDPServer ${UDP_SERVER_SOURCES} ) 
add_executable(TCPServer ${TCP_SERVER_SOURCES} ) 
# # Create client executables
add_executable(UDPClient ${UDP_CLIENT_SOURCES} ) 
add_executable(TCPClient ${TCP_CLIENT_SOURCES} ) 


# Set the output directory for the executables
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/output)