#version 330 core

out vec4 FragColor;

in vec3 color;
in vec2 texCoord;

uniform sampler2D tex0;
uniform bool useTexture;

void main()
{
    if (useTexture)
        FragColor = texture(tex0, texCoord);
    else
        FragColor = vec4(color, 1.0);
}
