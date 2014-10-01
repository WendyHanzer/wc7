#version 410

uniform mat4 mvp;
uniform mat4 view;
uniform mat4 model;
uniform mat4 modelView;

in vec3 vs_position;
in vec3 vs_normal;

out vec3 fs_normal;
out vec3 world_pos;

void main() {
    fs_normal = (model * vec4(vs_normal, 0.0)).xyz;
    gl_Position = (mvp * vec4(vs_position, 1.0));
    world_pos = (model * vec4(vs_position, 1.0)).xyz;

}