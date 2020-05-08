#version 450

struct Light {
	vec4 position;
	vec4 ambient_color;
	vec4 diffuse_color;
	vec4 specular_color;
};

struct Object {
	mat4 model_matrix;
	vec4 ambient_color;
	vec4 diffuse_color;
	vec3 specular_color;
	float shininess;
};

layout(binding = 0, std140) uniform Camera {
	mat4 projection;
	mat4 view;
	vec3 position;
} camera;

layout(binding = 1, std430) buffer Lights {
	Light lights[];
};

layout(binding = 2, std430) buffer Objects {
	Object objects[];
};

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texture_coordinate;

layout(location = 0) out vec3 fs_position;
layout(location = 1) out vec3 fs_normal;
layout(location = 2) out vec2 fs_texture_coordinate;
layout(location = 3) out flat int fs_instance_id;

void main()
{
	Object object = objects[gl_InstanceID];

	fs_position = position;
	fs_normal = transpose(inverse(mat3(object.model_matrix))) * normal;
	fs_texture_coordinate = texture_coordinate;

	// Tak 5.1: Multiply position by MVP matrices in correct order
    gl_Position = camera.projection * camera.view * object.model_matrix * vec4(position, 1.0);
}
