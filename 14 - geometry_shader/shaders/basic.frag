#version 460 core

out vec4 FragColor;

in vec3 currentPosition;
in vec3 Normal;
in vec3 color;
in vec2 texCoord;

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

vec4 direcLight() {
    vec3 lightVec = lightPos - currentPosition;

    float ambient = .3f;

    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.5f));
    float diffuse = max(dot(normal, lightDirection), .0f);

    float specularLight = .8;
    vec3 viewDirection = normalize(camPos - currentPosition);
    vec3 reflectionDirection = reflect(-lightDirection, normal);

    float specAmount = pow(max(dot(viewDirection, reflectionDirection), .0f), 8);
    float specular = specAmount * specularLight;

    return (texture(diffuse0, texCoord) * (diffuse+ ambient + texture(specular0, texCoord).r * specular))* lightColor;
}

void main() {
    FragColor = direcLight();
}