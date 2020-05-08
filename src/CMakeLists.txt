add_executable(08_ubo_final main.cpp application.cpp)

add_dependencies(08_ubo_final glfw glad framework)

target_link_libraries(08_ubo_final ${GLFW_LIBRARIES} ${GLAD_LIBRARIES} framework)
target_include_directories(08_ubo_final 
	PRIVATE ${GLFW_INCLUDE_DIR}
	PRIVATE ${GLAD_INCLUDE_DIR}
	PRIVATE ${SINGLE_HEADER_LIBS_INCLUDE_DIR}
	PRIVATE ${FRAMEWORK_INCLUDE_DIR}
)