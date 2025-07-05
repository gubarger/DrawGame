#version 330 core

out vec4 FragColor;
uniform vec3 color;

void main() {
    FragColor = vec4(color, 1.0);
    gl_FragDepth = gl_FragCoord.z - 0.0001;
}