#version 430 core

struct Material
{
    float ambient;
    float diffuse;
    int shininess;
    
    sampler2D texture;
    sampler2D specular;
};

struct Light
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 position;
};

struct DirLight
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 direction;
};

struct PointLight
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 position;

    float constant;
    float linear;
    float quadratic;
};

struct Spotlight
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 position;
    vec3 direction;

    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;
};

vec3 calcLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 calcSpotlight(Spotlight light, vec3 normal, vec3 fragPos, vec3 viewDir);

in vec3 vNormal;
in vec2 vTexCoord;
in vec3 vFragPos;

uniform int numLights = 0;
uniform int numDirLights = 1;
uniform int numPointLights = 1;
uniform int numSpotlights = 0;

uniform Light lights[1];
uniform DirLight dirLights[1];
uniform PointLight pointLights[1];
uniform Spotlight spotlights[1];

uniform Material material;
uniform vec3 viewPos;

out vec4 FragColor;

void main()
{
    vec3 viewDir = normalize(viewPos - vFragPos);

    vec3 result = vec3(0.0f);

    int i;
    for (i = 0; i < numLights; i++)
    {
        result += calcLight(lights[i], vNormal, vFragPos, viewDir);
    }
    for (i = 0; i < numDirLights; i++)
    {
        result += calcDirLight(dirLights[i], vNormal, viewDir);
    }
    for (i = 0; i < numPointLights; i++)
    {
        result += calcPointLight(pointLights[i], vNormal, vFragPos, viewDir);
    }
    for (i = 0; i < numSpotlights; i++)
    {
        result += calcSpotlight(spotlights[i], vNormal, vFragPos, viewDir);
    }
    FragColor = vec4(result, 1.0f);
}

vec3 calcLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // ambient
    vec3 ambient = material.ambient * light.ambient;

    // diffuse 
    float diffuseStrength = max(dot(normal, lightDir), 0.0);

    vec3 diffuse = material.diffuse * diffuseStrength * light.diffuse;

    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spec * light.specular;

    return vec3(vec3(texture(material.texture, vTexCoord)) * (ambient + diffuse) + vec3(texture(material.specular, vTexCoord)) * specular);
}

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    // ambient
    vec3 ambient = material.ambient * light.ambient;

    // diffuse 
    float diffuseStrength = max(dot(normal, lightDir), 0.0);

    vec3 diffuse = material.diffuse * diffuseStrength * light.diffuse;

    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spec * light.specular;

    return vec3((vec3(texture(material.texture, vTexCoord)) * (ambient + diffuse)) + (vec3(texture(material.specular, vTexCoord)) * specular));
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    float lightDistance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * lightDistance + light.quadratic * (lightDistance * lightDistance));

    vec3 lightDir = normalize(light.position - fragPos);

    // ambient
    vec3 ambient = material.ambient * light.ambient;

    // diffuse 
    float diffuseStrength = max(dot(normal, lightDir), 0.0);

    vec3 diffuse = material.diffuse * diffuseStrength * light.diffuse;

    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spec * light.specular;

    return vec3(vec3(texture(material.texture, vTexCoord)) * (ambient + diffuse) + vec3(texture(material.specular, vTexCoord)) * specular) * attenuation;
}

vec3 calcSpotlight(Spotlight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float theta = dot(lightDir, normalize(-light.direction)); //angle between the spotlight's direction and lightray's direction
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);  
    float lightDistance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * lightDistance + light.quadratic * (lightDistance * lightDistance));

    // ambient
    vec3 ambient = material.ambient * light.ambient;

    // diffuse 
    float diffuseStrength = max(dot(normal, lightDir), 0.0);

    vec3 diffuse = material.diffuse * diffuseStrength * light.diffuse * intensity;

    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spec * light.specular * intensity;

    return vec3(vec3(texture(material.texture, vTexCoord)) * (ambient + diffuse) + vec3(texture(material.specular, vTexCoord)) * specular) * attenuation;
}