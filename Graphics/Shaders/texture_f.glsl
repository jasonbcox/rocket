
#version 150

in vec3 interp_normal;
in vec2 uvcoord;

uniform float uniform_alphaTest = 0.0f;
uniform float uniform_alphaTransparency = 1.0f;
uniform vec3 uniform_color = vec3( 1.0, 0.0, 0.0);

uniform vec2 textureScale = vec2( 1.0f, 1.0f );
uniform vec2 textureOffset = vec2( 0.0f, 0.0f );
uniform sampler2D textureMap;

out vec4 fragColor;

void main(void) {
	vec4 texel = texture2D( textureMap, uvcoord * textureScale + textureOffset );

	// Alpha-blending Transparency
	texel.a *= uniform_alphaTransparency;

	// Alpha-test Transparency
	if (texel.a <= uniform_alphaTest) discard;

	fragColor = vec4( uniform_color, 1.0 ) * texel;
}
