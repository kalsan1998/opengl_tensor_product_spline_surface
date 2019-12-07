#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec3 color;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragment_color;
out vec3 fragment_norm;
out vec3 fragment_pos;

void main() {
    gl_Position = projection * view * model * vec4(pos, 1.0);
    fragment_color = vec3(1.0, 0.0, 0.0);
    fragment_norm = vec3(model * vec4(norm, 1.0));
    fragment_pos = vec3(model * vec4(pos, 1.0));
}