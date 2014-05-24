project(XFace)


cmake_minimum_required(VERSION 2.8)
#where to look for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

set(CMAKE_CXXFLAGS_DEBUG " -O0 -g -ggdb")
set(CMAKE_CXXFLAGS_Release " -O2")
set(CMAKE_BUILD_TYPE "Release")
subdirs(XSmilAgent XFace wxFacePlayer QFace wxFaceEd wxFaceClient)
