cmake_minimum_required(VERSION 3.28)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
add_compile_options(-stdlib=libc++ -std=c++23)
add_link_options(-stdlib=libc++)
set(CMAKE_BUILD_TYPE Debug)

project (tiny_render)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON )

include_directories("modules")

if (DEFINED ENV{VULKAN_SDK} AND NOT "$ENV{VULKAN_SDK}" STREQUAL "")
    message( "VULKAN_SDK is set to $ENV{VULKAN_SDK}")
    include_directories($ENV{VULKAN_SDK}/include)
else()
    message(SEND_ERROR "VULKAN_SDK is not set")
endif()

if (DEFINED ENV{NLOHMANN_JSON_DIR} AND NOT "$ENV{NLOHMANN_JSON_DIR}" STREQUAL "")
    message( "NLOHMANN_JSON_DIR is set to $ENV{NLOHMANN_JSON_DIR}")
    include_directories($ENV{NLOHMANN_JSON_DIR}/include)
else()
    message(SEND_ERROR "NLOHMANN_JSON_DIR is not set")
endif()

if (DEFINED ENV{LIBUV_DIR} AND NOT "$ENV{LIBUV_DIR}" STREQUAL "")
    message("LIBUV_DIR is set to $ENV{LIBUV_DIR}")
    include_directories($ENV{LIBUV_DIR}/include)
    link_directories($ENV{LIBUV_DIR}/build)
else()
    message(SEND_ERROR "LIBUV_DIR is not set")
endif()

# Today is no physics
# if (DEFINED ENV{JOLT_PHYSICS_DIR} AND NOT "$ENV{JOLT_PHYSICS_DIR}" STREQUAL "")
#     message("JOLT_PHYSICS_DIR is set to $ENV{JOLT_PHYSICS_DIR}")
#     include_directories($ENV{JOLT_PHYSICS_DIR})
#     link_directories($ENV{JOLT_PHYSICS_DIR}/Build/Linux_Release)
# else()
#     message(SEND_ERROR "JOLT_PHYSICS_DIR is not set")
# endif()

find_package(Vulkan REQUIRED)

add_library(log
    modules/log/log.h
    modules/log/log.cpp
)
#global user defined output messages macro definition
add_compile_definitions(ENABLE_DEBUG_OUTPUT=false)
add_compile_definitions(ENABLE_WARNING_OUTPUT=true)
add_compile_definitions(ENABLE_INFO_OUTPUT=true)
add_compile_definitions(ENABLE_NOTICE_OUTPUT=true)
add_compile_definitions(ENABLE_ERROR_OUTPUT=true)
set_target_properties(log PROPERTIES LINKER_LANGUAGE CXX)

add_library(algebra
    modules/algebra/concepts.h
    modules/algebra/matrix.h
    modules/algebra/matrix2.h
    modules/algebra/matrix3.h
    modules/algebra/matrix4.h
    modules/algebra/vector.h
    modules/algebra/vector2.h
    modules/algebra/vector3.h
    modules/algebra/vector4.h
    modules/algebra/quaternion.h
)
set_target_properties(algebra PROPERTIES LINKER_LANGUAGE CXX)

add_library(config)
target_sources(config
  PUBLIC
    FILE_SET CXX_MODULES FILES
    modules/config/interface.cpp
    modules/config/config.cpp
)

