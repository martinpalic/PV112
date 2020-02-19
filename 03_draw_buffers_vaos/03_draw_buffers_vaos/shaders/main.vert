#version 450

// Task 3.1: Draw a Square
// Task 3.2: Draw a Square using strip
const vec2 triangle[3] = {
	vec2(0.0, 0.5),
	vec2(-0.5, 0.0),
	vec2(0.5, 0.0)
};

void main()
{
    gl_Position = vec4(triangle[gl_VertexID], 0.0, 1.0);
}
