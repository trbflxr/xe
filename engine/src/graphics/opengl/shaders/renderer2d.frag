layout(location = 0) out vec4 color;

in vec2 v_texCoords;
in vec4 v_color;

uniform sampler2D u_tex2d0;

void main() {
    color = texture(u_tex2d0, v_texCoords) * v_color;
}
