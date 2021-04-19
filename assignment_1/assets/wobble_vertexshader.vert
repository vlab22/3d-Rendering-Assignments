#version 330

in vec3 vertex;
in vec3 color;

uniform vec2 offset;
uniform mat4 trans;
uniform float time;

out vec3 fColor;


void main (void) {
    vec3 new_pos = vertex * (1.0 + 0.20 * sin(vertex.x*25 + vertex.y*25 + 10.0 * time));
    //vec3 new_pos = vertex;
    gl_Position = trans * vec4(new_pos, 1) + vec4(offset,0,0);
    fColor = color;
}
