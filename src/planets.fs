#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture_diffuse1;
uniform vec3 lightPos;
uniform vec3 lightColor;

void main() {

  // ambient
  float ambientStrength = 0.03;
  vec3 ambient = ambientStrength * lightColor;

  // diffuse
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(lightPos - FragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * lightColor;
  vec4 result = vec4((diffuse + ambient), 1.0);

  FragColor = result * texture(texture_diffuse1, TexCoords);
}