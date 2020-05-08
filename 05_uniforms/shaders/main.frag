#version 450

layout(location = 4) uniform vec3 object_color;
layout(location = 5) uniform vec3 material_ambient_color;
layout(location = 6) uniform vec3 material_diffuse_color;
layout(location = 7) uniform vec4 light_position;
layout(location = 8) uniform vec3 camera_position;
layout(location = 9) uniform float material_shininess;
layout(location = 10) uniform vec3 material_specular_color;

layout(location = 11) uniform vec3 light_ambient_color;
layout(location = 12) uniform vec3 light_diffuse_color;
layout(location = 13) uniform vec3 light_specular_color;

layout(binding = 0) uniform sampler2D material_diffuse_texture;

// Tak 5.2: Add eye(camera) position
// Tak 5.2: Add light's position, ambient+diffuse+specular terms
// Tak 5.2: Add material's ambient+diffuse+specular+shininess terms

layout(location = 0) in vec3 fs_position;
layout(location = 1) in vec3 fs_normal;
layout(location = 2) in vec2 fs_texture_coordinate;

layout(location = 0) out vec4 final_color;

void main()
{
	// Tak 5.2: Compute (N)ormal, (L)ight, (E)ye, (H)alf vectors
	// Tak 5.2: Compute dot products between N and L, N and H
	// Tak 5.2: Compute material values multipliedby light's values
	// Tak 5.2: Compute final color using Blinn–Phong shading model
	vec3 light_vector = light_position.xyz - fs_position * light_position.w;
	vec3 L = normalize(light_vector);
	vec3 N = normalize(fs_normal); // Normal vector of the current fragment	
	vec3 E = normalize(camera_position - fs_position); // Direction from the current fragment to the camera	
	vec3 H = normalize(L + E); // Half vector between (E)ye and L
	
	float NdotL = max(dot(N, L), 0.0);
	float NdotH = max(dot(N, H), 0.0);
	
	float distance2 =  light_position.w == 1.0 ? pow(length(light_vector), 2) : 1.0;

	vec3 material_ambient = material_ambient_color * light_ambient_color;
	vec3 material_diffuse =  material_diffuse_color * light_diffuse_color * texture(material_diffuse_texture, fs_texture_coordinate).rgb ;
	vec3 material_specular = material_specular_color * light_specular_color;

	vec3 result =  material_ambient
		+ NdotL * material_diffuse 
		+ pow(NdotH, material_shininess) * material_specular;
	
	//Phong model
//	vec3 specular_light = specular_light_strength * pow(max(dot(normalize(camera_position - fs_position), reflect(- normalize(diffuse_light_position - fs_position), normalize(fs_normal))), 0.0), 32) * specular_light_color ;
//	vec3 diffuse_light = max(dot(normalize(fs_normal), normalize(diffuse_light_position - fs_position) ), 0.0) * diffuse_light_color ;
//	vec3 ambient_light = ambient_light_strength * ambient_light_color;
//	vec3 result = ( ambient_light + diffuse_light + specular_light) * object_color;


	final_color = vec4(result/distance2, 1.0);
	final_color = vec4(texture(material_diffuse_texture, fs_texture_coordinate).rgb,1.0);
}
