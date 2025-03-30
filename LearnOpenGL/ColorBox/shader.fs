#version 330 core
out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 color;
};

uniform Light light;
uniform Material material;

uniform vec3 objectColor;
uniform vec3 viewPos;

in vec3 normal;
in vec3 FragPos;

void main()
{
    // Ambient light
    vec3 ambient = light.ambient * (material.ambient * light.color);

    // Light direction
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - FragPos);

    // Diffuse light
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = (diff * material.diffuse) * light.color;

    // Fix diffuse
    diffuse = light.diffuse * diffuse;

    // View and reflect directions
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    // Specularity
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = (material.specular * spec) * light.color;

    // Fix specular
    specular = light.specular * specular;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result , 1.0f);
}