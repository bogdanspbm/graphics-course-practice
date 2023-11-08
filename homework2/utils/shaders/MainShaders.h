//
// Created by Bogdan Madzhuga on 08.11.2023.
//

#ifndef HOMEWORK2_MAINSHADERS_H
#define HOMEWORK2_MAINSHADERS_H

#endif //HOMEWORK2_MAINSHADERS_H

const char mainVertexSource[] =
        R"(#version 330 core

uniform mat4 model;
uniform mat4 sun_view;
uniform mat4 projection;
uniform mat4 view;
uniform float far;
uniform float near;

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texcoord;
layout (location = 3) in vec3 in_color;

out vec3 fragColor;
out vec3 normal;
out vec2 texcoord;
out vec4 shadowPosition;
out float depth;

void main()
{
    shadowPosition =  projection *  sun_view * model  * vec4(in_position, 1.0);
    gl_Position = projection * view * model * vec4(in_position, 1.0);
    normal = normalize(mat3(model) * in_normal);
    fragColor = in_color;
    texcoord = in_texcoord;
    depth = (shadowPosition.z - near)/(far-near);
}
)";

const char mainFragmentSource[] =
        R"(#version 330 core
        in vec3 normal;
        in vec3 fragColor;
        in vec4 shadowPosition;
        in vec2 texcoord;
        in float depth;

        uniform sampler2D shadow_map;
        uniform sampler2D textureLayer;
        uniform vec3 albedo;
        uniform vec3 ambient_light;
        uniform vec3 sun_color;
        uniform vec3 sun_direction;
        uniform vec3 view_direction;
        uniform vec3 view_position;
        uniform float roughness;
        uniform float glossiness;

        struct PointLight {
            vec3 position;
            vec3 color;
            vec3 attenuation;
        };

        #define NR_POINT_LIGHTS 16
        uniform PointLight pointLights[NR_POINT_LIGHTS];

        layout (location = 0) out vec4 out_color;

        vec3 diffuse(vec3 direction) {
            return albedo * max(0.0, dot(normal, direction));
        }

        vec3 specular(vec3 direction) {
            float power = 1.0 / (roughness * roughness) - 1.0;
            vec3 normalized_view = normalize(view_direction);
            vec3 reflected = reflect(-direction, normal);

            return glossiness * albedo * pow(max(0.0, dot(reflected, normalized_view)), power);
        }

        vec3 CalcPointLight(PointLight light)
        {
            vec3 point_light_dir = light.position - view_position;

            float attenuation = dot(light.attenuation, vec3(1.0, length(point_light_dir), length(point_light_dir) * length(point_light_dir)));

            vec3 point_light_extra = (diffuse(normalize(point_light_dir)) + specular(normalize(point_light_dir))) * light.color * attenuation;

            return point_light_extra;
        }

        void main()
        {
            vec3 ambient = albedo * ambient_light;

            vec3 sun_extra = (diffuse(sun_direction) + specular(sun_direction)) * sun_color;

            vec3 color = ambient + sun_extra;

            for(int i = 0; i < NR_POINT_LIGHTS; i++){
                    color += CalcPointLight(pointLights[i]);
            }

            vec3 shadowTextCoord = shadowPosition.xyz / shadowPosition.w;
            shadowTextCoord = shadowTextCoord * 0.5 + 0.5;
            vec4 depthValue = texture(shadow_map, shadowTextCoord.xy);
            float isVisible = (depth < depthValue.r + 0.01) ? 1.0 : 0.0;;

            if(isVisible > 0.5){
                //out_color = depthValue;
                //out_color = vec4(vec3(shadowTextCoord.z),1);
                //out_color = depth;
                  out_color = vec4(color, 0.5);
            } else {
                out_color = vec4(color * 0.5, 0.5);
            }

        }
)";