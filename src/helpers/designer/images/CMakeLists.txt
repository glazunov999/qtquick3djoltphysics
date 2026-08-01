#####################################################################
## QtQuick3D.JoltPhysics.Helpers designer images:
#####################################################################

qt_path_join(destination ${QT_INSTALL_DIR} "${INSTALL_QMLDIR}/QtQuick3D/JoltPhysics/Helpers/designer/images")
qt_copy_or_install(
    FILES
        capsulegeometry.png
        capsulegeometry16.png
        capsulegeometry@2x.png
        taperedcapsulegeometry.png
        taperedcapsulegeometry16.png
        taperedcapsulegeometry@2x.png
        taperedcylindergeometry.png
        taperedcylindergeometry16.png
        taperedcylindergeometry@2x.png
    DESTINATION "${destination}"
)
