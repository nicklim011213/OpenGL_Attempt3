#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 myTex;
out vec3 myNormal;
out vec3 myFragPos;

void main()
{
	myNormal = mat3(transpose(inverse(model))) * aNormal;
	myFragPos = vec3(model * vec4(aPos, 1.0));
	myTex = aTex;
	
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}	