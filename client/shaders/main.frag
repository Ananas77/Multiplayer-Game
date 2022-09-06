#version 430 core

struct Material
{
    float ambient;
    float diffuse;
    float specular;
};

in vec3 vNormal;
in vec2 vTexCoord;
in vec3 vFragPos;

uniform sampler2D textures;
uniform Material material;
uniform vec3 viewPos;

out vec4 FragColor;

void main()
{
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
    vec3 lightPos = vec3(10.0f, 10.0f, 10.0f);
    vec3 viewPos = vec3(0.0f, 0.0f, -3.0f);

    //ambient
    vec3 ambient = material.ambient * lightColor;

    // diffuse 
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(lightPos - vFragPos);
    float diffuseStrength = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = material.diffuse * diffuseStrength * lightColor;

    // specular
    vec3 viewDir = normalize(viewPos - vFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = material.specular * spec * lightColor; 

    FragColor = texture(textures, vTexCoord) * vec4((ambient + diffuse + specular), 1.0f);
}