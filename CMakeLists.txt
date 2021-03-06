cmake_minimum_required(VERSION 3.2)

include(ExternalProject)

project(pv112)

if(UNIX)
    find_package(OpenGL REQUIRED)
    find_package(X11 REQUIRED)
    find_package(Threads REQUIRED)
endif()

# GLFW library
set(GLFW_INSTALL_DIR "${CMAKE_BINARY_DIR}/glfw")
set(GLFW_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw/include")
set(GLFW_LIBRARIES   "${GLFW_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}glfw3${CMAKE_STATIC_LIBRARY_SUFFIX}")
ExternalProject_Add(glfw
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw"

    DOWNLOAD_COMMAND ""
    UPDATE_COMMAND ""

    CMAKE_ARGS
        "-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/glfw"
        "-DGLFW_BUILD_EXAMPLES=OFF"
        "-DGLFW_BUILD_TESTS=OFF"
        "-DGLFW_BUILD_DOCS=OFF"
        "-DBUILD_SHARED_LIBS=OFF"

    BUILD_BYPRODUCTS ${GLFW_LIBRARIES}
)

if(UNIX)
    set(GLFW_LIBRARIES ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES} ${X11_LIBRARIES} ${X11_Xrandr_LIB} ${X11_Xcursor_LIB} ${X11_Xinerama_LIB} ${X11_Xxf86vm_LIB} ${CMAKE_THREAD_LIBS_INIT})
endif()

