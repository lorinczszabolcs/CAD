QT += core gui widgets opengl

win32 {
    message("Windows platform...")

    INCLUDEPATH += $$PWD/Dependencies/Include
    DEPENDPATH += $$PWD/Dependencies/Include

    LIBS += -lopengl32 -lglu32

    CONFIG(release, debug|release): {
        contains(QT_ARCH, i386) {
            message("x86 (i.e., 32-bit) release build")
            LIBS += -L"$$PWD/Dependencies/Lib/GL/x86/" -lglew32
        } else {
            message("x86_64 (i.e., 64-bit) release build")
            LIBS += -L"$$PWD/Dependencies/Lib/GL/x86_64/" -lglew32
        }
    } else: CONFIG(debug, debug|release): {
        contains(QT_ARCH, i386) {
            message("x86 (i.e., 32-bit) debug build")
            LIBS += -L"$$PWD/Dependencies/Lib/GL/x86/" -lglew32
        } else {
            message("x86_64 (i.e., 64-bit) debug build")
            LIBS += -L"$$PWD/Dependencies/Lib/GL/x86_64" -lglew32
        }
    }

    msvc {
      QMAKE_CXXFLAGS += -openmp -arch:AVX -D "_CRT_SECURE_NO_WARNINGS"
      QMAKE_CXXFLAGS_RELEASE *= -O2
    }
}

mac {
    # for GLEW installed into /usr/lib/libGLEW.so or /usr/lib/glew.lib
    LIBS += -lGLEW -lGLU
}

unix {
    # for GLEW installed into /usr/lib/libGLEW.so or /usr/lib/glew.lib
    LIBS += -lGLEW -lGLU
}

FORMS += \
    GUI/MainWindow.ui \
    GUI/SideWidget.ui

HEADERS += \
    GUI/GLWidget.h \
    GUI/MainWindow.h \
    GUI/SideWidget.h \
    Core/Exceptions.h \
    Core/DCoordinates3.h \
    Core/Matrices.h \
    Core/RealSquareMatrices.h \
    Core/Constants.h \
    Core/GenericCurves3.h \
    Dependencies/Include/GL/glew.h \
    Parametric/ParametricCurves3.h \
    Test/TestFunctions.h \
    Core/LinearCombination3.h \
    Cyclic/CyclicCurves3.h \
    Core/Constants.h \
    Core/HCoordinates3.h \
    Parametric/ParametricSurfaces3.h \
    Core/HCoordinates3.h \
    Core/TCoordinates4.h \
    Core/TriangularFaces.h \
    Core/TriangulatedMeshes3.h \
    Core/Colors4.h \
    Core/Lights.h \
    Core/Materials.h \
    Core/Colors4.h \
    Core/Constants.h \
    Core/DCoordinates3.h \
    Core/Exceptions.h \
    Core/GenericCurves3.h \
    Core/HCoordinates3.h \
    Core/Lights.h \
    Core/LinearCombination3.h \
    Core/Materials.h \
    Core/Matrices.h \
    Core/RealSquareMatrices.h \
    Core/TCoordinates4.h \
    Core/TensorProductSurfaces3.h \
    Core/TriangularFaces.h \
    Core/TriangulatedMeshes3.h \
    Cyclic/CyclicCurves3.h \
    Dependencies/Include/GL/glew.h \
    FirstOrderAlgebraicTrigonometric/FirstOrderAlgebraicTrigonometricArcs3.h \
    FirstOrderAlgebraicTrigonometric/FirstOrderAlgebraicTrigonometricCompositeCurve3.h \
    FirstOrderAlgebraicTrigonometric/FirstOrderAlgebraicTrigonometricCompositeSurfaces3.h \
    FirstOrderAlgebraicTrigonometric/FirstOrderAlgebraicTrigonometricPatches3.h \
    GUI/GLWidget.h \
    GUI/MainWindow.h \
    GUI/SideWidget.h \
    Parametric/ParametricCurves3.h \
    Parametric/ParametricSurfaces3.h \
    Test/TestFunctions.h \
    Core/Colors4.h \
    Core/Constants.h \
    Core/DCoordinates3.h \
    Core/Exceptions.h \
    Core/GenericCurves3.h \
    Core/HCoordinates3.h \
    Core/Lights.h \
    Core/LinearCombination3.h \
    Core/Materials.h \
    Core/Matrices.h \
    Core/RealSquareMatrices.h \
    Core/ShaderPrograms.h \
    Core/TCoordinates4.h \
    Core/TensorProductSurfaces3.h \
    Core/TriangularFaces.h \
    Core/TriangulatedMeshes3.h \
    Cyclic/CyclicCurves3.h \
    Dependencies/Include/GL/glew.h \
    FirstOrderAlgebraicTrigonometric/FirstOrderAlgebraicTrigonometricArcs3.h \
    FirstOrderAlgebraicTrigonometric/FirstOrderAlgebraicTrigonometricCompositeCurve3.h \
    FirstOrderAlgebraicTrigonometric/FirstOrderAlgebraicTrigonometricCompositeSurfaces3.h \
    FirstOrderAlgebraicTrigonometric/FirstOrderAlgebraicTrigonometricPatches3.h \
    GUI/GLWidget.h \
    GUI/MainWindow.h \
    GUI/SideWidget.h \
    Parametric/ParametricCurves3.h \
    Parametric/ParametricSurfaces3.h \
    Test/TestFunctions.h
