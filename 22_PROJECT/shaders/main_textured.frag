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

layout(binding = 0) uniform sampler2D material_diffuse_texture;

// Tak 5.2: Add eye(camera) position
// Tak 5.2: Add light's position, ambient+diffuse+specular terms
// Tak 5.2: Add material's ambient+diffuse+specular+shininess terms

layout(location = 0) in vec3 fs_position;
layout(location = 1) in vec3 fs_normal;
layout(location = 2) in vec2 fs_texture_coordinate;
layout(location = 3) in flat int fs_instance_id;

layout(location = 0) out vec4 final_color;

void main()
{
	Object object = objects[fs_instance_id];

	vec3 color_sum = vec3(0.0);
	for(int i = 0; i < lights.length(); i++) {
		Light light = lights[i];

		vec3 light_vector = light.position.xyz - fs_position * light.position.w;
		vec3 L = normalize(light_vector);
		vec3 N = normalize(fs_normal);
		vec3 E = normalize(camera.position - fs_position); 
		vec3 H = normalize(L + E);

		float NdotL = max(dot(N, L), 0.0);
		float NdotH = max(dot(N, H), 0.0001);

		vec3 ambient = object.ambient_color.rgb * light.ambient_color.rgb;
		vec3 diffuse = object.diffuse_color.rgb * light.diffuse_color.rgb * texture(material_diffuse_texture, fs_texture_coordinate).rgb;
		vec3 specular = object.specular_color.rgb * light.specular_color.rgb;

		vec3 color = ambient.rgb
			+ NdotL * diffuse.rgb
			+ pow(NdotH, object.shininess) * specular;

		color /= (length(light_vector));

		color_sum += color;
	}

	final_color = vec4(color_sum, 1.0);
}