# GLAD library
set(GLAD_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/glad/include")
add_library(glad "${CMAKE_CURRENT_SOURCE_DIR}/libs/glad/src/glad.c")
target_include_directories(glad PRIVATE "${GLAD_INCLUDE_DIR}")
set(GLAD_LIBARIES)
if(UNIX)
    set(GLAD_LIBRARIES "glad" ${CMAKE_DL_LIBS})
else()
    set(GLAD_LIBRARIES glad)
endif()

# Single header libraries
set(SINGLE_HEADER_LIBS_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/single_header")

# Configure compiler flags for our project
if (CMAKE_CXX_COMPILER_ID MATCHES Clang OR ${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
  # using regular Clang or AppleClang or GCC
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
  # using Visual Studio C++
  set(CMAKE_CXX_FLAGS "/std:c++17 /permissive- /W3 /EHsc")
endif()

# PV112 Framework
set(FRAMEWORK_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/framework/include")
set(FRAMEWORK_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/framework/src")
add_library(framework
	"${FRAMEWORK_INCLUDE_DIR}/utilities.hpp"
	"${FRAMEWORK_SRC_DIR}/utilities.cpp"
	"${FRAMEWORK_INCLUDE_DIR}/program.hpp"
	"${FRAMEWORK_SRC_DIR}/program.cpp"
	"${FRAMEWORK_INCLUDE_DIR}/camera.hpp"
	"${FRAMEWORK_SRC_DIR}/camera.cpp"
	"${FRAMEWORK_INCLUDE_DIR}/mesh.hpp"
	"${FRAMEWORK_SRC_DIR}/mesh.cpp"
	"${FRAMEWORK_INCLUDE_DIR}/rotate_camera.hpp"
	"${FRAMEWORK_SRC_DIR}/rotate_camera.cpp"
	"${FRAMEWORK_INCLUDE_DIR}/texture.hpp"
	"${FRAMEWORK_SRC_DIR}/texture.cpp"
	)
add_dependencies(framework glfw glad)
target_link_libraries(framework 
	${GLFW_LIBRARIES}
	${GLAD_LIBRARIES}
)
target_include_directories(framework 
	PRIVATE ${GLFW_INCLUDE_DIR}
	PRIVATE ${GLAD_INCLUDE_DIR}
	PRIVATE ${FRAMEWORK_INCLUDE_DIR}
	PRIVATE ${SINGLE_HEADER_LIBS_INCLUDE_DIR}
	PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/objects"
)

# YOUR PROJECT
add_executable(final
	src/main.cpp 
	src/application.cpp
	src/application.hpp
	structs/CameraUBO.hpp
	structs/ObjectUBO.hpp
	structs/LightUBO.hpp
	structs/SpotLightUBO.hpp
	models/WallsObject.cpp
	models/WallsObject.hpp
	models/table/TableObject.cpp
	models/table/TableObject.hpp
	models/gardenTable/GardenTableObject.cpp
	models/gardenTable/GardenTableObject.hpp	
	models/snake/SnakeObject.cpp
	models/snake/SnakeObject.hpp
	models/plate/PlateObject.cpp
	models/plate/PlateObject.hpp
	models/cup/CupObject.cpp
	models/cup/CupObject.hpp
	models/chair/ChairObject.cpp
	models/chair/ChairObject.hpp
	models/cat/CatObject.cpp
	models/cat/CatObject.hpp
	models/treat/TreatObject.cpp
	models/treat/TreatObject.hpp
	models/lamp/LampObject.cpp
	models/lamp/LampObject.hpp
 )

add_dependencies(final 
	glfw 
	glad 
	framework
)

target_link_libraries(final 
	${GLFW_LIBRARIES} 
	${GLAD_LIBRARIES} 
	framework
)

target_include_directories(final 
	PRIVATE ${GLFW_INCLUDE_DIR}
	PRIVATE ${GLAD_INCLUDE_DIR}
	PRIVATE ${SINGLE_HEADER_LIBS_INCLUDE_DIR}
	PRIVATE ${FRAMEWORK_INCLUDE_DIR}
)

# You can use configure_file to copy all the files needed to run the application into the build folder.
# That way you don't have to configure the IDE to run the program from inside the source folder.
# configure_file("${CMAKE_CURRENT_SOURCE_DIR}/FILL_IN" "${CMAKE_CURRENT_BINARY_DIR}/FILL_IN" COPYONLY)
#
#
set(SHADERS ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
set(SHADERS_BINARY ${CMAKE_CURRENT_BINARY_DIR}/shaders)
configure_file(${SHADERS}/draw_lights.vert ${SHADERS_BINARY}/draw_lights.vert COPYONLY)
configure_file(${SHADERS}/draw_lights.frag ${SHADERS_BINARY}/draw_lights.frag COPYONLY)
configure_file(${SHADERS}/draw_object.vert ${SHADERS_BINARY}/draw_object.vert COPYONLY)
configure_file(${SHADERS}/draw_object.frag ${SHADERS_BINARY}/draw_object.frag COPYONLY)
configure_file(${SHADERS}/draw_object_textured.vert ${SHADERS_BINARY}/draw_object_textured.vert COPYONLY)
configure_file(${SHADERS}/draw_object_textured.frag ${SHADERS_BINARY}/draw_object_textured.frag COPYONLY)
configure_file(${SHADERS}/postprocess.vert ${SHADERS_BINARY}/postprocess.vert COPYONLY)
configure_file(${SHADERS}/postprocess.frag ${SHADERS_BINARY}/postprocess.frag COPYONLY)
configure_file(${SHADERS}/spot_light_textured.vert ${SHADERS_BINARY}/spot_light_textured.vert COPYONLY)
configure_file(${SHADERS}/spot_light_textured.frag ${SHADERS_BINARY}/spot_light_textured.frag COPYONLY)
configure_file(${SHADERS}/textured_walls.frag ${SHADERS_BINARY}/textured_walls.frag COPYONLY)

set(IMAGES ${CMAKE_CURRENT_SOURCE_DIR}/images)
set(IMAGES_BINARY ${CMAKE_CURRENT_BINARY_DIR}/images)
configure_file(${IMAGES}/default.png ${IMAGES_BINARY}/default.png COPYONLY)

set(OBJECTS ${CMAKE_CURRENT_SOURCE_DIR}/objects)
set(OBJECTS_BINARY ${CMAKE_CURRENT_BINARY_DIR}/objects)
configure_file(${OBJECTS}/test_scene.mtl ${OBJECTS_BINARY}/test_scene.mtl COPYONLY)
configure_file(${OBJECTS}/test_scene.obj ${OBJECTS_BINARY}/test_scene.obj COPYONLY)


configure_file("${CMAKE_CURRENT_SOURCE_DIR}/models/skybox/bottom.jpg" "${CMAKE_CURRENT_BINARY_DIR}/models/skybox/bottom.jpg" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/models/skybox/front.jpg" "${CMAKE_CURRENT_BINARY_DIR}/models/skybox/front.jpg" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/models/skybox/top.jpg" "${CMAKE_CURRENT_BINARY_DIR}/models/skybox/top.jpg" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/models/skybox/back.jpg" "${CMAKE_CURRENT_BINARY_DIR}/models/skybox/back.jpg" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/models/skybox/left.jpg" "${CMAKE_CURRENT_BINARY_DIR}/models/skybox/left.jpg" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/models/skybox/right.jpg" "${CMAKE_CURRENT_BINARY_DIR}/models/skybox/right.jpg" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/models/skybox/wood.jpg" "${CMAKE_CURRENT_BINARY_DIR}/models/skybox/wood.jpg" COPYONLY)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/models/table/table.obj" "${CMAKE_CURRENT_BINARY_DIR}/models/table/table.obj" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/models/table/wood.jpg" "${CMAKE_CURRENT_BINARY_DIR}/models/table/wood.jpg" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/models/chair/chair.obj" "${CMAKE_CURRENT_BINARY_DIR}/models/chair/chair.obj" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/models/chair/wood.jpg" "${CMAKE_CURRENT_BINARY_DIR}/models/chair/wood.jpg" COPYONLY)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/models/cat/cat.obj" "${CMAKE_CURRENT_BINARY_DIR}/models/cat/cat.obj" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/models/cat/cat.jpg" "${CMAKE_CURRENT_BINARY_DIR}/models/cat/cat.jpg" COPYONLY)



configure_file("${CMAKE_CURRENT_SOURCE_DIR}/models/snake/snake.obj" "${CMAKE_CURRENT_BINARY_DIR}/models/snake/snake.obj" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/models/snake/snake.jpg" "${CMAKE_CURRENT_BINARY_DIR}/models/snake/snake.jpg" COPYONLY)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/models/gardenTable/gardenTable.obj" "${CMAKE_CURRENT_BINARY_DIR}/models/gardenTable/gardenTable.obj" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/models/gardenTable/gardenTable.jpg" "${CMAKE_CURRENT_BINARY_DIR}/models/gardenTable/gardenTable.jpg" COPYONLY)


configure_file("${CMAKE_CURRENT_SOURCE_DIR}/models/lamp/Lamp.obj" "${CMAKE_CURRENT_BINARY_DIR}/models/lamp/Lamp.obj" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/models/lamp/Lamp.jpg" "${CMAKE_CURRENT_BINARY_DIR}/models/lamp/Lamp.jpg" COPYONLY)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/models/plate/plate.obj" "${CMAKE_CURRENT_BINARY_DIR}/models/plate/plate.obj" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/models/cup/cup.obj" "${CMAKE_CURRENT_BINARY_DIR}/models/cup/cup.obj" COPYONLY)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/audio.wav" "${CMAKE_CURRENT_BINARY_DIR}/src/audio.wav" COPYONLY)