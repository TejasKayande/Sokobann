#version 330

in vec3 fragPosition;
in vec3 fragNormal;
in vec4 fragColor;

uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform vec3 ambientColor;

void main() {

    vec3 normal = normalize(fragNormal);
    vec3 direction = normalize(-lightDirection);

    float diffuse = max(dot(normal, direction), 0.0);

    vec3 lighting = ambientColor + lightColor * diffuse;

    gl_FragColor = vec4(fragColor.rgb * lighting, fragColor.a);
}