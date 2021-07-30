#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

out vec4 finalColor;

//based on this: https://github.com/Hapaxia/Lens/blob/master/Lens/blendColor.frag
//modified for raylib

void main()
{
	vec4 pixel = texture(texture0, fragTexCoord.xy) * colDiffuse;

	finalColor = vec4((fragColor + pixel).rgb, pixel.a);
}