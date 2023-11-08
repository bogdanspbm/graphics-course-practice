//
// Created by Bogdan Madzhuga on 08.11.2023.
//

#ifndef HOMEWORK2_SHADOWSHADER_H
#define HOMEWORK2_SHADOWSHADER_H

#endif //HOMEWORK2_SHADOWSHADER_H

const char shadowVertexSource[] =
        R"(#version 330 core
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        uniform float far;
        uniform float near;

        layout (location = 0) in vec3 in_position;
        layout (location = 1) in vec3 in_normal;
        layout (location = 2) in vec2 in_texcoord;
        layout (location = 3) in vec3 in_color;

        out vec3 fragColor;

        void main()
        {
            gl_Position = projection * view * model * vec4(in_position, 1.0);
            float depth = gl_Position.z; // Retrieve the depth value
            depth = (depth - near) / (far - near);

            fragColor = vec3(depth);
        }
)";

const char shadowFragmentSource[] =
        R"(#version 330 core
        in vec3 fragColor;
        layout (location = 0) out vec4 out_color;

        void main()
        {
            out_color = vec4(fragColor, 1.0);
        }
)";