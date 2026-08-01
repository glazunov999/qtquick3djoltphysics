set(PHYSICS_REPO_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/)

if(EMSCRIPTEN)
    set(USE_WASM_SIMD ON)
endif()

set(ENABLE_OBJECT_STREAM ON)

include(Jolt/Jolt.cmake)

qt_internal_add_3rdparty_library(BundledJoltPhysics
    STATIC
    SKIP_AUTOMOC
    EXCEPTIONS
    INSTALL
    SOURCES ${JOLT_PHYSICS_SRC_FILES}
    INCLUDE_DIRECTORIES
        ./
    DEFINES
        JPH_DEBUG_RENDERER
        JPH_OBJECT_STREAM
)

# Silence warnings
qt_disable_warnings(BundledJoltPhysics)
qt_set_symbol_visibility_hidden(BundledJoltPhysics)

if(EMSCRIPTEN AND USE_WASM_SIMD)
    target_compile_options(BundledJoltPhysics PRIVATE -msimd128 -msse4.2)
endif()
