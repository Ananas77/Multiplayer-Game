#version 430 core

struct Material
{
    float ambient;
    float diffuse;
    int shininess;
    
    sampler2D texture;
    sampler2D specular;
};

in vec3 vNormal;
in vec2 vTexCoord;
in vec3 vFragPos;

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
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spec * lightColor; 

    FragColor = (texture(material.texture, vTexCoord) * vec4((ambient + diffuse), 1.0f)) + (texture(material.specular, vTexCoord) * vec4(specular, 1.0f));
}