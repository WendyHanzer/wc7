#version 410


const float pi = 3.14159;
uniform mat4 mvp;
uniform mat4 view;
uniform mat4 model;
uniform mat4 modelView;

in vec3 vs_position;
in vec3 vs_normal;
in vec2 vs_texture;
uniform float time;

out vec3 fs_normal;
out vec3 world_pos;
out vec2 fs_texture;

// water variables
float wavelength[4]; // speed per second
float speed[4];
float amplitude[4]; // max height of waves
vec2 direction[4];

float waveheight = 3.0f;
int numWaves;

// SOURCE: http://jayconrod.com/posts/34/water-simulation-in-glsl

float wave(int i, float x, float y) {
    float frequency = 2*pi/wavelength[i];
    float phase = speed[i] * frequency;
    float theta = dot(direction[i], vec2(x, y));
    return amplitude[i] * sin(theta * frequency + time * phase);
}

float waveHeight(float x, float y) {
    float height = 0.0;
    for (int i = 0; i < numWaves; ++i)
        height += wave(i, x, y);
    return height;
}

float dWavedx(int i, float x, float y) {
    float frequency = 2*pi/wavelength[i];
    float phase = speed[i] * frequency;
    float theta = dot(direction[i], vec2(x, y));
    float A = amplitude[i] * direction[i].x * frequency;
    return A * cos(theta * frequency + time * phase);
}

float dWavedy(int i, float x, float y) {
    float frequency = 2*pi/wavelength[i];
    float phase = speed[i] * frequency;
    float theta = dot(direction[i], vec2(x, y));
    float A = amplitude[i] * direction[i].y * frequency;
    return A * cos(theta * frequency + time * phase);
}

vec3 waveNormal(float x, float y) {
    float dx = 0.0;
    float dy = 0.0;
    for (int i = 0; i < numWaves; ++i) {
        dx += dWavedx(i, x, y);
        dy += dWavedy(i, x, y);
    }
    vec3 n = vec3(-dx, -dy, 1.0);
    return normalize(n);
}

void main() {
    numWaves = 4;

	// wave 1
	wavelength[0] = 800 * pi;
	speed[0] = 1000.0f;
	amplitude[0] = 30f;
	direction[0] = vec2(0.0, 1.0); 

	// wave 2
	wavelength[1] = 500 * pi;
	speed[1] = 37.0f;
	amplitude[1] = 13.0f;
	direction[1] = vec2(1.0, 0.5); 

	// wave 3
	wavelength[2] = 223 * pi;
	speed[2] = 9.5f;
	amplitude[2] = 10.0f;
	direction[2] = vec2(-0.4, 0.6); 

	// wave 3
	wavelength[3] = 500 * pi;
	speed[3] = 60.0f;
	amplitude[3] = 10.0f;
	direction[3] = vec2(-0.4, -0.6);

	vec4 pos = vec4(vs_position, 1.0);
	pos.y = waveHeight(pos.x, pos.z);

    fs_normal = (model * vec4(waveNormal(pos.x, pos.z), 0.0)).xyz;//(model * vec4(vs_normal, 0.0)).xyz;
    fs_texture = vs_texture;

    gl_Position = (mvp * pos);
    world_pos = (model * vec4(vs_position, 1.0)).xyz;

}

/*float dist = distance((model * vec4(vs_position.xyz, 1.0)).xyz, vec3(0,0,0));
vec3 newPos = vs_position;
vec3 direction = normalize((model * vec4(vs_position.xyz, 1.0)).xyz - vec3(0,0,0));*/

    /*if(dist < speed * time){
    	fs_normal = normalize(vec3(-direction.x, 1.0, -direction.z));
    	newPos.y = amp * sin(frequency * time);
    }*/