
// fragment shader

#version 460

uniform vec3 resolution;
uniform sampler2D channel0;
uniform sampler2D channel1;
out vec4 outputColor;

void main()
{
    vec2 uv = vec2(gl_FragCoord.xy / resolution.xy);
    outputColor = 5.* texture2D(channel0, uv) + 0.75 * texture2D(channel1, uv);
}
