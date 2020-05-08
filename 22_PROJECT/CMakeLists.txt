add_executable(22_PROJECT main.cpp application.cpp)

add_dependencies(22_PROJECT glfw glad framework)

target_link_libraries(22_PROJECT ${GLFW_LIBRARIES} ${GLAD_LIBRARIES} framework)
target_include_directories(22_PROJECT 
	PRIVATE ${GLFW_INCLUDE_DIR}
	PRIVATE ${GLAD_INCLUDE_DIR}
	PRIVATE ${SINGLE_HEADER_LIBS_INCLUDE_DIR}
	PRIVATE ${FRAMEWORK_INCLUDE_DIR}
)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/shaders/main.vert" "${CMAKE_CURRENT_BINARY_DIR}/shaders/main.vert" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/shaders/main.frag" "${CMAKE_CURRENT_BINARY_DIR}/shaders/main.frag" COPYONLY)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/models/table/table.obj" "${CMAKE_CURRENT_BINARY_DIR}/models/table/table.obj" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/models/table/table.obj" "${CMAKE_CURRENT_BINARY_DIR}/models/table/table.obj" COPYONLY)


configure_file("${CMAKE_CURRENT_SOURCE_DIR}/shaders/main_textured.vert" "${CMAKE_CURRENT_BINARY_DIR}/shaders/main_textured.vert" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/shaders/main_textured.frag" "${CMAKE_CURRENT_BINARY_DIR}/shaders/main_textured.frag" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/models/skybox/bottom.jpg" "${CMAKE_CURRENT_BINARY_DIR}/models/skybox/bottom.jpg" COPYONLY)