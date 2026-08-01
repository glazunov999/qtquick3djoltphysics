#####################################################################
## QtQuick3D.JoltPhysics.Helpers designer support:
#####################################################################

qt_path_join(destination ${QT_INSTALL_DIR} "${INSTALL_QMLDIR}/QtQuick3D/JoltPhysics/Helpers/designer")
qt_copy_or_install(
    FILES
        CapsuleGeometrySection.qml
        CapsuleGeometrySpecifics.qml
        TaperedCapsuleGeometrySection.qml
        TaperedCapsuleGeometrySpecifics.qml
        TaperedCylinderGeometrySection.qml
        TaperedCylinderGeometrySpecifics.qml
        helpers.metainfo
    DESTINATION "${destination}"
)

add_subdirectory(images)
