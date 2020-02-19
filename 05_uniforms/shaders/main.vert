#version 450

// Tak 5.1: Add projection, view, model matrices as uniforms

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texture_coordinate;

layout(location = 0) out vec3 fs_position;
layout(location = 1) out vec3 fs_normal;
layout(location = 2) out vec2 fs_texture_coordinate;

void main()
{
	fs_position = position;
	// Task 5.1 Uncomment when you obtain model matrix
	// fs_normal = transpose(inverse(mat3(model))) * normal;
	fs_texture_coordinate = texture_coordinate;

	// Tak 5.1: Multiply position by MVP matrices in correct order
    gl_Position = vec4(position, 1.0);
}
