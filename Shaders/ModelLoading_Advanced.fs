#version 330 core
out vec4 FragColor;

in vec2 TexturePosition;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

uniform vec3 viewPos;
uniform vec3 lightPos;

void main()
{   
	// Ambient
	vec3 ambient = 0.1 * texture(texture_diffuse1, TexturePosition).rgb;

	// Diffuse
	vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = 0.8 * diff * texture(texture_diffuse1, TexturePosition).rgb;  
	
	// Specular
	vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 8);
    vec3 specular =  0.2 * spec * mix(texture(texture_specular1, TexturePosition).rgb, vec3(1.0), 1); 
	
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}