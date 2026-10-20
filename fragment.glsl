#version 440 core
out vec4 pixel_color;
uniform int max_iterations;
uniform dvec2 plot_bottom_left;
uniform dvec2 plot_sizes;
uniform dvec2 screen_sizes;

dvec2 complex_square(dvec2 complex) {
    return dvec2(
        complex.x*complex.x - complex.y*complex.y,
        2.0*complex.x*complex.y
    );
}

bool has_escaped (dvec2 complex) {
    return complex.x*complex.x + complex.y*complex.y > 4;
}

vec3 hsv2rgb(vec3 c) {
    // source: https://github.com/hughsk/glsl-hsv2rgb
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

vec4 calculate_color(int iterations) {
    float scale = 1.0*iterations / max_iterations;
    vec3 rgb = hsv2rgb(vec3(pow(scale, 2) , 1.0, 1.0));
    return vec4(rgb, 1.0);
}

void main() {
    const dvec2 c = plot_sizes *gl_FragCoord.xy / screen_sizes + plot_bottom_left;
    dvec2 z = vec2(0.0, 0.0);
    for (int iterations = 0; iterations < max_iterations; iterations++) {
        z = complex_square(z) + c;
        if (has_escaped(z)) {
            pixel_color = calculate_color(iterations);
            return;
        }
    }

    pixel_color = vec4(0, 0, 0, 1);
}