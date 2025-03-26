cmake_minimum_required(VERSION 3.0.2)

project(cansend)

set(CMAKE_CXX_STANDARD 11)

find_package(catkin REQUIRED COMPONENTS 
roscpp
)

catkin_package(
    INCLUDE_DIRS include
    LIBRARIES cansend
    CATKIN_DEPENDS roscpp
)

find_package(Boost COMPONENTS thread)
set(boost_1_80 /tmp/temp_ws/boost_1_80_0)

include_directories (
    include
    ${catkin_INCLUDE_DIRS}
    ${Boost_LIBRARIES}
    ${boost_1_80}
    ${boost_1_80}/stage/lib
)

add_library(cansend
    include/lib.h
    include/canframelen.h
    include/cansend.h
    src/lib.cpp
    src/canframelen.cpp
    src/cansend.cpp
)

#add_executable(cansend cansend.c)
add_executable(cansend_cmd src/main.cpp)

target_link_libraries(cansend_cmd
    cansend
    ${Boost_LIBRARIES}
)

install(TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_GLOBAL_BIN_DESTINATION}
)

# Mark cpp header files for installation
install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
  FILES_MATCHING PATTERN "*.hpp"
  PATTERN ".svn" EXCLUDE
)
