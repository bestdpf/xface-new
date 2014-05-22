#####################
#XSmilAgent Library
#####################

project(XSmilAgent CXX)

#check xerces dependencies
find_package(XercesC REQUIRED)

#check wxWidgets dependencies
set(wxWidgets_USE_LIBS base)
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})

set(libXSmilAgent_SRCS 
 ScriptProcessorLoader.cpp
 SGNode.cpp
 SMILManager.cpp
 SMILObjects.cpp
 SMILProcessor.cpp
 TTSEngineMaker.cpp
 VisitorAnimCreator.cpp
 VisitorInfoPrinter.cpp
 VisitorSAPISpeech.cpp
 VisitorSpeech.cpp
 VisitorTiming.cpp
 wxScriptProcessor.cpp
 XercesString.cpp
)

if(WIN32)
 set(libXSmilAgent_SRCS Wave.cpp XSAPI.cpp ${libXSmilAgent_SRCS})
else(WIN32)
 set(libXSmilAgent_SRCS Wave2.cpp ${libXSmilAgent_SRCS})
endif(WIN32)

#Non Enabled
#ItalianFlite.cpp #Circular dependency upon XFace!

include_directories(${wxWidgets_INCLUDE_DIRS} ${XERCESC_INCLUDE_DIR})
add_library(XSmilAgent ${libXSmilAgent_SRCS})
target_link_libraries(XSmilAgent ${XERCECS_LIBRARIES})