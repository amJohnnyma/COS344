#!/bin/bash

#  submission.sh  –  COS344 P2 Submission Tool
#  Usage:
#    ./submission.sh create    – flatten all files into ./submission/
#    ./submission.sh makerun   – restore structure, build, and run
#    ./submission.sh clean     – remove build/bin output
#    ./submission.sh delete    – delete the submission folder

ACTION=$1
SUBMIT="submission"

# HELP
if [[ -z "$ACTION" ]]; then
    echo "Usage: ./submission.sh [create|makerun|clean|delete]"
    echo ""
    echo "  create   – Copy all necessary files flat into ./submission/"
    echo "  makerun  – Restore folder structure, build with CMake, and run"
    echo "  clean    – Delete build/ directory inside submission"
    echo "  delete   – Delete the entire submission folder"
    exit 0
fi

# CREATE  –  flatten everything into ./submission/
if [[ "$ACTION" == "create" ]]; then
    echo " Creating flat submission folder..."
    rm -rf "$SUBMIT"
    mkdir -p "$SUBMIT"

    #Source files (all flat)
    cp main.cpp                  "$SUBMIT/"
    cp src/Vector.cpp            "$SUBMIT/"
    cp src/Square.cpp            "$SUBMIT/"
    cp src/Matrix.cpp            "$SUBMIT/"
    cp src/Triangle.cpp          "$SUBMIT/"
    cp src/Circle.cpp            "$SUBMIT/"
    cp src/Renderer.cpp          "$SUBMIT/"
    cp src/shader.cpp            "$SUBMIT/"
    cp src/Scene.cpp             "$SUBMIT/"
    cp src/SceneCreator.cpp      "$SUBMIT/"
    cp src/glad.c                "$SUBMIT/"

    #Shaders (prefixed to avoid name clashes)
    cp src/shaders/fragment/fragment.glsl   "$SUBMIT/shader_fragment.glsl"
    cp src/shaders/vertex/vertex.glsl       "$SUBMIT/shader_vertex.glsl"
    cp src/shaders/wireframe.glsl           "$SUBMIT/shader_wireframe.glsl"

    #Headers (all flat, prefixed by original subfolder)
    cp include/DebugOptions.h            "$SUBMIT/"
    cp include/shader.hpp                "$SUBMIT/"
    cp include/khrplatform.h             "$SUBMIT/khrplatform_root.h"

    cp include/engine/Camera.h           "$SUBMIT/engine_Camera.h"
    cp include/engine/Color.h            "$SUBMIT/engine_Color.h"
    cp include/engine/FpsLimiter.h       "$SUBMIT/engine_FpsLimiter.h"
    cp include/engine/Input.h            "$SUBMIT/engine_Input.h"
    cp include/engine/PhysicsBody.h      "$SUBMIT/engine_PhysicsBody.h"
    cp include/engine/Physics.h          "$SUBMIT/engine_Physics.h"
    cp include/engine/Renderer.h         "$SUBMIT/engine_Renderer.h"
    cp include/engine/SceneCreator.h     "$SUBMIT/engine_SceneCreator.h"
    cp include/engine/Scene.h            "$SUBMIT/engine_Scene.h"

    cp include/math/Circle.h             "$SUBMIT/math_Circle.h"
    cp include/math/Matrix.h             "$SUBMIT/math_Matrix.h"
    cp include/math/Shape.h              "$SUBMIT/math_Shape.h"
    cp include/math/Square.h             "$SUBMIT/math_Square.h"
    cp include/math/Triangle.h           "$SUBMIT/math_Triangle.h"
    cp include/math/Vector.h             "$SUBMIT/math_Vector.h"

    cp include/glad/glad.h               "$SUBMIT/glad_glad.h"
    cp include/KHR/khrplatform.h         "$SUBMIT/KHR_khrplatform.h"

    #Build config
    cp CMakeLists.txt            "$SUBMIT/"

    #Bin text files
    cp build/bin/Demo.txt        "$SUBMIT/"
    cp build/bin/DemoSave.txt    "$SUBMIT/"
    cp build/bin/test.txt        "$SUBMIT/"

    #Docs
    cp BonusMarks.txt            "$SUBMIT/"

    # Copy this script itself into the submission
    cp "$0"                      "$SUBMIT/submission.sh"
    chmod +x                     "$SUBMIT/submission.sh"

    echo ""
    echo " Flat submission folder created at ./$SUBMIT/"
    echo "  Files: $(ls $SUBMIT | wc -l)"
    ls "$SUBMIT"
    exit 0
fi

