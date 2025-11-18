#version 330 core
layout (location = 0) in vec2 aPos;

const int ARRAY_LIMIT = 128;

uniform mat4 projection;
uniform mat4 transforms[ARRAY_LIMIT];
uniform int letterMap[ARRAY_LIMIT];

out vec3 vTexCoord;

void main()
{
    gl_Position = projection * transforms[gl_InstanceID] * vec4(aPos.xy, 0.0, 1.0);
    
    vTexCoord = vec3(aPos.xy, float(letterMap[gl_InstanceID]));
}
