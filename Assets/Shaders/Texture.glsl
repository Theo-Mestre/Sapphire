#type vertex
#version 450 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_texCoord;
layout(location = 3) in float a_texIndex;
layout(location = 4) in float a_tilingFactor;
layout(location = 5) in int a_entityID;

uniform mat4 u_viewProjection;

out vec2 v_texCoord;
out vec4 v_color;
out float v_texIndex;
out float v_tilingFactor;
out flat int v_entityID;

void main()
{
	v_color = a_color;
	v_texCoord = a_texCoord;
	v_texIndex = a_texIndex;
	v_tilingFactor = a_tilingFactor;
	v_entityID = a_entityID;

	gl_Position = u_viewProjection * vec4(a_position, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 color;
layout(location = 1) out int entityIDMap;

in vec2 v_texCoord;
in vec4 v_color;
in float v_texIndex;
in float v_tilingFactor;
in flat int v_entityID;

uniform sampler2D u_textures[32];

void main()
{
	int intTexIndex = int(round(v_texIndex));

	vec4 texColor = texture(u_textures[intTexIndex], v_texCoord * v_tilingFactor);
	color = v_color * texColor;

	entityIDMap = v_entityID;
}