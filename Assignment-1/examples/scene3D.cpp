#include "../src/a1.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace R = COL781::Software;
// namespace R = COL781::Hardware;
using namespace glm;

int main() {
	R::Rasterizer r;
	int width = 640, height = 480;
    if (!r.initialize("Cube Land", width, height))
        return EXIT_FAILURE;

    R::ShaderProgram program = r.createShaderProgram(
        r.vsColorTransform(),
        r.fsIdentity()
    );

    R::ShaderProgram program_color = r.createShaderProgram(
        r.vsTransform(),
        r.fsConstant()
    );

    // Scene ground
    vec4 vertices_ground[] = {
        vec4(-1.0, 0.0, 1.0, 1.0),
        vec4(1.0, 0.0, 1.0, 1.0),
        vec4(1.0, 0.0, -1.0, 1.0),
        vec4(-1.0, 0.0, -1.0, 1.0),
        vec4(-1.0, -0.4, 1.0, 1.0),
        vec4(1.0, -0.4, 1.0, 1.0),
        vec4(1.0, -0.4, -1.0, 1.0),
        vec4(-1.0, -0.4, -1.0, 1.0)
    };
    vec4 colors_ground[] = {
        vec4(0.49, 0.99, 0.0, 1.0),
        vec4(0.49, 0.99, 0.0, 1.0),
        vec4(0.49, 0.99, 0.0, 1.0),
        vec4(0.49, 0.99, 0.0, 1.0),
        vec4(0.38, 0.27, 0.06, 1.0),
        vec4(0.38, 0.27, 0.06, 1.0),
        vec4(0.38, 0.27, 0.06, 1.0),
        vec4(0.38, 0.27, 0.06, 1.0)
    };
    ivec3 triangles_ground[] = {
        ivec3(0, 1, 2),
        ivec3(0, 2, 3),
        ivec3(0, 1, 5),
        ivec3(0, 5, 4),
        ivec3(1, 2, 6),
        ivec3(1, 6, 5),
        ivec3(2, 3, 7),
        ivec3(2, 7, 6),
        ivec3(3, 0, 4),
        ivec3(3, 4, 7),
        ivec3(4, 5, 6),
        ivec3(4, 6, 7)
    };
    R::Object ground = r.createObject();
	r.setVertexAttribs(ground, 0, 8, vertices_ground);
	r.setVertexAttribs(ground, 1, 8, colors_ground);
	r.setTriangleIndices(ground, 12, triangles_ground);

    // Train tracks
    vec4 vertices_tracks[] = {
        vec4(-1.0, 0.5, 1.0, 1.0),
        vec4(1.0, 0.5, 1.0, 1.0),
        vec4(1.0, 0.5, -1.0, 1.0),
        vec4(-1.0, 0.5, -1.0, 1.0)
    };
    vec4 colors_tracks[] = {
        vec4(0.75, 0.75, 0.75, 1.0),
        vec4(0.75, 0.75, 0.75, 1.0),
        vec4(0.75, 0.75, 0.75, 1.0),
        vec4(0.75, 0.75, 0.75, 1.0),
    };
    ivec3 triangles_tracks[] = {
        ivec3(0, 1, 2),
        ivec3(0, 2, 3)
    };
    R::Object track = r.createObject();
	r.setVertexAttribs(track, 0, 4, vertices_tracks);
	r.setVertexAttribs(track, 1, 4, colors_tracks);
	r.setTriangleIndices(track, 2, triangles_tracks);

    // Cube shadow
    vec4 vertices_square[] = {
        vec4(-1, 0.2, 1, 1),
        vec4(1, 0.2, 1, 1),
        vec4(1, 0.2, -1, 1),
        vec4(-1, 0.2, -1, 1),
        vec4(0, 0.2, 0, 1)
    };
    vec4 colors_square[] = {
        vec4(0.49, 0.99, 0.0, 1.0),
        vec4(0.49, 0.99, 0.0, 1.0),
        vec4(0.49, 0.99, 0.0, 1.0),
        vec4(0.49, 0.99, 0.0, 1.0),
        vec4(0.0, 0.40, 0.0, 1.0)
    };
    ivec3 triangles_square[] = {
        ivec3(0, 1, 4),
        ivec3(1, 2, 4),
        ivec3(2, 3, 4),
        ivec3(3, 0, 4)
    };
    R::Object square = r.createObject();
	r.setVertexAttribs(square, 0, 5, vertices_square);
	r.setVertexAttribs(square, 1, 5, colors_square);
	r.setTriangleIndices(square, 4, triangles_square);

    // Tree
    vec4 vertices_tree[] = {
        vec4(-1, 0, 1, 1),
        vec4(1, 0, 1, 1),
        vec4(1, 0, -1, 1),
        vec4(-1, 0, -1, 1),
        vec4(0, 1, 0, 1)
    };
    vec4 colors_tree[] = {
        vec4(0, 0.4, 0, 1),
        vec4(0, 0.4, 0, 1),
        vec4(0, 0.4, 0, 1),
        vec4(0, 0.4, 0, 1),
        vec4(0, 0.4, 0, 1)
    };
    ivec3 triangles_tree[] = {
        ivec3(0, 1, 4),
        ivec3(1, 2, 4),
        ivec3(2, 3, 4),
        ivec3(3, 0, 4)
    };
    R::Object tree = r.createObject();
	r.setVertexAttribs(tree, 0, 5, vertices_tree);
	r.setVertexAttribs(tree, 1, 5, colors_tree);
	r.setTriangleIndices(tree, 4, triangles_tree);

    // Sky
    vec4 vertices_sky[] = {
        vec4(-1, 0, 0, 1),
        vec4(1, 0, 0, 1),
        vec4(1, 1, 0, 1),
        vec4(-1, 1, 0, 1)
    };
    vec4 colors_sky[] = {
        vec4(0.53, 0.81, 0.92, 1),
        vec4(0.53, 0.81, 0.92, 1),
        vec4(0.53, 0.81, 0.92, 1),
        vec4(0.53, 0.81, 0.92, 1)
    };
    ivec3 triangles_sky[] = {
        ivec3(0, 1, 2),
        ivec3(0, 2, 3)
    };
    R::Object sky = r.createObject();
	r.setVertexAttribs(sky, 0, 4, vertices_sky);
	r.setVertexAttribs(sky, 1, 4, colors_sky);
	r.setTriangleIndices(sky, 2, triangles_sky);

    // Train
    vec4 vertices_train[] = {
        vec4(-1.0, 1.0, 1.0, 1.0),
        vec4(1.0, 1.0, 1.0, 1.0),
        vec4(1.0, 1.0, -1.0, 1.0),
        vec4(-1.0, 1.0, -1.0, 1.0),
        vec4(-1.0, 2.0, 1.0, 1.0),
        vec4(1.0, 2.0, 1.0, 1.0),
        vec4(1.0, 2.0, -1.0, 1.0),
        vec4(-1.0, 2.0, -1.0, 1.0)
    };
    ivec3 triangles_train[] = {
        ivec3(0, 1, 2),
        ivec3(0, 2, 3),
        ivec3(0, 1, 4),
        ivec3(1, 4, 5),
        ivec3(1, 2, 5),
        ivec3(2, 5, 6),
        ivec3(2, 3, 6),
        ivec3(3, 6, 7),
        ivec3(3, 0, 7),
        ivec3(0, 7, 4),
        ivec3(4, 5, 6),
        ivec3(4, 6, 7)
    };
    R::Object train = r.createObject();
	r.setVertexAttribs(train, 0, 8, vertices_train);
	r.setTriangleIndices(train, 12, triangles_train);

    // Cube vertices (All 8 cubes have same polygon mesh)
    vec4 vertices[] = {
        vec4(-0.8, -0.8, 0.8, 1.0),
        vec4(0.8, -0.8, 0.8, 1.0),
        vec4(0.8, 0.8, 0.8, 1.0),
        vec4(-0.8, 0.8, 0.8, 1.0),
        vec4(-0.8, 0.8, -0.8, 1.0),
        vec4(-0.8, -0.8, -0.8, 1.0),
        vec4(0.8, -0.8, -0.8, 1.0),
        vec4(0.8, 0.8, -0.8, 1.0),
        vec4(0, 0, 0.8, 1.0),
        vec4(0, 0, -0.8, 1.0),
        vec4(0, 0.8, 0, 1.0),
        vec4(0, -0.8, 0, 1.0),
        vec4(-0.8, 0, 0, 1.0),
        vec4(0.8, 0, 0, 1.0)
    };
    ivec3 triangles[] = {
		ivec3(0, 1, 8),
        ivec3(1, 2, 8),
        ivec3(2, 3, 8),
        ivec3(3, 0, 8),
        ivec3(3, 4, 10),
        ivec3(4, 7, 10),
        ivec3(7, 2, 10),
        ivec3(2, 3, 10),
        ivec3(0, 1, 11),
        ivec3(1, 6, 11),
        ivec3(6, 5, 11),
        ivec3(5, 0, 11),
        ivec3(0, 3, 12),
        ivec3(3, 4, 12),
        ivec3(4, 5, 12),
        ivec3(5, 0, 12),
        ivec3(1, 6, 13),
        ivec3(6, 7, 13),
        ivec3(7, 2, 13),
        ivec3(2, 1, 13),
        ivec3(4, 5, 9),
        ivec3(5, 6, 9),
        ivec3(6, 7, 9),
        ivec3(7, 4, 9)
	};

    // Color of cube # = 1
    vec4 colors1[] = {
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(1.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(1.0, 1.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 1.0, 0.0, 1.0)
    };
	R::Object cube1 = r.createObject();
	r.setVertexAttribs(cube1, 0, 14, vertices);
	r.setVertexAttribs(cube1, 1, 14, colors1);
	r.setTriangleIndices(cube1, 24, triangles);

    // Color of cube # = 2
    vec4 colors2[] = {
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(1.0, 0.65, 0.0, 1.0),
        vec4(1.0, 1.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 1.0, 0.0, 1.0)
    };
	R::Object cube2 = r.createObject();
	r.setVertexAttribs(cube2, 0, 14, vertices);
	r.setVertexAttribs(cube2, 1, 14, colors2);
	r.setTriangleIndices(cube2, 24, triangles);

    // Color of cube # = 3
    vec4 colors3[] = {
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(1.0, 0.65, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(1.0, 1.0, 1.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 1.0, 0.0, 1.0)
    };
	R::Object cube3 = r.createObject();
	r.setVertexAttribs(cube3, 0, 14, vertices);
	r.setVertexAttribs(cube3, 1, 14, colors3);
	r.setTriangleIndices(cube3, 24, triangles);

    // Color of cube # = 4
    vec4 colors4[] = {
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(1.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(1.0, 1.0, 1.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 1.0, 0.0, 1.0)
    };
	R::Object cube4 = r.createObject();
	r.setVertexAttribs(cube4, 0, 14, vertices);
	r.setVertexAttribs(cube4, 1, 14, colors4);
	r.setTriangleIndices(cube4, 24, triangles);

    // Color of cube # = 5
    vec4 colors5[] = {
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(1.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(1.0, 1.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 1.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0)
    };
	R::Object cube5 = r.createObject();
	r.setVertexAttribs(cube5, 0, 14, vertices);
	r.setVertexAttribs(cube5, 1, 14, colors5);
	r.setTriangleIndices(cube5, 24, triangles);

    // Color of cube # = 6
    vec4 colors6[] = {
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(1.0, 0.65, 0.0, 1.0),
        vec4(1.0, 1.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 1.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0)
    };
	R::Object cube6 = r.createObject();
	r.setVertexAttribs(cube6, 0, 14, vertices);
	r.setVertexAttribs(cube6, 1, 14, colors6);
	r.setTriangleIndices(cube6, 24, triangles);

    // Color of cube # = 7
    vec4 colors7[] = {
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(1.0, 0.65, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(1.0, 1.0, 1.0, 1.0),
        vec4(0.0, 0.0, 1.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0)
    };
	R::Object cube7 = r.createObject();
	r.setVertexAttribs(cube7, 0, 14, vertices);
	r.setVertexAttribs(cube7, 1, 14, colors7);
	r.setTriangleIndices(cube7, 24, triangles);

    // Color of cube # = 8
    vec4 colors8[] = {
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(1.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(1.0, 1.0, 1.0, 1.0),
        vec4(0.0, 0.0, 1.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0)
    };
	R::Object cube8 = r.createObject();
	r.setVertexAttribs(cube8, 0, 14, vertices);
	r.setVertexAttribs(cube8, 1, 14, colors8);
	r.setTriangleIndices(cube8, 24, triangles);

    r.enableDepthTest();
	mat4 view = translate(mat4(1.0f), vec3(0, -5.0f, -15.0f)) * rotate(mat4(1.0f), radians(40.0f), vec3(1, 0, 0)); 
    mat4 projection = perspective(radians(70.0f), (float)width/(float)height, 0.1f, 50.0f);
    mat4 vp = projection * view;
    mat4 rot = rotate(mat4(1.0f), pi<float>() / 2.0f - (float) atan(1 / sqrt(2.0)), vec3(1.0, 0.0, 0.0)) * rotate(mat4(1.0f), radians(-45.0f), vec3(0.0, 1.0, 0.0));
    float speed = 45.0f; // degrees per second
    float time = 0;
    float time_2 = 0;
    bool first = false, second = false, third = false, fourth = false;
    std::vector<mat4> model(8, mat4(1.0f));
    std::vector<int> indices(8);
    for (int i = 0; i < 8; i ++) indices[i] = i;
    bool off = false;
    int count = 0;
    while (!r.shouldQuit()) {
        r.clear(vec4(1.0, 1.0, 1.0, 1.0));
        r.useShaderProgram(program);
        // Draw ground
        mat4 sc = scale(mat4(1.0f), vec3(16, 5, 16));
        mat4 ro = rotate(mat4(1.0f), radians(-30.0f), vec3(0, 1, 0));
        mat4 t = translate(mat4(1.0f), vec3(0.0, -5.0, -20.0));
        r.setUniform(program, "transform", vp * t * ro * sc);
		r.drawObject(ground);
        // Draw train tracks
        for (int i = 0; i < 48; i ++) {
            mat4 sc_t = scale(mat4(1.0f), vec3(0.7, 1, 0.3));
            mat4 ro_t = rotate(mat4(1.0f), radians(-7.5f * i), vec3(0, 1, 0));
            mat4 t_t = translate(mat4(1.0f), vec3(-13 * cos(radians(7.5f * i)), 0, -13 * sin(radians(7.5f * i))));
            r.setUniform(program, "transform", vp * t * ro * t_t * ro_t * sc_t);
            r.drawObject(track);
        }
        // Draw sky
        mat4 sc_sk = scale(mat4(1.0f), vec3(16, 9, 1));
        mat4 t_sk = translate(mat4(1.0f), vec3(0, 0, -16));
        r.setUniform(program, "transform", vp * t * ro * t_sk * sc_sk);
        r.drawObject(sky);
        mat4 ro_sk = rotate(mat4(1.0f), radians(90.0f), vec3(0, 1, 0));
        r.setUniform(program, "transform", vp * t * ro * ro_sk * t_sk * sc_sk);
        r.drawObject(sky);
        // Draw cube shadow
        mat4 sc_f = scale(mat4(1.0f), vec3(5, 1, 5));
        mat4 ro_f = rotate(mat4(1.0f), radians(45.0f), vec3(0, 1, 0));
        r.setUniform(program, "transform", vp * t * ro * ro_f * sc_f);
        r.drawObject(square);
        // Draw trees
        mat4 sc_tr = scale(mat4(1.0f), vec3(0.5, 5, 0.5));
        for (int i = 0; i < 20; i ++) {
            mat4 ro_tr = rotate(mat4(1.0f), radians(-18.0f * i - 9.0f), vec3(0, 1, 0));
            mat4 t_tr = translate(mat4(1.0f), vec3(-11 * cos(radians(18.0f * i + 9.0f)), 0, -11 * sin(radians(18.0f * i + 9.0f))));
            r.setUniform(program, "transform", vp * t * t_tr * ro_tr * sc_tr);
            r.drawObject(tree);
        }
        for (int i = 0; i < 20; i ++) {
            mat4 ro_tr = rotate(mat4(1.0f), radians(-18.0f * i), vec3(0, 1, 0));
            mat4 t_tr = translate(mat4(1.0f), vec3(-15 * cos(radians(18.0f * i)), 0, -15 * sin(radians(18.0f * i))));
            r.setUniform(program, "transform", vp * t * t_tr * ro_tr * sc_tr);
            r.drawObject(tree);
        }
        float temp = time;
        float diff = SDL_GetTicks64()*1e-3 - time;
        float diff_2 = SDL_GetTicks64()*1e-3 - time_2;
        time += diff;
        time_2 += diff_2;
        float angle = -20.0f * time;
        // Draw train
        r.useShaderProgram(program_color);
        mat4 sc_train = scale(mat4(1.0f), vec3(0.7, 1, 0.5));
        mat4 ro_train = rotate(mat4(1.0f), radians(-angle), vec3(0, 1, 0));
        mat4 t_train = translate(mat4(1.0f), vec3(-13 * cos(radians(angle)), 0, -13 * sin(radians(angle))));
        r.setUniform(program_color, "transform", vp * t * ro * t_train * ro_train * sc_train);
        r.setUniform<vec4>(program_color, "color", vec4(1.0, 0.0, 0.0, 1.0));
        r.drawObject(train);
        ro_train = rotate(mat4(1.0f), radians(-angle - 5.0f), vec3(0, 1, 0));
        t_train = translate(mat4(1.0f), vec3(-13 * cos(radians(angle + 5.0f)), 0, -13 * sin(radians(angle + 5.0f))));
        r.setUniform(program_color, "transform", vp * t * ro * t_train * ro_train * sc_train);
        r.setUniform<vec4>(program_color, "color", vec4(1.0, 0.65, 0.0, 1.0));
        r.drawObject(train);
        ro_train = rotate(mat4(1.0f), radians(-angle - 10.0f), vec3(0, 1, 0));
        t_train = translate(mat4(1.0f), vec3(-13 * cos(radians(angle + 10.0f)), 0, -13 * sin(radians(angle + 10.0f))));
        r.setUniform(program_color, "transform", vp * t * ro * t_train * ro_train * sc_train);
        r.setUniform<vec4>(program_color, "color", vec4(1.0, 1.0, 0.0, 1.0));
        r.drawObject(train);
        ro_train = rotate(mat4(1.0f), radians(-angle - 15.0f), vec3(0, 1, 0));
        t_train = translate(mat4(1.0f), vec3(-13 * cos(radians(angle + 15.0f)), 0, -13 * sin(radians(angle + 15.0f))));
        r.setUniform(program_color, "transform", vp * t * ro * t_train * ro_train * sc_train);
        r.setUniform<vec4>(program_color, "color", vec4(1.0, 1.0, 1.0, 1.0));
        r.drawObject(train);
        ro_train = rotate(mat4(1.0f), radians(-angle - 20.0f), vec3(0, 1, 0));
        t_train = translate(mat4(1.0f), vec3(-13 * cos(radians(angle + 20.0f)), 0, -13 * sin(radians(angle + 20.0f))));
        r.setUniform(program_color, "transform", vp * t * ro * t_train * ro_train * sc_train);
        r.setUniform<vec4>(program_color, "color", vec4(0.0, 0.0, 1.0, 1.0));
        r.drawObject(train);
        ro_train = rotate(mat4(1.0f), radians(-angle - 25.0f), vec3(0, 1, 0));
        t_train = translate(mat4(1.0f), vec3(-13 * cos(radians(angle + 25.0f)), 0, -13 * sin(radians(angle + 25.0f))));
        r.setUniform(program_color, "transform", vp * t * ro * t_train * ro_train * sc_train);
        r.setUniform<vec4>(program_color, "color", vec4(0.0, 0.5, 0.0, 1.0));
        r.drawObject(train);

        r.useShaderProgram(program);
        // Get transformation matrices for cubes
        rot = rotate(mat4(1.0f), radians(60 * diff_2), vec3(0.0, 1.0, 0.0)) * rot;        
        if (((int) (time / 2) % 4) == 0) {
            if (fourth) {
                diff = (int) (time / 2) * 2.0f - temp;
                time = (int) (time / 2) * 2.0f;
                model[indices[0]] = rotate(mat4(1.0f), radians(speed * diff), vec3(0.0f,1.0f,0.0f)) * model[indices[0]];
                model[indices[1]] = rotate(mat4(1.0f), radians(speed * diff), vec3(0.0f,1.0f,0.0f)) * model[indices[1]];
                model[indices[4]] = rotate(mat4(1.0f), radians(speed * diff), vec3(0.0f,1.0f,0.0f)) * model[indices[4]];
                model[indices[5]] = rotate(mat4(1.0f), radians(speed * diff), vec3(0.0f,1.0f,0.0f)) * model[indices[5]];
                int temp_idx = indices[1];
                indices[1] = indices[0];
                indices[0] = indices[4];
                indices[4] = indices[5];
                indices[5] = temp_idx;
                fourth = false;
                count ++;
            } else {
                if (! off) {
                    if (count == 6) {
                        off = true;
                    } else {
                        first = true;
                        model[indices[0]] = rotate(mat4(1.0f), radians(-speed * diff), vec3(1.0f,0.0f,0.0f)) * model[indices[0]];
                        model[indices[1]] = rotate(mat4(1.0f), radians(-speed * diff), vec3(1.0f,0.0f,0.0f)) * model[indices[1]];
                        model[indices[2]] = rotate(mat4(1.0f), radians(-speed * diff), vec3(1.0f,0.0f,0.0f)) * model[indices[2]];
                        model[indices[3]] = rotate(mat4(1.0f), radians(-speed * diff), vec3(1.0f,0.0f,0.0f)) * model[indices[3]];
                    }
                }
            }
        } else if (((int) (time / 2) % 4) == 1) {
            if (first) {
                diff = (int) (time / 2) * 2.0f - temp;
                time = (int) (time / 2) * 2.0f;
                model[indices[0]] = rotate(mat4(1.0f), radians(-speed * diff), vec3(1.0f,0.0f,0.0f)) * model[indices[0]];
                model[indices[1]] = rotate(mat4(1.0f), radians(-speed * diff), vec3(1.0f,0.0f,0.0f)) * model[indices[1]];
                model[indices[2]] = rotate(mat4(1.0f), radians(-speed * diff), vec3(1.0f,0.0f,0.0f)) * model[indices[2]];
                model[indices[3]] = rotate(mat4(1.0f), radians(-speed * diff), vec3(1.0f,0.0f,0.0f)) * model[indices[3]];
                int temp_idx = indices[1];
                indices[1] = indices[0];
                indices[0] = indices[3];
                indices[3] = indices[2];
                indices[2] = temp_idx;
                first = false;
            } else {
                if (! off) {
                    second = true;
                    model[indices[0]] = rotate(mat4(1.0f), radians(-speed * diff), vec3(0.0f,1.0f,0.0f)) * model[indices[0]];
                    model[indices[1]] = rotate(mat4(1.0f), radians(-speed * diff), vec3(0.0f,1.0f,0.0f)) * model[indices[1]];
                    model[indices[4]] = rotate(mat4(1.0f), radians(-speed * diff), vec3(0.0f,1.0f,0.0f)) * model[indices[4]];
                    model[indices[5]] = rotate(mat4(1.0f), radians(-speed * diff), vec3(0.0f,1.0f,0.0f)) * model[indices[5]];    
                }            
            }
        } else if (((int) (time / 2) % 4) == 2) {
            if (second) {
                diff = (int) (time / 2) * 2.0f - temp;
                time = (int) (time / 2) * 2.0f;
                model[indices[0]] = rotate(mat4(1.0f), radians(-speed * diff), vec3(0.0f,1.0f,0.0f)) * model[indices[0]];
                model[indices[1]] = rotate(mat4(1.0f), radians(-speed * diff), vec3(0.0f,1.0f,0.0f)) * model[indices[1]];
                model[indices[4]] = rotate(mat4(1.0f), radians(-speed * diff), vec3(0.0f,1.0f,0.0f)) * model[indices[4]];
                model[indices[5]] = rotate(mat4(1.0f), radians(-speed * diff), vec3(0.0f,1.0f,0.0f)) * model[indices[5]];
                int temp_idx = indices[1];
                indices[1] = indices[5];
                indices[5] = indices[4];
                indices[4] = indices[0];
                indices[0] = temp_idx;
                second = false;
            } else {
                if (! off) {
                    third = true;
                    model[indices[0]] = rotate(mat4(1.0f), radians(speed * diff), vec3(1.0f,0.0f,0.0f)) * model[indices[0]];
                    model[indices[1]] = rotate(mat4(1.0f), radians(speed * diff), vec3(1.0f,0.0f,0.0f)) * model[indices[1]];
                    model[indices[2]] = rotate(mat4(1.0f), radians(speed * diff), vec3(1.0f,0.0f,0.0f)) * model[indices[2]];
                    model[indices[3]] = rotate(mat4(1.0f), radians(speed * diff), vec3(1.0f,0.0f,0.0f)) * model[indices[3]];
                }
            }
        } else {
            if (third) {
                diff = (int) (time / 2) * 2.0f - temp;
                time = (int) (time / 2) * 2.0f;
                model[indices[0]] = rotate(mat4(1.0f), radians(speed * diff), vec3(1.0f,0.0f,0.0f)) * model[indices[0]];
                model[indices[1]] = rotate(mat4(1.0f), radians(speed * diff), vec3(1.0f,0.0f,0.0f)) * model[indices[1]];
                model[indices[2]] = rotate(mat4(1.0f), radians(speed * diff), vec3(1.0f,0.0f,0.0f)) * model[indices[2]];
                model[indices[3]] = rotate(mat4(1.0f), radians(speed * diff), vec3(1.0f,0.0f,0.0f)) * model[indices[3]];
                int temp_idx = indices[1];
                indices[1] = indices[2];
                indices[2] = indices[3];
                indices[3] = indices[0];
                indices[0] = temp_idx;
                third = false;
            } else {
                if (! off) {
                    fourth = true;
                    model[indices[0]] = rotate(mat4(1.0f), radians(speed * diff), vec3(0.0f,1.0f,0.0f)) * model[indices[0]];
                    model[indices[1]] = rotate(mat4(1.0f), radians(speed * diff), vec3(0.0f,1.0f,0.0f)) * model[indices[1]];
                    model[indices[4]] = rotate(mat4(1.0f), radians(speed * diff), vec3(0.0f,1.0f,0.0f)) * model[indices[4]];
                    model[indices[5]] = rotate(mat4(1.0f), radians(speed * diff), vec3(0.0f,1.0f,0.0f)) * model[indices[5]];
                }
            }
        }
        // Draw cubes
        mat4 sc_c = scale(mat4(1.0f), vec3(1.5, 1.5, 1.5));
        mat4 t_c = translate(mat4(1.0f), vec3(0, 5, 0));
        t_c = t_c * sc_c;
        mat4 ct1 = model[0] * translate(mat4(1.0f), vec3(0.8, 0.8, 0.8));
        r.setUniform(program, "transform", vp * t * ro * t_c * rot * ct1);
		r.drawObject(cube1);
        mat4 ct2 = model[1] * translate(mat4(1.0f), vec3(0.8, 0.8, -0.8));
        r.setUniform(program, "transform", vp * t * ro * t_c * rot * ct2);
		r.drawObject(cube2);
        mat4 ct3 = model[2] * translate(mat4(1.0f), vec3(0.8, -0.8, -0.8));
        r.setUniform(program, "transform", vp * t * ro * t_c * rot * ct3);
		r.drawObject(cube3);
        mat4 ct4 = model[3] * translate(mat4(1.0f), vec3(0.8, -0.8, 0.8));
        r.setUniform(program, "transform", vp * t * ro * t_c * rot * ct4);
		r.drawObject(cube4);
        mat4 ct5 = model[4] * translate(mat4(1.0f), vec3(-0.8, 0.8, 0.8));
        r.setUniform(program, "transform", vp * t * ro * t_c * rot * ct5);
		r.drawObject(cube5);
        mat4 ct6 = model[5] * translate(mat4(1.0f), vec3(-0.8, 0.8, -0.8));
        r.setUniform(program, "transform", vp * t * ro * t_c * rot * ct6);
		r.drawObject(cube6);
        mat4 ct7 = model[6] * translate(mat4(1.0f), vec3(-0.8, -0.8, -0.8));
        r.setUniform(program, "transform", vp * t * ro * t_c * rot * ct7);
		r.drawObject(cube7);
        mat4 ct8 = model[7] * translate(mat4(1.0f), vec3(-0.8, -0.8, 0.8));
        r.setUniform(program, "transform", vp * t * ro * t_c * rot * ct8);
		r.drawObject(cube8);
        r.show();
    }
    r.deleteShaderProgram(program);
    return EXIT_SUCCESS;
}
