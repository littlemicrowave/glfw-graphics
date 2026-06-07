#version 430 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec3 vertexPosition;
in vec3 vertexColor;
in vec2 vertexUV;
in vec3 vertexNormal;

out vec3 v;
out vec3 l;
out vec3 n;

out vec2 uv;
out vec3 baseColor;

uniform vec4 lightPosition;
uniform float lightBool;

void main()
{
    uv = vertexUV;
    baseColor = vertexColor;
    vec4 position = view * model * vec4(vertexPosition, 1.0);
    gl_Position = projection * position;
    vec4 lightView = view * lightPosition;

    l = normalize((lightView - position).xyz);
	n = normalize(lightBool * (view * model * vec4(vertexNormal, 0.0)).xyz);
	v = normalize(-position.xyz);
}