#version 460 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex;

out vec3 fragPos;
out vec3 Normal;
out vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalModel;

void main()
{
	gl_Position = projection * view * model * vec4(vertex, 1.0f);
	fragPos = vec3(model * vec4(vertex, 1.0f));
	Normal = normalModel * normalize(normal);
	texCoords = tex;
}