#shader vertex
#version 330 core

// Access data from the memory layout where the vertex buffer are stored in the GPU
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

// outputs data to the next stage in the pipeline
out vec2 v_TexCoord;

// Uniforms loads data directly from the CPU
uniform mat4 u_MVP; // Model View Projection matrix

void main()
{
    gl_Position = u_MVP * position; // apply the transforms
    // This vertex program runs for each and every vertex in the vertex buffer
    // and then sets defining coordinate that will be used to compute the mesh
    // and finally render pixels in the fragment shader.
    v_TexCoord = texCoord;
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

// takes data from the last stage in the pipeline (the vertex shader)
in vec2 v_TexCoord;

// loads data from the CPU
//uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
    vec4 texColor = texture(u_Texture, v_TexCoord); // samples the texture coordinate
    //color = u_Color;  // outputs the custom color uniform to the vertex buffer
    color = texColor; // outputs the sampled color to the vertex buffer
}