add_library(render
    modules/render/render.cpp
    modules/render/render.h
    modules/render/timer.cpp
    modules/render/timer.h
    modules/render/screen_string_base.cpp
    modules/render/screen_string_base.h
    modules/render/gl/rendergl.cpp
    modules/render/gl/rendergl.h
    modules/render/gl/screen_string.cpp
    modules/render/gl/screen_string.h
    modules/render/gl/main_loop.cpp
    modules/render/gl/functions.cpp
    modules/render/gl/functions.h
    modules/render/gl/buffers/body_buffer.cpp
    modules/render/gl/buffers/body_buffer.h
    modules/render/gl/buffers/screen_string_buffer.cpp
    modules/render/gl/buffers/screen_string_buffer.h
    modules/render/gl/scene.h
    modules/render/gl/scene.cpp
    modules/render/gl/program.h
    modules/render/gl/program.cpp
    modules/render/gl/texture.h
    modules/render/gl/texture.cpp
    modules/render/gl/material.h
    modules/render/gl/material.cpp
    modules/render/vk/rendervk.cpp
    modules/render/vk/rendervk.h
    modules/render/vk/context.h
    modules/render/vk/context.cpp
    modules/render/vk/instance.cpp
    modules/render/vk/device.cpp
    modules/render/vk/swapchain.cpp
    modules/render/vk/main_loop.cpp
    modules/render/vk/pipelines/program.h
    modules/render/vk/pipelines/program.cpp
    modules/render/vk/pipelines/pipeline.h
    modules/render/vk/pipelines/pipeline.cpp
    modules/render/vk/pipelines/pipeline_matrix_ready.cpp
    modules/render/vk/pipelines/pipeline_vertex_buffer.cpp
    modules/render/vk/commands/scene_render_command.h
    modules/render/vk/commands/scene_render_command.cpp
    modules/render/vk/buffers/plain_buffer.h
    modules/render/vk/buffers/plain_buffer.cpp
    modules/render/vk/buffers/vertex_buffer.h
    modules/render/vk/buffers/vertex_buffer.cpp
    modules/render/vk/images/plain_image.h
    modules/render/vk/images/plain_image.cpp
    modules/render/vk/images/texture_image.h
    modules/render/vk/images/texture_image.cpp
    modules/render/vk/scene.h
    modules/render/vk/scene.cpp
    modules/render/vk/command_pool.h
    modules/render/vk/command_pool.cpp
)
target_link_libraries(render scene config geometry algebra event)

add_library(image
    modules/image/bmp.cpp
    modules/image/bmp.h
    modules/image/tga.cpp
    modules/image/tga.h
    modules/image/canvas.cpp
    modules/image/canvas.h
    modules/image/color.cpp
    modules/image/color.h
)

add_library(geometry
    modules/geometry/wavefront.cpp
    modules/geometry/wavefront.h
    modules/geometry/polytope.cpp
    modules/geometry/polytope.h
    modules/geometry/bounding_volumes.cpp
    modules/geometry/bounding_volumes.h
)
target_link_libraries(geometry algebra)

add_library(scene
    modules/scene/constants.h
    modules/scene/scene.h
    modules/scene/scene.cpp
    modules/scene/body.cpp
    modules/scene/body.h
    modules/scene/light.h
    modules/scene/light.cpp
    modules/scene/flycam.h
    modules/scene/flycam.cpp
    modules/scene/mesh.h
    modules/scene/mesh.cpp
)
target_link_libraries(scene config io image algebra geometry)

add_library(event)
target_sources(event
  PUBLIC
    FILE_SET CXX_MODULES FILES
    modules/event/interface.cpp
    modules/event/async.cpp
    modules/event/context.cpp
    modules/event/timer.cpp
)
target_link_libraries(event config)

add_library(io)
target_sources(io
  PUBLIC
    FILE_SET CXX_MODULES FILES
    modules/io/interface.cpp
    modules/io/file.cpp
)
target_link_libraries(io event)

add_subdirectory(test/window)
target_link_libraries(gl_test PUBLIC 
    log 
    event 
    io 
    algebra 
    scene 
    config 
    render 
    X11
    Xfixes
    GL 
    GLU 
    Vulkan::Vulkan 
    uv
)

target_link_libraries(vk_test PUBLIC 
    log 
    event 
    io 
    algebra 
    scene 
    config 
    render 
    X11
    Xfixes
    GL 
    GLU 
    Vulkan::Vulkan 
    uv
)

set_target_properties( vk_test gl_test
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "../../../bin"
)

add_subdirectory(test/wavefront)
target_link_libraries(wavefront geometry)

add_subdirectory(test/color)
target_link_libraries(color image)
