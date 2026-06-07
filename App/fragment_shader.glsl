#version 430 core

in vec2 uv;
in vec3 baseColor;
uniform sampler2D texture_sampler;

in vec3 v;
in vec3 l;
in vec3 n;

out vec4 color;

uniform vec3 diffuseAlbedo = vec3(0, 0, 1);
uniform vec3 specularAlbedo = vec3(0.1, 0.1, 0.1);
const vec4 ambient = vec4(0.1, 0.1, 0.1, 0.8);

void main()
{
	vec3 r = reflect(-l, n);

	vec4 textureColor = texture(texture_sampler, uv);
	vec3 diffuse = max(dot(l, n), 0.0) * (textureColor.rgb + baseColor);
	vec3 specular = pow(max(dot(r, v), 0.0), 12) * specularAlbedo;

	color = ambient * textureColor + vec4(diffuse + specular, 1.0);
}
