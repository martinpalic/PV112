#version 450

// Gaussian kernel
const float gaussBlurKernel[3][3] = {
	{1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0},
	{2.0 / 16.0, 4.0 / 16.0, 2.0 / 16.0},
	{1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0}
};

// Edge detection kernel
const float edgeDetectionKernel[3][3] = {
	{-1.0, -1.0, -1.0},
	{-1.0,  8.0, -1.0},
	{-1.0, -1.0, -1.0}
};

layout(binding = 0) uniform sampler2D input_image;

layout(location = 0) out vec4 final_color;

void main() {
	vec3 color = vec3(0.0);
	for(int i = -1; i <= 1; i++) {
		for(int j = -1; j <= 1; j++) {
			color += edgeDetectionKernel[i + 1][j + 1] * texelFetch(input_image, ivec2(gl_FragCoord.xy) + ivec2(i, j), 0).rgb;
		}
	}

	final_color = vec4(color, 1.0);	
}