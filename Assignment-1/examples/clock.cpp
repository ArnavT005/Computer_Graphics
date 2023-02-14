#include "../src/a1.hpp"

// namespace R = COL781::Software;
namespace R = COL781::Hardware;
using namespace glm;

int main() {
	R::Rasterizer r;
    if (!r.initialize("Example 2", 640, 480))
        return EXIT_FAILURE;
    R::ShaderProgram program = r.createShaderProgram(
        r.vsColorTransform(),
        r.fsIdentity()
    ); 




    // Roman numbers
    // vec4 unitSquare[] = {
    //     vec4(-0.5, -0.5, 0.0, 1.0),
    //     vec4(0.5, -0.5, 0.0, 1.0),
    //     vec4(0.5, 0.5, 0.0, 1.0),
    //     vec4(-0.5, 0.5, 0.0, 1.0)
    // };
    // ivec3 numberIndices[] = {
    //     ivec3(0, 1, 2),
    //     ivec3(2, 3, 0)
    // };
    // vec4 numberMarkingColor[] = {
    //     vec4(0.5, 0.5, 0.5, 1.0),
    //     vec4(0.5, 0.5, 0.5, 1.0)
    // };

    // float numberMarkingRadius = 0.70f;
    // mat4 numberMarkingScale = scale(mat4(1.0f), vec3(0.01, 0.15, 0.15));
    // R::Object numberShape = r.createObject();
	// r.setVertexAttribs(numberShape, 0, 4, unitSquare);
	// r.setVertexAttribs(numberShape, 1, 4, numberMarkingColor);
	// r.setTriangleIndices(numberShape, 2, numberIndices);






    // clock forks
    vec4 forks[] = {
        vec4(-0.01, -0.3, 0.0, 1.0),
        vec4(0.01, -0.3, 0.0, 1.0),
        vec4(0.01, 0.3, 0.0, 1.0),
        vec4(-0.01, 0.3, 0.0, 1.0)
    };
    ivec3 forkIndices[] = {
        ivec3(0, 1, 3),
        ivec3(1, 2, 3)
    };
    vec4 forkMarkingColor[] = {
        vec4(1.0, 1.0, 1.0, 1.0),
        vec4(1.0, 1.0, 1.0, 1.0)
    };
    float forkMarkingRadius = 0.90f;
    mat4 iScale = scale(mat4(1.0f), vec3(0.1f, 0.125f, 1.0f));
    mat4 iScaleLarge = scale(mat4(1.0f), vec3(0.25f, 0.25f, 1.0f));
    R::Object forkShape = r.createObject();
	r.setVertexAttribs(forkShape, 0, 4, forks);
	r.setVertexAttribs(forkShape, 1, 4, forkMarkingColor);
    r.setTriangleIndices(forkShape, 2, forkIndices);


        while (!r.shouldQuit()) {
            r.clear(vec4(1.0, 1.0, 1.0, 1.0));
            r.useShaderProgram(program);
            r.setUniform(program, "transform", iScale);
            r.drawObject(forkShape);
            r.show();

        // for (int i = 0; i < 60; i ++) {
        //     float markingAngle = radians(i * 6.0f);
        //     mat4 markingRotate = rotate(mat4(1.0f), markingAngle, vec3(0.0, 0.0, -1.0));
        //     vec3 markingCenterCoordinate(forkMarkingRadius * sin(markingAngle), forkMarkingRadius * cos(markingAngle), 0.0f);
        //     mat4 markingTranslate = translate(mat4(1.0f), markingCenterCoordinate);
        //     if(i%5==0){
        //         r.setUniform(program, "transform", markingTranslate * markingRotate * iScaleLarge);
        //         r.drawObject(forkShape);
        //     }
        //     else{
        //         r.setUniform(program, "transform", markingTranslate * markingRotate * iScale);
        //         r.drawObject(forkShape);
        //     }
            
        // }
    }
    r.deleteShaderProgram(program);
    return EXIT_SUCCESS;
	
}
