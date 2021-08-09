#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTex;

out DATA {
    vec3 Normal;
    vec3 color;
    vec2 texCoord;
    mat4 projection;
} data_out;

uniform mat4 camMatrix;
uniform mat4 model;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;

void main() {
    gl_Position = model * translation * rotation * scale * vec4(aPos, 1.0f);

    data_out.Normal = aNormal;
    data_out.color = aColor;
    data_out.texCoord = mat2(0.0f, -1.0f, 1.0f, 0.0f) * aTex;
    data_out.projection = camMatrix;
}