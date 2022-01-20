#version 460 core

#define shininess 32
#define DIR_LIGHTS 5
#define PNT_LIGHTS 5
#define SPT_LIGHTS 5

struct Material {
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	sampler2D texture_emission1;
};
struct dirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
struct pntLight {
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
struct sptLight {
	vec3 position;
	vec3 direction;

	float cutoff;
	float outercutoff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material material;
uniform dirLight dirlight[DIR_LIGHTS];
uniform pntLight pntlight[PNT_LIGHTS];
uniform sptLight sptlight[SPT_LIGHTS];
uniform vec3 viewerPos;

in vec3 fragPos;
in vec3 Normal;
in vec2 texCoords;

out vec4 fragColor;

vec3 calcDirLight(dirLight light, vec3 normal, vec3 viewDir);
vec3 calcPntLight(pntLight light, vec3 normal, vec3 viewDir, vec3 fragPos);
vec3 calcSptLight(sptLight light, vec3 normal, vec3 viewDir, vec3 fragPos);

void main()
{
	vec3 viewDir = normalize(viewerPos - fragPos);

	vec3 result = vec3(0.0f);

	for (int i = 0; i < DIR_LIGHTS; i++)
		result += calcDirLight(dirlight[i], Normal, viewDir);
	for (int i = 0; i < PNT_LIGHTS; i++)
		result += calcPntLight(pntlight[i], Normal, viewDir, fragPos);
	for (int i = 0; i < SPT_LIGHTS; i++)
		result += calcSptLight(sptlight[i], Normal, viewDir, fragPos);

	fragColor = vec4(result, 1.0f);
}

vec3 calcDirLight(dirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	//Ambient component
	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, texCoords));
	//Diffuse component
	float diff = max(dot(lightDir, normal), 0.0f);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, texCoords));
	//Specular component
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0f), shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, texCoords));

	return (ambient + diffuse + specular);
}
vec3 calcPntLight(pntLight light, vec3 normal, vec3 viewDir, vec3 fragPos)
{
	vec3 lightDir = normalize(light.position - fragPos);
	float distance = length(light.position - fragPos);
	float attenuation;
	if (light.constant != 0)
		attenuation =  1.0f / (light.constant + light.linear * distance + light.quadratic * distance * distance);
	else
		attenuation = 0.0f;

	vec3 ambient = light.ambient * attenuation * vec3(texture(material.texture_diffuse1, texCoords));
	
	float diff = max(dot(lightDir, normal), 0.0f);
	vec3 diffuse = light.diffuse * diff * attenuation * vec3(texture(material.texture_diffuse1, texCoords));

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0f), shininess);
	vec3 specular = light.specular * spec * attenuation * vec3(texture(material.texture_specular1, texCoords));

	return (ambient + diffuse + specular);
}
vec3 calcSptLight(sptLight light, vec3 normal, vec3 viewDir, vec3 fragPos)
{
	vec3 lightDir = normalize(light.position - fragPos);
	float theta = dot(lightDir, light.direction);
	float epsilon = light.cutoff - light.outercutoff;
	float intensity = clamp((theta - light.outercutoff) / epsilon, 0.0f, 1.0f);

	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, texCoords));

	float diff = max(dot(lightDir, normal), 0.0f);
	vec3 diffuse = light.diffuse * diff * intensity * vec3(texture(material.texture_diffuse1, texCoords));

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0f), shininess);
	vec3 specular = light.specular * spec * intensity * vec3(texture(material.texture_specular1, texCoords));

	return (ambient + diffuse + specular);
}