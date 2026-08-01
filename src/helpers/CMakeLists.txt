qt_internal_add_qml_module(Quick3DJoltPhysicsHelpers
    URI "QtQuick3D.JoltPhysics.Helpers"
    VERSION "${PROJECT_VERSION}"
    DESIGNER_SUPPORTED
    CLASS_NAME QtQuick3DJoltPhysicsHelpersPlugin
    PLUGIN_TARGET qtquick3djoltphysicshelpersplugin
    SOURCES
        capsulegeometry.cpp capsulegeometry_p.h
        capsulegeometryutils_p.h
        taperedcapsulegeometry.cpp taperedcapsulegeometry_p.h
        taperedcylindergeometry.cpp taperedcylindergeometry_p.h
    DEPENDENCIES
        QtQuick3D.JoltPhysics/auto
        QtQuick3D/auto
        QtQuick/auto
    PAST_MAJOR_VERSIONS 1
    DEFINES
    LIBRARIES
        Qt::Quick3DJoltPhysics
    PUBLIC_LIBRARIES
        Qt::Qml
        Qt::Quick
        Qt::Quick3D
)

# External modules must not use qrc "prefer" — it breaks DS code model.
get_target_property(_jolt_helpers_qmldir_content Quick3DJoltPhysicsHelpers _qt_internal_qmldir_content)
if(_jolt_helpers_qmldir_content)
    string(REGEX REPLACE "\nprefer :[^\\n]*\\n" "\n" _jolt_helpers_qmldir_content "${_jolt_helpers_qmldir_content}")
    set_target_properties(Quick3DJoltPhysicsHelpers PROPERTIES _qt_internal_qmldir_content "${_jolt_helpers_qmldir_content}")
endif()

if(QT_FEATURE_quick_designer AND QT_BUILD_SHARED_LIBS)
    add_subdirectory(designer)
endif()
