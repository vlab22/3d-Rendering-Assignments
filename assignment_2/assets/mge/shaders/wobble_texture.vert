//DIFFUSE TEXTURE VERTEX SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform    mat4    mvpMatrix;

in vec3 vertex;
in vec3 normal;
in vec2 uv;

out vec2 texCoord;//make sure the texture coord is interpolated

uniform float strength;
uniform float time;

void main(void){

    vec3 new_pos = vertex * (1.0 + 0.20 * sin(vertex.x * strength + vertex.y * strength + vertex.z * strength + 10.0 * time));
    gl_Position = mvpMatrix * vec4(new_pos, 1.f);
    texCoord = uv;
}
