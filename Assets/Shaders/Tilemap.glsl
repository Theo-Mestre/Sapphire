#type vertex
#version 330 core

layout (location = 0) in vec2 a_position;

uniform mat4 u_viewProjection;
uniform float2 u_tileSize;
uniform float2 u_mapSize;

out vec2 v_globalTexCoords;

void main()
{
	v_globalposition = u_viewProjection * vec4(a_position, 1.0);

	gl_Position = vec4(a_position, 1.0);
}

#type fragment
#version 330 core

int MAX_LAYER = 4;

struct Tile
{
	int textureIndex;
	float2 texcoords;
}; // align: 8

struct Layer
{
	Layer layers[MAX_LAYER];
	sampler2D textures;
}; // align: 16

in vec2 v_globalTexCoords;

layout (std140, binding = 2) uniform TileMap 
{
    int layerCount;
    Layer layers[MAX_LAYER];
}; // align: 16


void main()
{
	int2 tileCoords = int2(v_globalTexCoords / u_tileSize);
	int tileIndex = int(tileCoords.x + tileCoords.y * u_mapSize.x);

	vec4 texColor = vec4(0.0f);
	for (int i = 0; i < layerCount; i++)
	{
		Tile tile = layers[i].layers[tileIndex];
		if (tile.textureIndex != -1)
		{
			texColor += texture(layers[i].textures, tile.texcoords);
		}
	}

	color = texColor;
}