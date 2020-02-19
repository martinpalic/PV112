#version 450

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

	final_color = vec4(1.0, 1.0, 1.0, 1.0);
}
