in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

layout (location = 0) out vec3 g_position;
layout (location = 1) out vec4 g_normal_shininess;
layout (location = 2) out vec4 g_albedo_specular;

uniform float shininess;

uniform sampler2D wind_map;
uniform sampler2D diffuse_map;

const float SPECULAR = 0.1;

void main()
{
    g_position.rgb = FragPos;

    g_normal_shininess.rgb = Normal;
    g_normal_shininess.a = shininess;
    g_albedo_specular.rgb = mix(texture(diffuse_map, TexCoord).rgb,
                                0.8*normalize(vec3(0.32, 0.7, 0.2)), 0.3);
    g_albedo_specular.a = SPECULAR;
}
