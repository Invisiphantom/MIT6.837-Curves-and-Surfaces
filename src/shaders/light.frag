#version 420
in vec4 var_Color;
in vec3 var_Normal;
in vec3 var_Position;

uniform vec3 camPos;

uniform vec4 diffColor;
uniform vec4 specColor;
uniform float shininess;

uniform vec4 lightPos;
uniform vec4 lightDiff;

layout(location=0) out vec4 out_Color;

// shaders can have #defines, too
#define PI_INV 0.318309886183791

vec4 blinn_phong() {
    // Implement Blinn-Phong Shading Model
    // 1. Convert everything to world space
    //    and normalize directions
    vec4 pos_world = vec4(var_Position, 1);
    vec3 normal_world = normalize(var_Normal);
    pos_world /= pos_world.w;
    vec3 light_dir = (lightPos - pos_world).xyz;
    vec3 cam_dir = camPos - pos_world.xyz;
    float distsq = dot(light_dir, light_dir);
    light_dir = normalize(light_dir);
    cam_dir = normalize(cam_dir);

    // 2. Compute Diffuse Contribution
    float ndotl = max(dot(normal_world, light_dir), 0.0);
    vec3 diffContrib = PI_INV * lightDiff.xyz * diffColor.xyz
                       * ndotl / distsq;

    // 3. Compute Specular Contribution
    vec3 halfVec = light_dir + cam_dir;
    vec3 R = reflect( -light_dir, normal_world );
    float eyedotr = max(dot(cam_dir, R), 0.0);
    vec3 specContrib = pow(eyedotr, shininess) *
                       specColor.xyz * lightDiff.xyz / distsq;
                       
    // 4. Add specular and diffuse contributions
    return vec4(diffContrib + specContrib, diffColor.w);
}

void main () {
    out_Color = blinn_phong();
}