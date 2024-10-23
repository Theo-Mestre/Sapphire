#type vertex
#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texCoord;

uniform mat4 u_viewProjection;
uniform mat4 u_transform;

out vec2 v_texCoords;

void main()
{
    v_texCoords = a_texCoord;

	gl_Position = u_viewProjection * u_transform * vec4(a_position, 1.0);
}

#type fragment
#version 420 core

layout (std140, binding = 0) uniform AppData 
{
    vec2 u_resolution;
};

struct Light 
{
    vec3 color;
    vec2 position;
    float intensity;
    float radius;
};


layout (std140, binding = 1) uniform LightBlock 
{
    int lightsCount;
    Light lights[100];
};

uniform sampler2D u_texture;
uniform vec3 u_ambientLight;
uniform mat4 u_sceneVP;

layout(location = 0) out vec4 color;

in vec2 v_texCoords;


void main()
{
    vec4 texColor = texture(u_texture, v_texCoords);

    vec3 finalColor = u_ambientLight * texColor.rgb;

    for (int i = 0; i < lightsCount; i++) 
    {
        Light light = lights[i];
        vec4 position = u_sceneVP * vec4(light.position, 0, 1);
        position /= position.w;
        position.xy = position.xy * 0.5 + 0.5;

        vec2 lightDir = position.xy - v_texCoords;
        lightDir.x *= u_resolution.x / u_resolution.y;

        float distance = length(lightDir);
        
        if (distance > light.radius)
            continue;

        if (distance < 0.01)
        {
            color = vec4(1, 0, 1, 1);
            return;
        }

        float attenuation = clamp(1.0 - (distance / light.radius), 0.0, 1.0);

        float intensity = attenuation * light.intensity;
        vec3 lightEffect = intensity * light.color;
    
        finalColor += lightEffect * texColor.rgb;
    }

    color = vec4(finalColor, texColor.a);
}