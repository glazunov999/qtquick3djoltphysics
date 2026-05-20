#####################################################################
## Quick3DJoltPhysics Module:
#####################################################################

qt_internal_add_qml_module(Quick3DJoltPhysics
    URI "QtQuick3D.JoltPhysics"
        VERSION "${PROJECT_VERSION}"
        CLASS_NAME QtQuick3DJoltPhysicsPlugin
        PLUGIN_TARGET qquick3djoltphysicsplugin
        DESIGNER_SUPPORTED
        NO_PLUGIN_OPTIONAL
        NO_GENERATE_PLUGIN_SOURCE
    SOURCES
        abstractphysicsbody_p.h abstractphysicsbody.cpp
        abstractphysicscharacter_p.h abstractphysicscharacter.cpp
        abstractphysicsconstraint_p.h abstractphysicsconstraint.cpp
        abstractphysicsnode_p.h abstractphysicsnode.cpp
        abstractshape_p.h abstractshape.cpp
        abstractsoftbodycontactlistener.h abstractsoftbodycontactlistener.cpp
        body_p.h body.cpp
        boxshape_p.h boxshape.cpp
        capsuleshape_p.h capsuleshape.cpp
        character_p.h character.cpp
        abstractcharactercontactlistener.h abstractcharactercontactlistener.cpp
        charactervirtual_p.h charactervirtual.cpp
        collideshaperesult_p.h collideshaperesult.cpp
        collisiongroup_p.h collisiongroup.cpp
        abstractcontactlistener.h abstractcontactlistener.cpp
        convexhullshape_p.h convexhullshape.cpp
        cylindershape_p.h cylindershape.cpp
        distanceconstraint_p.h distanceconstraint.cpp
        emptyshape_p.h emptyshape.cpp
        fixedconstraint_p.h fixedconstraint.cpp
        gearconstraint_p.h gearconstraint.cpp
        groupfiltertable_p.h groupfiltertable.cpp
        heightfieldshape_p.h heightfieldshape.cpp
        hermitepath_p.h hermitepath.cpp
        hingeconstraint_p.h hingeconstraint.cpp
        meshshape_p.h meshshape.cpp
        motorsettings_p.h motorsettings.cpp
        pathconstraint_p.h pathconstraint.cpp
        pathconstraintpath_p.h pathconstraintpath.cpp
        physicslayers.h physicslayers.cpp
        physicsmeshutils_p_p.h
        physicssettings_p.h physicssettings.cpp
        physicssystem_p.h physicssystem.cpp
        physicsutils_p.h
        planeshape_p.h planeshape.cpp
        pointconstraint_p.h pointconstraint.cpp
        pulleyconstraint_p.h pulleyconstraint.cpp
        rackandpinionconstraint_p.h rackandpinionconstraint.cpp
        sliderconstraint_p.h sliderconstraint.cpp
        softbody_p.h softbody.cpp
        softbodymeshgeometry.h softbodymeshgeometry.cpp
        softbodysharedsettings.h softbodysharedsettings.cpp
        qtquick3djoltphysicsglobal.h
        qtquick3djoltphysicsglobal_p.h
        raycastresult_p.h raycastresult.cpp
        rotatedtranslatedshape_p.h rotatedtranslatedshape.cpp
        shapecastresult_p.h shapecastresult.cpp
        sphereshape_p.h sphereshape.cpp
        springsettings_p.h springsettings.cpp
        staticcompoundshape_p.h staticcompoundshape.cpp
        swingtwistconstraint_p.h swingtwistconstraint.cpp
        triangleresult_p.h triangleresult.cpp
    DEFINES
    SYSTEM_INCLUDE_DIRECTORIES
        ../3rdparty/JoltPhysics/
    LIBRARIES
        Qt::CorePrivate
        Qt::GuiPrivate
        Qt::QmlPrivate
        Qt::Quick3DPrivate
        Qt::QuickPrivate
        Qt::BundledJoltPhysics
    PUBLIC_LIBRARIES
        Qt::Core
        Qt::Gui
        Qt::Qml
        Qt::Quick
        Qt::Quick3D
    PRIVATE_MODULE_INTERFACE
        Qt::CorePrivate
        Qt::GuiPrivate
        Qt::QmlPrivate
        Qt::Quick3D
        Qt::QuickPrivate
)

qt_internal_extend_target(qquick3djoltphysicsplugin
    SOURCES
        plugin.cpp
    LIBRARIES
        Qt::Quick3DJoltPhysicsPrivate
)
