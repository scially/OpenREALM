cmake_minimum_required(VERSION 2.8.3)
project(realm_io)

find_package(catkin REQUIRED COMPONENTS
        realm_core
        )
# Fix to avoid OpenCV package confusion with ROS melodic
find_package(OpenCV 3.2 EXACT)
if (NOT OpenCV_FOUND)
    find_package(OpenCV 3)
endif()
find_package(cmake_modules REQUIRED)

# Fix to find Exiv2, otherwise it really causes some nerves
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
find_package(Exiv2 REQUIRED)
find_package(GDAL REQUIRED)
find_package(PCL 1.7 REQUIRED)
find_package(Boost REQUIRED COMPONENTS
        filesystem
        )

####################
## Catkin Package ##
####################

catkin_package(
        INCLUDE_DIRS
                include
                ${PCL_INCLUDE_DIRS}
        LIBRARIES
                ${PROJECT_NAME}
        CATKIN_DEPENDS
                realm_core
                cmake_modules
        DEPENDS
                OpenCV
                Exiv2
                GDAL
                PCL
                Boost
)

###################
## Build Library ##
###################

include_directories(
        include
        ${OpenCV_INCLUDE_DIRS}
        ${catkin_INCLUDE_DIRS}
        ${PCL_INCLUDE_DIRS}
        ${PCL_LIBRARY_DIRS}
        ${GDAL_INCLUDE_DIR}
        ${Exiv2_INCLUDE_DIRS}
        ${Boost_INCLUDE_DIR}
        ${cmake_modules_INCLUDE_DIR}
        ${realm_core_INCLUDE_DIR}
        ${realm_core_INCLUDE_DIRS}
)
add_library(${PROJECT_NAME} SHARED
        src/realm_io_lib/exif_export.cpp
        src/realm_io_lib/exif_import.cpp
        src/realm_io_lib/gis_export.cpp
        src/realm_io_lib/pcl_export.cpp
        src/realm_io_lib/cv_export.cpp
        src/realm_io_lib/realm_import.cpp
        src/realm_io_lib/realm_export.cpp
        src/realm_io_lib/utilities.cpp
        )
target_link_libraries(${PROJECT_NAME}
        ${catkin_LIBRARIES}
        ${OpenCV_LIBRARIES}
        ${PCL_COMMON_LIBRARIES}
        ${PCL_IO_LIBRARIES}
        ${GDAL_LIBRARY}
        ${Exiv2_LIBRARIES}
        ${Boost_LIBRARIES}
        ${cmake_modules_LIBRARIES}
        -lexiv2
        )

add_definitions(
        -std=c++11
        ${PCL_DEFINITIONS}
        )

#####################
## Install Library ##
#####################

# Mark executables and/or libraries for installation
install(
        TARGETS ${PROJECT_NAME}
        ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
        LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
        RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

# Mark cpp header files for installation
install(
        DIRECTORY include/${PROJECT_NAME}/
        DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
        FILES_MATCHING PATTERN "*.h"
)
