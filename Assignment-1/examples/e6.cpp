#include "../src/a1.hpp"

namespace R = COL781::Software;
// namespace R = COL781::Hardware;
using namespace glm;

int main() {
	R::Rasterizer r;
    if (!r.initialize("Example 2", 640, 480))
        return EXIT_FAILURE;
    R::ShaderProgram program = r.createShaderProgram(
        r.vsColor(),
        r.fsIdentity()
    );  
    vec4 vertices[] = {
		vec4( 0.0, -0.8, 0.0, 1.0),
        vec4( 0.8,  0.0, 0.0, 1.0),
        vec4( 0.0,  0.8, 0.0, 1.0),
        vec4(-0.8,  0.0, 0.0, 1.0)
    };
    vec4 colors[] = {
        vec4(0.5, 0.5, 0.5, 1.0),
        vec4(0.5, 0.5, 0.5, 1.0),
        vec4(0.5, 0.5, 0.5, 1.0),
        vec4(0.5, 0.5, 0.5, 1.0)
    };
	ivec3 triangles[] = {
		ivec3(0, 1, 2),
		ivec3(0, 2, 3)
	};
    R::Object shape = r.createObject();
	r.setVertexAttribs(shape, 0, 4, vertices);
	r.setVertexAttribs(shape, 1, 4, colors);
	r.setTriangleIndices(shape, 2, triangles);
    r.enableDepthTest();
	mat4 view = translate(mat4(1.0f), vec3(0.0f, 0.0f, -10.0f)); 
    mat4 projection = perspective(radians(60.0f), (float)480/(float)640, 0.1f, 100.0f);
    mat4 vp = projection * view;
    mat4 rot = rotate(mat4(1.0f), pi<float>() / 2.0f - (float) atan(1 / sqrt(2.0)), vec3(1.0, 0.0, 0.0)) * rotate(mat4(1.0f), radians(-45.0f), vec3(0.0, 1.0, 0.0));

    while (!r.shouldQuit()) {
            r.clear(vec4(1.0, 1.0, 1.0, 1.0));
            r.useShaderProgram(program);
            // r.setUniform(program, "transform", vp * rot);
            r.drawObject(shape);
            r.show();
        }

    r.deleteShaderProgram(program);
    return EXIT_SUCCESS;
}
