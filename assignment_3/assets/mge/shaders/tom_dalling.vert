#version 330

uniform	mat4 mvpMatrix;

in vec3 vertex;
in vec3 normal;
in vec2 uv;

out vec3 fragVert;
out vec2 fragTexCoord;
out vec3 fragNormal;

void main() {
    // Pass some variables to the fragment shader
    fragTexCoord = uv;
    fragNormal = normal;
    fragVert = vertex;
    
    // Apply all matrix transformations to vert
    gl_Position = mvpMatrix * vec4(vertex, 1.0);
}