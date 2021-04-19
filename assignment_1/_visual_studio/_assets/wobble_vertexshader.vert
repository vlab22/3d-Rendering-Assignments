#version 330

in vec3 vertex;
in vec3 color;
uniform vec2 offset;
uniform float time;

out vec3 fColor;

void main (void) {
    vec3 new_pos = vertex * (1.0 + 0.20 * sin(vertex.x + vertex.y + 10.0 * time));
    gl_Position = vec4(new_pos,1) + vec4(offset,0,0);
    fColor = color;
}
