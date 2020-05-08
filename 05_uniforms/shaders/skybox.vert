
#version 450
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

layout(location = 0) uniform mat4 projection;
layout(location = 1) uniform mat4 view;

void main()
{
    TexCoords = aPos;
    gl_Position = projection * view * vec4(aPos, 1.0);
} 