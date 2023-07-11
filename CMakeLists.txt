cmake_minimum_required(VERSION 3.10) 
project(PacketStructure) 

set(CMAKE_CXX_STANDARD 17) 

set(main1 
    main1.cpp 

) 

# # Add server source files 
# set(UDP_SERVER_SOURCES 
#     udp_server.cpp 

# ) 

# # Add client source files 
# set(UDP_CLIENT_SOURCES 
#     udp_client.cpp 
  
# ) 

#set(TXT_FILES 
#resources/Packet.txt 
#Packet.txt
#) 

add_executable(main1 ${main1} ) 


# # Create server executables 
# add_executable(udp_server ${UDP_SERVER_SOURCES} ) 

# # Create client executables
# add_executable(udp_client ${UDP_CLIENT_SOURCES} ) 



# Set the output directory for the executables
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/output)