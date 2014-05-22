##############################
#wxFaceClient
##############################
project(wxFaceClient CXX)

#XFace
set(XFace_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/XFace/include)

#XSmilAgent
set(XSmilAgent_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/XSmilAgent)

#wxWidgets
set(wxWidgets_USE_LIBS base core gl net)
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})

set(wxfaceclient_SRC
 ConnectionDlg.cpp
 ControlPanel.cpp
 DropScriptTarget.cpp
 FaceApp.cpp
 FaceFrame.cpp
 TaskSender.cpp
 toggle.cpp
)

include_directories(${XSmilAgent_INCLUDE_DIR} ${XFace_INCLUDE_DIR} ${wxWidgets_INCLUDE_DIRS})
add_executable(wxfaceclient ${wxfaceclient_SRC})
target_link_libraries(wxfaceclient XSmilAgent XFace ${wxWidgets_LIBRARIES})

