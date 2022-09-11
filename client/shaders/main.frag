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
};

vec3 calcLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 calcSpotlight(Spotlight light, vec3 normal, vec3 fragPos, vec3 viewDir);

in vec3 vNormal;
in vec2 vTexCoord;
in vec3 vFragPos;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

out vec4 FragColor;

void main()
{
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
    vec3 lightPos = vec3(10.0f, 10.0f, 10.0f);
    vec3 viewPos = vec3(0.0f, 0.0f, -3.0f);

    vec3 norm = normalize(vNormal);
    vec3 viewDir = normalize(viewPos - vFragPos);

    /*// ambient
    vec3 ambient = material.ambient * lightColor;

    // diffuse 
    vec3 lightDir = normalize(lightPos - vFragPos);
    float diffuseStrength = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = material.diffuse * diffuseStrength * lightColor;

    // specular
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spec * lightColor; */

    FragColor = vec4(calcLight(light, norm, vFragPos, viewDir), 1.0f);
    // (texture(material.texture, vTexCoord) * vec4((ambient + diffuse), 1.0f)) + (texture(material.specular, vTexCoord) * vec4(specular, 1.0f));
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
    // ambient
    vec3 ambient = material.ambient * light.ambient;

    // diffuse 
    float diffuseStrength = max(dot(normal, light.direction), 0.0);

    vec3 diffuse = material.diffuse * diffuseStrength * light.diffuse;

    // specular
    vec3 reflectDir = reflect(-light.direction, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spec * light.specular;

    return vec3(vec3(texture(material.texture, vTexCoord)) * (ambient + diffuse) + vec3(texture(material.specular, vTexCoord)) * specular);
}
/*vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{

}
vec3 calcSpotlight(Spotlight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{

}*/