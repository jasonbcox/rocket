
#version 150

in vec4 vert_position;
in vec3 vert_normal;
in vec2 vert_uv;

uniform mat4 transform = mat4(1.0);		// object view
uniform mat4 orientation = mat4(1.0);	// world view (camera)
uniform mat4 perspective = mat4(1.0);	// perspective

out vec2 uvcoord;

void main(void) {
	uvcoord = vert_uv;
	gl_Position = perspective * orientation * transform * vert_position;
}
