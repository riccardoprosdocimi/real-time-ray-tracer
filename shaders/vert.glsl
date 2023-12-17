#version 410 core
// Read in our attributes stored from our vertex buffer object.
// We explicitly state which is the vertex information
// (The first 3 floats are positional data we are putting in our vector)
layout(location = 0) in vec2 position; // Our first attribute - vertex coordinates
layout(location = 1) in vec2 texCoord; // Our second attribute - texture coordinates

void main()
{
    gl_Position = vec4(position.x, position.y, 0.0f, 1.0f);
}
