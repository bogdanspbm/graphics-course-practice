//
// Created by Bogdan Madzhuga on 08.11.2023.
//

#ifndef HOMEWORK2_SCREENVIEWSHADER_H
#define HOMEWORK2_SCREENVIEWSHADER_H

#endif //HOMEWORK2_SCREENVIEWSHADER_H

const char screenViewVertexSource[] =
        R"(#version 330 core

        uniform vec2 center;
        uniform vec2 size;

        vec2 vertices[6] = vec2[6](
            vec2(-1.0, -1.0),
            vec2( 1.0, -1.0),
            vec2( 1.0,  1.0),
            vec2(-1.0, -1.0),
            vec2( 1.0,  1.0),
            vec2(-1.0,  1.0)
        );

        out vec2 texcoord;

        void main()
        {
            vec2 vertex = vertices[gl_VertexID];
            gl_Position = vec4((vertex * size) + center, 0.0, 1.0);
            texcoord = vertex * 0.5 + vec2(0.5);
        }
)";

const char screenViewFragmentSource[] =
        R"(#version 330 core

        uniform sampler2D texture31;

        in vec2 texcoord;

        layout (location = 0) out vec4 out_color;

        void main()
        {
            out_color = vec4(texture(texture31, texcoord).rrr, 1.0);
        }
)";