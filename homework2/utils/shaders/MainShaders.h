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
out vec3 inputNormal;
out vec2 texCoord;
out vec4 shadowPosition;
out float depth;

void main()
{
    shadowPosition =  projection *  sun_view * model  * vec4(in_position, 1.0);
    gl_Position = projection * view * model * vec4(in_position, 1.0);
    inputNormal = normalize(mat3(model) * in_normal);
    fragColor = in_color;
    texCoord = in_texcoord;
    depth = (shadowPosition.z - near)/(far-near);
}
)";

const char mainFragmentSource[] =
        R"(#version 330 core
        in vec3 inputNormal;
        in vec3 fragColor;
        in vec4 shadowPosition;
        in vec2 texCoord;
        in float depth;

        uniform float roughness;
        uniform float glossiness;

        uniform int texturesCount;

        uniform sampler2D texture0;
        uniform sampler2D texture1;
        uniform sampler2D texture2;
        uniform sampler2D texture3;
        uniform sampler2D texture4;
        uniform sampler2D texture5;
        uniform sampler2D texture6;
        uniform sampler2D texture7;
        uniform sampler2D texture8;
        uniform sampler2D texture9;

        uniform sampler2D shadow_map;

        uniform vec3 albedo;
        uniform vec3 inputAmbientLight;
        uniform vec3 inputSunColor;
        uniform vec3 inputSunDirection;
        uniform vec3 inputViewDirection;
        uniform vec3 inputViewPosition;

        struct PointLight {
            vec3 position;
            vec3 color;
            vec3 attenuation;
        };

        #define NR_POINT_LIGHTS 16
        uniform PointLight pointLights[NR_POINT_LIGHTS];

        layout (location = 0) out vec4 out_color;

        vec3 diffuse(vec3 direction, vec3 normal) {
            return albedo * max(0.0, dot(normal, direction));
        }

        vec3 specular(vec3 direction, vec3 normal) {
            float power = 1.0 / (roughness * roughness) - 1.0;

            vec3 viewDirection  =  inputViewDirection;
            if(viewDirection == vec3(0)){
                viewDirection = vec3(0,0,-1);
            }

            vec3 normalizedViewDirection = normalize(viewDirection);
            vec3 reflected = reflect(-direction, normal);

            return glossiness * albedo * pow(max(0.0, dot(reflected, normalizedViewDirection)), power);
        }

        vec3 CalcPointLight(PointLight light, vec3 normal)
        {
            vec3 point_light_dir = light.position - inputViewPosition;

            float attenuation = dot(light.attenuation, vec3(1.0, length(point_light_dir), length(point_light_dir) * length(point_light_dir)));

            vec3 point_light_extra = (diffuse(normalize(point_light_dir),normal) + specular(normalize(point_light_dir),normal)) * light.color * attenuation;

            return point_light_extra;
        }

        void main()
        {

            vec4 textureColor = texture(texture0, texCoord);

            vec3 normalMapColor = texture(texture1, texCoord).xyz * 2.0 - 1.0;
            vec3 perturbedNormal = normalize(inputNormal + normalMapColor);

            vec3 defaultColor = textureColor.xyz;

            if(texturesCount == 0){
                defaultColor = albedo;
            }

            vec3 ambientLight = inputAmbientLight;

            if(ambientLight == vec3(0)){
                ambientLight = vec3(0.8);
            }

            vec3 ambient = defaultColor * ambientLight;

            vec3 sunColor = inputSunColor;
            if(sunColor == vec3(0)){
                sunColor = vec3(0.8);
            }

            vec3 sunDirection = inputSunDirection;
            if(sunDirection == vec3(0)){
                sunDirection = vec3(0,0.8,0.6);
            }

            vec3 sunExtra = (diffuse(sunDirection, perturbedNormal) + specular(sunDirection, perturbedNormal)) * sunColor;

            vec3 color = ambient + sunExtra;

            //for(int i = 0; i < NR_POINT_LIGHTS; i++){
            //        color += CalcPointLight(pointLights[i],perturbedNormal);
            //}

            vec3 shadowTextCoord = shadowPosition.xyz / shadowPosition.w;
            shadowTextCoord = shadowTextCoord * 0.5 + 0.5;
            vec4 depthValue = texture(shadow_map, shadowTextCoord.xy);
            float isVisible = (depth < depthValue.r + 0.01) ? 1.0 : 0.0;;

            if(isVisible > 0.5){
                  out_color = vec4(color, 0.5);
            } else {
                out_color = vec4(color * 0.5, 0.5);
            }

            out_color = vec4(color,1);

        }
)";