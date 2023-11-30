#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D myTexture1;
uniform sampler2D myTexture2;

uniform vec3 colorcube;

void main()
{
	vec4 texColor = texture(myTexture1, TexCoord);
    if(texColor.a < 0.05)
        discard;
    FragColor = texColor;
}c();