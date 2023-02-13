#include "../src/a1.hpp"

namespace R = COL781::Software;
using namespace glm;

int main() {
    R::Rasterizer r;
    if (!r.initialize("2x2 Rubik's Cube", 640, 480))
        return EXIT_FAILURE;

    R::ShaderProgram program = r.createShaderProgram(
        r.vsIdentity(),
        r.fsConstant()
    );

    vec4 vertices[] = {
        vec4(-0.8, -0.8,  0.8, 1.0), // 0
        vec4(-0.8,  0.8,  0.8, 1.0), // 1
        vec4( 0.8,  0.8,  0.8, 1.0), // 2
        vec4( 0.8, -0.8,  0.8, 1.0), // 3
        vec4(-0.8, -0.8, -0.8, 1.0), // 4
        vec4(-0.8,  0.8, -0.8, 1.0), // 5
        vec4( 0.8,  0.8, -0.8, 1.0), // 6
        vec4( 0.8, -0.8, -0.8, 1.0)  // 7
    };

    ivec3 triangles[] = {
        // Front
        ivec3(0, 1, 2),
        ivec3(0, 2, 3),
        // Back
        ivec3(4, 7, 6),
        ivec3(4, 6, 5),
        // Left
        ivec3(0, 4, 5),
        ivec3(0, 5, 1),
        // Right
        ivec3(2, 6, 7),
        ivec3(2, 7, 3),
        // Top
        ivec3(1, 5, 6),
        ivec3(1, 6, 2),
        // Bottom
        ivec3(3, 7, 4),
        ivec3(3, 4, 0)
    };

    R::Object cube = r.createObject();
    r.setVertexAttribs(cube, 0, 8, vertices);
    r.setTriangleIndices(cube, 12, triangles);

    while (!r.shouldQuit()) {
        r.clear(vec4(1.0, 1.0, 1.0, 1.0));
        r.useShaderProgram(program);

        // Draw each face with a different color
        r.setUniform<vec4>(program, "color", vec4(1.0, 0.0, 0.0, 1.0));
        r.drawObject(cube);
        r.setUniform<vec4>(program, "color", vec4(0.0, 1.0, 0.0, 1.0));
        r.drawObject(cube);
        r.setUniform<vec4>(program, "color", vec4(0.0, 0.0, 1.0, 1.0));
        r.drawObject(cube);
        r.setUniform<vec4>(program, "color", vec4(1.0, 1.0, 0.0, 1.0));
        r.drawObject(cube);
        r.setUniform<vec4>(program, "color", vec4(0.0, 1.0, 1.0, 1.0));
        r.drawObject(cube);
        r.setUniform<vec4>(program, "color", vec4(1.0, 0.0, 1.0, 1.0));
        r.drawObject(cube);
    }
    r.deleteShaderProgram(program);
    return EXIT_SUCCESS;
}
