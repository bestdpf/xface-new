##############################
#wxFacePlayer
##############################
project(wxFacePlayer CXX)

#Check dependencies:
#wxWidgets
set(wxWidgets_USE_LIBS base core gl net)
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})

#SDL
find_package(SDL REQUIRED)

#XSmilAgent
set(XSmilAgent_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/XSmilAgent)

#XFace Library
set(XFace_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/XFace/include)

###############wxXFacePlayer library
set(wxXFacePlayer_SRC
 toggle.cpp
 SDLTimer.cpp
 SDLSound.cpp
 DicTraverser.cpp
 wxFaceTimer.cpp
)
include_directories(${SDL_INCLUDE_DIR} ${wxWidgets_INCLUDE_DIRS} ${XFace_INCLUDE_DIR} ${XSmilAgent_INCLUDE_DIR})
#link_directories()
add_library(wxXFacePlayer ${wxXFacePlayer_SRC})
target_link_libraries(wxXFacePlayer XSmilAgent XFace ${wxWidgets_LIBRARIES} ${SDL_LIBRARY})

set(wxfaceplayer_SRC
 FaceView.cpp
 ServerThread.cpp
 wxFace.cpp
 wxFacePlayerFrm.cpp
 ConnPortSetting.cpp
 DropFAPFileTarget.cpp
 NotificationSenderThread.cpp
 wxFacePlayerApp.cpp
)
#AVIGenerator.cpp
add_executable(wxfaceplayer ${wxfaceplayer_SRC})
target_link_libraries(wxfaceplayer wxXFacePlayer XFace)
