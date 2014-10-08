#version 410

struct AmbientLight {
    vec3 color;
    float intensity;
};

struct DirectionalLight {
    AmbientLight ambient;
    vec3 color;
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
uniform vec3 cameraPos;
uniform float specularIntensity, specularPower;
uniform float ambient;
uniform sampler2D texSampler;

in vec3 fs_normal;
in vec2 fs_texture;
in vec3 world_pos;
out vec4 glColor;

void main() {
    AmbientLight light;
    light.color = vec3(1,1,1);
    light.intensity = ambient;

    DirectionalLight d_light;
    d_light.ambient = light;
    d_light.color = vec3(1,1,1);
    d_light.direction = light_dir;

    float diffIntense = max(0.0, dot(fs_normal, -d_light.direction));

    vec4 totalLight = vec4(d_light.ambient.color, 1.0) * (d_light.ambient.intensity + diffIntense);

    // calculate specular color
    float diffuseFactor = dot(fs_normal, d_light.direction);
    float specularColor = vec4(0,0,0,0);

    if(diffuseFactor > 0){
        vec3 vertexToView = normalize(cameraPos - world_pos);
        vec3 lightReflect = normalize(reflect(-d_light.direction, fs_normal));
        float specularFactor = max(0.0, dot(vertexToView, lightReflect));
        specularFactor = pow(specularFactor, specularPower);
        if(specularFactor > 0){
            specularColor = vec4(d_light.color, 1.0f) * specularIntensity * specularFactor;
        }
    }

    totalLight += specularColor;
    glColor = texture(texSampler, fs_texture.xy) * totalLight;
}
