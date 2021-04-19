#version 330

in vec3 fColor;

uniform vec2 windowSize;
uniform uint rows;
uniform uint columns;
uniform vec2 offset;
uniform vec2 scale;
uniform mat4 trans;

out vec4 sColor;

void main (void) {

    float x = gl_FragCoord.x - windowSize.x * 0.5 * offset.x;
    float y = gl_FragCoord.y - windowSize.y * 0.5 * offset.y;

    float scalex = scale.x;

    float rowPixels = windowSize.y / (rows * scalex);
    float columnPixels = windowSize.x / (columns * scalex);

    int row = int(floor(y / rowPixels));
    int column = int(floor(x / columnPixels));

    if (mod(row + column, 2) == 0.0) {
        sColor = vec4(0,0,0,1);
    }
    else {
        sColor = vec4(1,1,1,1);
    }
}



