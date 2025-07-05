#version 330 core

in vec3 Normal;
in vec3 FragPos;

// Output color
out vec4 FragColor;

// Uniform
uniform vec3 objectColor = vec3(0.5, 0.5, 0.5);

struct SunLight {
    vec3 direction;
    vec3 color;
    float intensity;
};

uniform SunLight sunLight;

vec3 CalculateSunLight(vec3 normal) {
    // diffuse component
    float diff = max(dot(normal, normalize(-sunLight.direction)), 0.0);
    vec3 diffuse = diff * sunLight.color * sunLight.intensity;
    
    // Hemispherical lighting
    float hemisphere = 0.5 + 0.5 * dot(normal, vec3(0.0, 1.0, 0.0));
    vec3 ambient = mix(vec3(0.3), vec3(0.7), hemisphere);
    
    return ambient + diffuse;
}

void main() {
    vec3 norm = normalize(Normal);
    vec3 lightResult = CalculateSunLight(norm);

    FragColor = vec4(lightResult * objectColor, 1.0);
}