cmake_minimum_required(VERSION 3.7)
project(jfelip_codebase)

set(CMAKE_CXX_STANDARD 11)

find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})

include_directories(
        include
        include/TinyGLViz
        )

add_definitions(-D_ACTIVATE_GENERIC_MESSAGES_)

set(SOURCE_FILES
        src/main.cpp
        include/GenericMessages.hpp
        include/CScopedTimer.hpp
        include/StringHelpers.hpp
        include/TinyGLViz/COpenGLVisualizer.hpp
        include/TinyGLViz/COpenGLViewport.hpp
        include/TinyGLViz/CGLGeometry.h
        include/TinyGLViz/Transform.h
        include/TinyGLViz/CDualQuaternion.hpp
        include/TinyGLViz/CQuaternion.hpp
        include/TinyGLViz/CCamera.hpp
        include/TinyGLViz/CGLLight.hpp
        include/TinyGLViz/CMaterial.hpp
        include/TinyGLViz/CGLShader.hpp
        include/TinyGLViz/primitives/CSolidBox.h
        include/TinyGLViz/primitives/CSolidCapsule.h
        include/TinyGLViz/primitives/CSolidCone.h
        include/TinyGLViz/primitives/CSolidCylinder.h
        include/TinyGLViz/primitives/CSolidSphere.h
        include/TinyGLViz/primitives/CSolidArrow.h
        include/TinyGLViz/primitives/CReferenceFrame.h
        include/TinyGLViz/primitives/CSolidLine.h
        include/TinyGLViz/primitives/CParticleSystem.h
        include/TinyGLViz/CVector3.hpp
        include/TinyGLViz/CGLPrimitive.hpp
        include/TinyGLViz/Types.h
        include/TinyGLViz/CTextDrawer.hpp)

add_executable(jfelip_codebase ${SOURCE_FILES})

target_link_libraries(jfelip_codebase GLEW GL glfw ${FREETYPE_LIBRARIES})
