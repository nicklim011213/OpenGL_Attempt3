#version 330 core

struct Material
{
	sampler2D texture_diffuse1;
	sampler2D texture_diffuse2;
	sampler2D texture_diffuse3;
	sampler2D texture_specular1;
	sampler2D texture_specular2;
	float shininess;
};

struct Light {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
}; 

out vec4 FragColor;

in vec2 myTex;
in vec3 myNormal;
in vec3 myFragPos;

uniform vec3 viewpos;

uniform Material mat;
uniform Light light; 

void main()
{

	vec3 lightDir = normalize(-light.direction);

	// Ambient	
	vec3 ambientLight = light.ambient * vec3(texture(mat.diffuse, myTex));
	
	// Diffuse
	vec3 norm = normalize(myNormal);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(mat.diffuse, myTex));
	
	// Specular
	vec3 viewDir = normalize(viewpos - myFragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), (mat.shininess * 128));
	vec3 specular = light.specular * spec * vec3(texture(mat.specular, myTex));  
	
	// Combined
	vec3 result = (ambientLight + diffuse + specular);
	
	FragColor = vec4(result, 1.0);
	
	//Greyscale Light Debug
	//FragColor = vec4(vec3(diff), 1.0);
}