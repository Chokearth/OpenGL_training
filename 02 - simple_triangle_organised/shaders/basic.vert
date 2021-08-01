#version 330 core

layout (location = 0) in vec3 aPos;

uniform float rotation;

void main() {
    float s = sin(rotation);
    float c = cos(rotation);

    gl_Position= vec4(aPos.x*c - aPos.y *s, aPos.x*s + aPos.y * c, aPos.z, 1.0f);
}