#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;
uniform Material material;
uniform vec3 viewPos;

in vec3 normal;
in vec3 FragPos;
in vec2 TexCoords;

void main()
{
    // Ambient light
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

    // Light direction
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - FragPos);

    // Diffuse light
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

    // View and reflect directions
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    // Specularity
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result , 1.0f);
}