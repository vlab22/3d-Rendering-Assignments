#version 330

uniform	mat4 mvpMatrix;
uniform sampler2D heightMapTexture;
uniform float heightFactor;

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
   
    vec4 heightMap_color = texture(heightMapTexture,uv);

	vec3 newHeight = vec3(vertex.x, vertex.y + length(heightMap_color) * heightFactor, vertex.z);

	gl_Position = mvpMatrix * vec4(newHeight, 1.f);
}