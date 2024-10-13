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
#version 330 core

struct Light 
{
    vec2 position;
    vec3 color;
    float intensity;
    float radius;
};

//layout (std140)
uniform LightBlock 
{
    Light lights;
    int numLights;
};

uniform sampler2D u_texture;
uniform vec3 u_ambientLight;

layout(location = 0) out vec4 color;

in vec2 v_texCoords;

void main()
{
    vec4 texColor = texture(u_texture, v_texCoords);
    vec3 finalColor = u_ambientLight * texColor.rgb;

    //for (int i = 0; i < numLights; i++) 
    //{
        Light light = lights;//lights[i];
        vec2 lightDir = light.position - v_texCoords;
        float distance = length(lightDir);
        
        float attenuation = clamp(1.0 - (distance / light.radius), 0.0, 1.0);

        float intensity = attenuation * light.intensity;
        vec3 lightEffect = intensity * light.color;
    
        finalColor += lightEffect * texColor.rgb;
    //}

    color = vec4(finalColor, texColor.a); // Output the final color
}