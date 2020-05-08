add_executable(05_uniforms main.cpp application.cpp)

add_dependencies(05_uniforms glfw glad framework)

target_link_libraries(05_uniforms ${GLFW_LIBRARIES} ${GLAD_LIBRARIES} framework)
target_include_directories(05_uniforms 
	PRIVATE ${GLFW_INCLUDE_DIR}
	PRIVATE ${GLAD_INCLUDE_DIR}
	PRIVATE ${SINGLE_HEADER_LIBS_INCLUDE_DIR}
	PRIVATE ${FRAMEWORK_INCLUDE_DIR}
)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/shaders/main.vert" "${CMAKE_CURRENT_BINARY_DIR}/shaders/main.vert" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/shaders/main.frag" "${CMAKE_CURRENT_BINARY_DIR}/shaders/main.frag" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/shaders/skybox.vert" "${CMAKE_CURRENT_BINARY_DIR}/shaders/skybox.vert" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/shaders/skybox.frag" "${CMAKE_CURRENT_BINARY_DIR}/shaders/skybox.frag" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cat/12221_Cat_v1_l3.obj" "${CMAKE_CURRENT_BINARY_DIR}/cat/12221_Cat_v1_l3.obj" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cat/Cat_diffuse.jpg" "${CMAKE_CURRENT_BINARY_DIR}/cat/Cat_diffuse.jpg" COPYONLY)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/skybox/right.jpg" "${CMAKE_CURRENT_BINARY_DIR}/skybox/right.jpg" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/skybox/left.jpg" "${CMAKE_CURRENT_BINARY_DIR}/skybox/left.jpg" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/skybox/front.jpg" "${CMAKE_CURRENT_BINARY_DIR}/skybox/front.jpg" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/skybox/back.jpg" "${CMAKE_CURRENT_BINARY_DIR}/skybox/back.jpg" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/skybox/top.jpg" "${CMAKE_CURRENT_BINARY_DIR}/skybox/top.jpg" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/skybox/bottom.jpg" "${CMAKE_CURRENT_BINARY_DIR}/skybox/bottom.jpg" COPYONLY)