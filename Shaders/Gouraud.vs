#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 viewPos;
uniform vec3 lightPos;

out float ambient_vert;
out float diffuse_vert;
out float specular_vert;

out vec2 TexCoords;

void main()
{
	vec3 FragPos = vec3(model * vec4(aPos, 1.0));
	vec3 Normal = mat3(transpose(inverse(model))) * aNormal;  
	
	// Ambient
	ambient_vert = 0.1;

	// Diffuse
	vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    diffuse_vert = 0.8 * diff; 
	
	// Specular
	vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 8);
    specular_vert =  0.2 * spec;
	
	TexCoords = aTexCoords;
	
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}