#version 420
// These are vertex attributes.
// You can define custom attributes,
// like color or curvature.
layout(location=0) in vec3 Position;
layout(location=1) in vec3 Normal;
layout(location=2) in vec3 Color;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform mat4 N;

// var_ (varying) variables are output in the vertex
// shader and are interpolated by the GPU for each
// pixel of the triangle.
out vec3 var_Position;
out vec3 var_Normal;
out vec4 var_Color;

void main () {
    // Simple pass-through vertex shader
    vec4 position_world = M * vec4(Position, 1);
    gl_Position = P * V * M * vec4(Position, 1);
    var_Position = position_world.xyz / position_world.w;

    vec3 normal_world = (N * vec4(Normal, 1)).xyz;
    var_Normal = normalize(normal_world);
    var_Color = vec4(Color, 1);
}