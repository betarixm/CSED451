cmake_minimum_required(VERSION 3.17)
project(CSED451)

set(CMAKE_CXX_STANDARD 11)

# macOS Load
IF (APPLE)
    # FreeGLUT
    # brew install freeglut
    set(FREEGLUT_H /usr/local/Cellar/freeglut/3.2.1_1/include)
    set(FREEGLUT_LINK /usr/local/Cellar/freeglut/3.2.1_1/lib/libglut.dylib)

    # GLEW
    # brew install glew
    set(GLEW_H /usr/local/Cellar/glew/2.2.0/include)
    set(GLEW_LINK /usr/local/Cellar/glew/2.2.0/lib/libGLEW.dylib)

    # GLM
    # brew install glm
    set(GLM_H /usr/local/Cellar/glm/0.9.9.8/include)

    include_directories(${GLEW_H} ${FREEGLUT_H} ${GLM_H})
    link_libraries(${GLEW_LINK} ${FREEGLUT_LINK})

    # OpenGL
    include_directories(/System/Library/Frameworks)

    find_library(GLUT_LIBRARY GLUT)

    find_library(OpenGL_LIBRARY OpenGL)

    mark_as_advanced(GLUT_LIBRARY OpenGL_LIBRARY)

    set(EXTRA_LIBS ${GLUT_LIBRARY} ${OpenGL_LIBRARY})

ENDIF (APPLE)

# ASSN1
add_executable(ASSN1 ASSN/ASSN1/main.cpp ASSN/ASSN1/Shape.cpp ASSN/ASSN1/Shape.h)
target_link_libraries(ASSN1 ${EXTRA_LIBS})