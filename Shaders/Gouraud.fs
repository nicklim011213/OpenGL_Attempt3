#version 330 core
out vec4 FragColor;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

in float ambient_vert;
in float diffuse_vert;
in float specular_vert;

in vec2 TexCoords;

void main()
{    
	vec3 DiffuseTexture = texture(texture_diffuse1, TexCoords).rgb;
	vec3 SpecularTexture = mix(texture(texture_specular1, TexCoords).rgb, vec3(1.0), 0.8);
	
	vec3 FinalAmbient = ambient_vert * DiffuseTexture;
	vec3 FinalDiffuse = diffuse_vert * DiffuseTexture;
	vec3 FinalSpecular = specular_vert * SpecularTexture;
	
	vec3 result = FinalAmbient + FinalDiffuse + FinalSpecular;
	
    FragColor = vec4(result, 1.0);
}