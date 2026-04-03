#version 330 core
out vec4 fragColor;

uniform vec4 uColor;   // per-shape color

void main() {
    fragColor = uColor;
}
