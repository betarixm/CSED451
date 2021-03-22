cmake_minimum_required(VERSION 3.17)
project(CSED451)

set(CMAKE_CXX_STANDARD 11)

# macOS Load
IF (APPLE)
    # FreeGLUT
    set(FREEGLUT_H ${CMAKE_SOURCE_DIR}/extensions/darwin/freeglut/3.2.1_1/include)
    set(FREEGLUT_LINK ${CMAKE_SOURCE_DIR}/extensions/darwin/freeglut/3.2.1_1/lib/libglut.dylib)

    # GLEW
    set(GLEW_H ${CMAKE_SOURCE_DIR}/extensions/darwin/glew/2.2.0/include)
    set(GLEW_LINK ${CMAKE_SOURCE_DIR}/extensions/darwin/glew/2.2.0/lib/libGLEW.dylib)

    # GLM
    set(GLM_H ${CMAKE_SOURCE_DIR}/extensions/darwin/glm/0.9.9.8/include)

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
add_executable(ASSN1 ASSN/ASSN1/main.cpp ASSN/ASSN1/Shape.cpp ASSN/ASSN1/Shape.h ASSN/ASSN1/Ship.cpp ASSN/ASSN1/Ship.h ASSN/ASSN1/Game.cpp ASSN/ASSN1/Game.h)
target_link_libraries(ASSN1 ${EXTRA_LIBS})
# ASSN2
file(GLOB ASSN2_H ASSN/ASSN2/*.h)
file(GLOB ASSN2_CPP ASSN/ASSN2/*.cpp)
add_executable(ASSN2 ${ASSN2_CPP} ${ASSN2_H})

target_link_libraries(ASSN2 ${EXTRA_LIBS})