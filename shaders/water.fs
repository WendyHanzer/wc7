#version 410

struct AmbientLight {
    vec3 color;
    float intensity;
};

struct DirectionalLight {
    AmbientLight ambient;
    vec3 direction;
};

struct Attenuation {
    float constant;
    float linear;
    float ex;
};

struct PointLight {
    AmbientLight ambient;
    vec3 pos;
};

uniform vec3 light_dir;

in vec3 fs_normal;

out vec4 glColor;

void main() {
    AmbientLight light;
    light.color = vec3(1,1,1);
    light.intensity = 0.0;

    DirectionalLight d_light;
    d_light.ambient = light;
    d_light.direction = light_dir;

    float diffIntense = max(0.0, dot(fs_normal, -d_light.direction));

    vec4 totalLight = vec4(d_light.ambient.color, 1.0) * (d_light.ambient.intensity + diffIntense);

    glColor = vec4(0,0,1,1) * totalLight;
}