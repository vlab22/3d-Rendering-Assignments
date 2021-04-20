//DIFFUSE TEXTURE VERTEX SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform	mat4 	mvpMatrix;
uniform sampler2D heightMapTexture;
uniform float heightFactor;

in vec3 vertex;
in vec3 normal;
in vec2 uv;

out vec2 texCoord; //make sure the texture coord is interpolated

void main( void ){

	vec4 heightMap_color = texture(heightMapTexture,uv);

	texCoord = uv;

	vec3 newHeight = vec3(vertex.x, vertex.y + length(heightMap_color) * heightFactor, vertex.z);

	gl_Position = mvpMatrix * vec4(newHeight, 1.f);
}
