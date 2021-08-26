#version 330 core
#extension GL_ARB_shading_language_420pack : enable
out vec4 pixel_color;
uniform int max_iterations;
uniform vec2 plot_bottom_left;
uniform vec2 plot_sizes;
uniform vec2 screen_sizes;

vec2 complex_square(vec2 complex) {
    return vec2(
        complex.x*complex.x - complex.y*complex.y,
        2.0*complex.x*complex.y
    );
}

bool has_escaped (vec2 complex) {
    return complex.x*complex.x + complex.y*complex.y > 4;
}

vec4 calculate_color(int iterations) {
    float scale = 0.05+0.95*(log(iterations) / log(max_iterations));
    return vec4(scale , scale , 1.0 - scale , 1.0);
}

void main() {
    const vec2 c = plot_sizes *gl_FragCoord.xy / screen_sizes + plot_bottom_left;
    vec2 z = vec2(0.0, 0.0);
    for (int iterations = 0; iterations < max_iterations; iterations++) {
        z = complex_square(z) + c;
        if (has_escaped(z)) {
            pixel_color = calculate_color(iterations);
            return;
        }
    }

    pixel_color = vec4(0.0, 0.0, 0.0, 1.0);
}