# MAKERUN  –  restore structure, cmake, build, run
if [[ "$ACTION" == "makerun" ]]; then
    # Must be run from inside the submission folder
    SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
    cd "$SCRIPT_DIR"

    echo " Restoring folder structure..."

    #Create directories
    mkdir -p src/shaders/fragment
    mkdir -p src/shaders/vertex
    mkdir -p include/engine
    mkdir -p include/math
    mkdir -p include/glad
    mkdir -p include/KHR
    mkdir -p include/game
    mkdir -p build/bin

    #Move source files into src/
    for f in Vector.cpp Square.cpp Matrix.cpp Triangle.cpp Circle.cpp \
              Renderer.cpp shader.cpp Scene.cpp SceneCreator.cpp glad.c; do
        [[ -f "$f" ]] && mv "$f" src/
    done

    #Restore shaders
    [[ -f shader_fragment.glsl ]]  && mv shader_fragment.glsl  src/shaders/fragment/fragment.glsl
    [[ -f shader_vertex.glsl ]]    && mv shader_vertex.glsl    src/shaders/vertex/vertex.glsl
    [[ -f shader_wireframe.glsl ]] && mv shader_wireframe.glsl src/shaders/wireframe.glsl

    #Restore headers
    [[ -f DebugOptions.h ]]        && mv DebugOptions.h        include/
    [[ -f shader.hpp ]]            && mv shader.hpp            include/
    [[ -f khrplatform_root.h ]]    && mv khrplatform_root.h    include/khrplatform.h

    [[ -f engine_Camera.h ]]       && mv engine_Camera.h       include/engine/Camera.h
    [[ -f engine_Color.h ]]        && mv engine_Color.h        include/engine/Color.h
    [[ -f engine_FpsLimiter.h ]]   && mv engine_FpsLimiter.h   include/engine/FpsLimiter.h
    [[ -f engine_Input.h ]]        && mv engine_Input.h        include/engine/Input.h
    [[ -f engine_PhysicsBody.h ]]  && mv engine_PhysicsBody.h  include/engine/PhysicsBody.h
    [[ -f engine_Physics.h ]]      && mv engine_Physics.h      include/engine/Physics.h
    [[ -f engine_Renderer.h ]]     && mv engine_Renderer.h     include/engine/Renderer.h
    [[ -f engine_SceneCreator.h ]] && mv engine_SceneCreator.h include/engine/SceneCreator.h
    [[ -f engine_Scene.h ]]        && mv engine_Scene.h        include/engine/Scene.h

    [[ -f math_Circle.h ]]         && mv math_Circle.h         include/math/Circle.h
    [[ -f math_Matrix.h ]]         && mv math_Matrix.h         include/math/Matrix.h
    [[ -f math_Shape.h ]]          && mv math_Shape.h          include/math/Shape.h
    [[ -f math_Square.h ]]         && mv math_Square.h         include/math/Square.h
    [[ -f math_Triangle.h ]]       && mv math_Triangle.h       include/math/Triangle.h
    [[ -f math_Vector.h ]]         && mv math_Vector.h         include/math/Vector.h

    [[ -f glad_glad.h ]]           && mv glad_glad.h           include/glad/glad.h
    [[ -f KHR_khrplatform.h ]]     && mv KHR_khrplatform.h     include/KHR/khrplatform.h

    #Move txt files to bin
    for f in Demo.txt DemoSave.txt test.txt; do
        [[ -f "$f" ]] && mv "$f" build/bin/
    done

    echo "Structure restored."
    echo ""
    echo " Running CMake configure..."
    cd build
    cmake .. -DCMAKE_BUILD_TYPE=Release
    if [[ $? -ne 0 ]]; then
        echo " CMake configure failed."
        exit 1
    fi

    echo ""
    echo " Building..."
    make -j$(nproc)
    if [[ $? -ne 0 ]]; then
        echo " Build failed."
        exit 1
    fi

    echo ""
    echo " Build successful. Launching..."
    cd bin
    ./MyOpenGLProject
    exit 0
fi

# CLEAN  –  remove build output
if [[ "$ACTION" == "clean" ]]; then
    SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
    cd "$SCRIPT_DIR"

    echo " Cleaning build directory..."
    rm -rf build/CMakeFiles build/CMakeCache.txt build/Makefile \
           build/cmake_install.cmake build/lib \
           build/bin/MyOpenGLProject build/bin/shaders
    echo " Clean done. Text files in build/bin/ preserved."
    exit 0
fi


echo "Unknown action: '$ACTION'"
echo "Usage: ./submission.sh [create|makerun|clean]"
exit 1
