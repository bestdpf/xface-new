###############################
#wxFaceEd
###############################

project(wxFaceEd CXX)

#Check XSmilAgent dependencies
set(XSMILAGENT_INCLUDES ${CMAKE_SOURCE_DIR}/XSmilAgent)

#Check XFace library dependencies
set(XFACE_INCLUDES ${CMAKE_SOURCE_DIR}/XFace/include)

#Check wxXFacePlayer dependencies
set(WXXFACEPLAYER_INCLUDES ${CMAKE_SOURCE_DIR}/wxFacePlayer)

#check wxWidgets dependencies
set(wxWidgets_USE_LIBS base core gl adv html)
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})

set(wxFaceEd_SRCS
 ConfPanel.cpp
 DictionaryFilesDlg.cpp
 
 DropFDPFileTarget.cpp
 FaceApp.cpp
 FaceFrame.cpp
 FaceView.cpp
 FAPUPanel.cpp
 FDPPanel.cpp
 FDPPicture.cpp
 KeyframeAddDlg.cpp
# KeyframePanel.cpp
 Mediator.cpp
 ModelViewerCamera.cpp
 ModifyDictionaryFileDlg.cpp
 Panel.cpp
 PhonDictOptions.cpp
 PreviewKeyframeAddDlg.cpp
 PreviewPanel.cpp
 RubberBandBox.cpp
 Splitter.cpp
 VertexSelector.cpp
 wxFaceEd.cpp
 wxNumberEditCtrl.cpp
)

include_directories(${XFACE_INCLUDES} ${XSMILAGENT_INCLUDES} ${wxWidgets_INCLUDE_DIRS})
add_executable(wxFaceEd ${wxFaceEd_SRCS})
target_link_libraries(wxFaceEd ${wxWidgets_LIBRARIES} wxXFacePlayer XFace XSmilAgent)
