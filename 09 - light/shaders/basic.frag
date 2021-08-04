#version 460 core

out vec4 FragColor;

in vec2 texCoord;
in vec3 Normal;
in vec3 currentPosition;

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

vec4 pointLight() {
    vec3 lightVec = lightPos - currentPosition;
    float dist = length(lightVec);
    float a = 0.5f;
    float b = .01f;
    float inten = 1.0f / (a * dist + b * dist + 1.0f);

    float ambient = .3f;

    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(lightVec);
    float diffuse = max(dot(normal, lightDirection), .0f);

    float specularLight = .8;
    vec3 viewDirection = normalize(camPos - currentPosition);
    vec3 reflectionDirection = reflect(-lightDirection, normal);

    float specAmount = pow(max(dot(viewDirection, reflectionDirection), .0f), 8);
    float specular = specAmount * specularLight;

    return (texture(tex0, texCoord) * (diffuse * inten + ambient + texture(tex1, texCoord).r * specular * inten))* lightColor;
}

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

    return (texture(tex0, texCoord) * (diffuse+ ambient + texture(tex1, texCoord).r * specular))* lightColor;
}

vec4 spotLight() {
    float outerCone = .9f;
    float innerCone = .95f;
    float ambient = .3f;

    vec3 lightVec = lightPos - currentPosition;

    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(lightVec);
    float diffuse = max(dot(normal, lightDirection), .0f);

    float specularLight = .8;
    vec3 viewDirection = normalize(camPos - currentPosition);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float specAmount = pow(max(dot(viewDirection, reflectionDirection), .0f), 8);
    float specular = specAmount * specularLight;

    float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
    float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

    return (texture(tex0, texCoord) * (diffuse * inten + ambient + texture(tex1, texCoord).r * specular * inten))* lightColor;
}

void main() {
    FragColor = spotLight();
}