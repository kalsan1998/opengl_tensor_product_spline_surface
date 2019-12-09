#version 330 core
in vec3 fragment_color;
in vec3 fragment_norm;
in vec3 fragment_pos;

out vec4 color;
void main() {
  vec3 light_color = vec3(1.0, 1.0, 1.0);

  float ambient_strength = 0.4;
  vec3 ambient = ambient_strength * light_color;

  vec3 light_pos = vec3(2.0, 2.0, 2.0);
  vec3 norm = normalize(fragment_norm);
  vec3 light_direction = normalize(light_pos - fragment_pos);
  float diff = max(dot(norm, light_direction), 0.0);
  vec3 diffuse = diff * light_color;

  color = vec4((ambient + diffuse) * fragment_color, 1.0);
}