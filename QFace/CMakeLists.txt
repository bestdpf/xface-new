##############################
#QFace
##############################
project(QFace CXX)

#Check dependencies:
#wxWidgets
find_package(Qt4 COMPONENTS QtCore QtGui QtOpenGL REQUIRED)
include(${QT_USE_FILE})

#SDL
find_package(SDL REQUIRED)
find_package(OpenGL REQUIRED)


#XSmilAgent
set(XSmilAgent_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/XSmilAgent)

#XFace Library
set(XFace_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/XFace/include)

###############wxXFacePlayer library
set(QFace_SRC
    QFace.cpp
)
set(QFace_HEADERS
    QFace.h
)
include_directories(${SDL_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${QT_QTOPENGL_INCLUDE_DIRS} ${XFace_INCLUDE_DIR} ${XSmilAgent_INCLUDE_DIR})
#link_directories()
add_executable(QFace ${QFace_SRC})
target_link_libraries(QFace XSmilAgent XFace ${QT_LIBRARIES} ${SDL_LIBRARY})