SOURCES += \
    GUI/GLWidget.cpp \
    GUI/MainWindow.cpp \
    GUI/SideWidget.cpp \
    main.cpp \
    Core/RealSquareMatrices.cpp \
    Core/GenericCurves3.cpp \
    Parametric/ParametricCurves3.cpp \
    Core/LinearCombination3.cpp \
    Test/TestFunctions.cpp \
    Cyclic/CyclicCurves3.cpp \
    Core/GenericCurves3.cpp \
    Parametric/ParametricSurfaces3.cpp \
    Core/TriangulatedMeshes3.cpp \
    Core/Lights.cpp \
    Core/Lights.cpp \
    Core/Materials.cpp \
    Core/GenericCurves3.cpp \
    Core/Lights.cpp \
    Core/LinearCombination3.cpp \
    Core/Materials.cpp \
    Core/RealSquareMatrices.cpp \
    Core/TensorProductSurfaces3.cpp \
    Core/TriangulatedMeshes3.cpp \
    Cyclic/CyclicCurves3.cpp \
    FirstOrderAlgebraicTrigonometric/FirstOrderAlgebraicTrigonometricArcs3.cpp \
    FirstOrderAlgebraicTrigonometric/FirstOrderAlgebraicTrigonometricCompositeCurves3.cpp \
    FirstOrderAlgebraicTrigonometric/FirstOrderAlgebraicTrigonometricCompositeSurfaces3.cpp \
    FirstOrderAlgebraicTrigonometric/FirstOrderAlgebraicTrigonometricPatches.cpp \
    GUI/GLWidget.cpp \
    GUI/MainWindow.cpp \
    GUI/SideWidget.cpp \
    Parametric/ParametricCurves3.cpp \
    Parametric/ParametricSurfaces3.cpp \
    Test/TestFunctions.cpp \
    main.cpp \
    Core/GenericCurves3.cpp \
    Core/Lights.cpp \
    Core/LinearCombination3.cpp \
    Core/Materials.cpp \
    Core/RealSquareMatrices.cpp \
    Core/TensorProductSurfaces3.cpp \
    Core/TriangulatedMeshes3.cpp \
    Cyclic/CyclicCurves3.cpp \
    FirstOrderAlgebraicTrigonometric/FirstOrderAlgebraicTrigonometricArcs3.cpp \
    FirstOrderAlgebraicTrigonometric/FirstOrderAlgebraicTrigonometricCompositeCurves3.cpp \
    FirstOrderAlgebraicTrigonometric/FirstOrderAlgebraicTrigonometricCompositeSurfaces3.cpp \
    FirstOrderAlgebraicTrigonometric/FirstOrderAlgebraicTrigonometricPatches.cpp \
    GUI/GLWidget.cpp \
    GUI/MainWindow.cpp \
    GUI/SideWidget.cpp \
    Parametric/ParametricCurves3.cpp \
    Parametric/ParametricSurfaces3.cpp \
    Test/TestFunctions.cpp \
    main.cpp \
    Core/ShaderPrograms.cpp

CONFIG += console

DISTFILES += \
    ../build-QtFramework-Desktop_Qt_5_10_1_MSVC2017_64bit-Debug/test.txt \
    Dependencies/Lib/GL/x86/glew32.lib \
    Dependencies/Lib/GL/x86_64/glew32.lib \
    Dependencies/Lib/GL/x86/glew32.dll \
    Dependencies/Lib/GL/x86_64/glew32.dll

SUBDIRS += \
    QtFramework.pro

RESOURCES += qdarkstyle/style.qrc
