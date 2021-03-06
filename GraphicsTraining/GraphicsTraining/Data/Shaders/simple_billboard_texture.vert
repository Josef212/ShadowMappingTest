#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aColor;

uniform mat4 projection;
uniform mat4 model_view;

out vec2 TexCoords;

void main()
{
	gl_Position = projection * model_view * vec4(aPos, 1.0);
	TexCoords = aTexCoord;
}
