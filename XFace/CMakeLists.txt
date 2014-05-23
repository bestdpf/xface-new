#############################
#XFace Library
#############################
project(XFace CXX)

#Check XSmilAgent dependencies
set(XSMILAGENT_INCLUDES ${CMAKE_SOURCE_DIR}/XSmilAgent)

#Check Xerces dependencies
find_package(XercesC REQUIRED)

#Check GL dependencies
find_package(OpenGL REQUIRED)
find_package(GLU REQUIRED)

set(libXFace_SRCS
 src/XMath/Matrix4.cpp
 src/XMath/Perlin.cpp
 src/XMath/Quaternion.cpp
 src/XMath/raytri.cpp
 src/XMath/XMath.cpp
 src/XEngine/AuxXFace.cpp
 src/XEngine/BinaryModelBatchLoader.cpp
 src/XEngine/Drawable.cpp
 src/XEngine/DeformableGeometry.cpp
 src/XEngine/Entity.cpp
 src/XEngine/Geometry.cpp
 src/XEngine/GLPrerrequisites.cpp
 src/XEngine/IndexedFaceSet.cpp
 src/XEngine/KeyframeInterpolator.cpp
 src/XEngine/MeshManager.cpp
 src/XEngine/ModelFileFactory.cpp
 src/XEngine/MorphBlender.cpp
 src/XEngine/MorphChannel.cpp
 src/XEngine/MorphController.cpp
 src/XEngine/OBJLoader.cpp
 src/XEngine/RendererGL.cpp
 src/XEngine/RenderList.cpp
 src/XEngine/RenderManager.cpp
 src/XEngine/TextureLoaderGL.cpp
 src/XEngine/TextureManager.cpp
 src/XEngine/TgaFile.cpp
 src/XEngine/Transform.cpp
 src/XEngine/VRML1Loader.cpp
 src/XEngine/VRML97Loader.cpp
 src/XFace/AnimProcessor.cpp
 src/XFace/ExpressionChannel.cpp
 src/XFace/EyeChannel.cpp
 src/XFace/FaceBase.cpp
 src/XFace/FaceEntity.cpp
 src/XFace/FapFile.cpp
 src/XFace/FDP.cpp
 src/XFace/FDPItem.cpp
 src/XFace/FDPLoader.cpp
 src/XFace/FDPWriter.cpp
 src/XFace/HeadChannel.cpp
 src/XFace/InfluenceCalculatorMaker.cpp
 src/XFace/PHOLoader.cpp
 src/XFace/PhonemeDictionary.cpp
 src/XFace/RaisedCosInfluence.cpp
 src/XFace/XercesString.cpp
 src/XFaceApp/ApplicationBase.cpp
 src/XFaceApp/ModelCamera.cpp
 src/XFaceApp/Notification.cpp
 src/XFaceApp/Task.cpp
 src/XFaceApp/TaskHandlerBase.cpp
 src/XFaceApp/XMLUtils.cpp
)

#Non-enabled files ?
# src/XFace/FDPFile.cpp

#platform dependent files 
set(libXFace_SRCS ${libXFace_SRCS} src/XEngine/BmpFile.cpp)

include_directories(${XFace_SOURCE_DIR}/include ${XSMILAGENT_INCLUDES} ${XERCESC_INCLUDE_DIR} ${GLU_INCLUDE_DIR})
add_library(XFace ${libXFace_SRCS})
target_link_libraries(XFace ${XERCESC_LIBRARIES} ${OpenGL_LIBRARY} ${GLU_LIBRARY})
