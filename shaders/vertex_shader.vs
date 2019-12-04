#version 330 core
layout(location = 0) in vec3 pos;
uniform mat4 model_view_projection;

void main() {
    gl_Position = model_view_projection * vec4(pos, 1.0);
}