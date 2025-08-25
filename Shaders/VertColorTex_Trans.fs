#version 330 core
out vec4 FragColor;

in vec3 myColor;
in vec2 myTex;
in vec3 myNormal;
in vec3 myFragPos;

uniform sampler2D mytexture;
uniform vec3 lightcolor;
uniform vec3 lightPos;
uniform float Trans;

void main()
{
	float ambientStrength = 0.1;
	vec3 ambientLight = ambientStrength * lightcolor;
	
	vec3 norm = normalize(myNormal);
	vec3 lightDir = normalize(lightPos - myFragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightcolor;
	
	vec3 result = (ambientLight + diffuse) * myColor;
	
	FragColor = texture(mytexture, myTex) * vec4(result, Trans);
}