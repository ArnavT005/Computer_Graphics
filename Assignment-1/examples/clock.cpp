#include "../src/a1.hpp"
#include "romans.hpp"
#include <iostream>
using namespace std;

// namespace R = COL781::Software;
namespace R = COL781::Hardware;
using namespace glm;

int main() {
	R::Rasterizer r;
    if (!r.initialize("Clock", 640, 480))
        return EXIT_FAILURE;
    R::ShaderProgram program = r.createShaderProgram(
        r.vsColorTransform(),
        r.fsIdentity()
    );  
    vec4 unitSquare[] = {
            vec4(-0.5, -0.5, 0.0, 1.0),
            vec4(0.5, -0.5, 0.0, 1.0),
            vec4(0.5, 0.5, 0.0, 1.0),
            vec4(-0.5, 0.5, 0.0, 1.0)
    };


    vec4 forkColors[] = {
		vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.0, 0.0, 0.0, 1.0)
    };

    vec4 secondHandColors[] = {
		vec4(1.0, 0.0, 0.0, 1.0),
        vec4(1.0, 0.0, 0.0, 1.0),
        vec4(1.0, 0.0, 0.0, 1.0),
        vec4(1.0, 0.0, 0.0, 1.0)
    };

    vec4 MinuteColors[] = {
		vec4(0.0, 1.0, 0.0, 1.0),
        vec4(0.0, 1.0, 0.0, 1.0),
        vec4(0.0, 1.0, 0.0, 1.0),
        vec4(0.0, 1.0, 0.0, 1.0)
    };

    vec4 hourHandColors[] = {
		vec4(0.0, 0.0, 1.0, 1.0),
        vec4(0.0, 0.0, 1.0, 1.0),
        vec4(0.0, 0.0, 1.0, 1.0),
        vec4(0.0, 0.0, 1.0, 1.0)
    };


	ivec3 forkTriangles[] = {
		ivec3(0, 1, 2),
		ivec3(0, 2, 3)
	};
	R::Object fork = r.createObject();
	r.setVertexAttribs(fork, 0, 4, unitSquare);
	r.setVertexAttribs(fork, 1, 4, forkColors);
	r.setTriangleIndices(fork, 2, forkTriangles);


    R::Object secondHand = r.createObject();
    r.setVertexAttribs(secondHand, 0, 4, unitSquare);
    r.setVertexAttribs(secondHand, 1, 4, secondHandColors);
    r.setTriangleIndices(secondHand, 2, forkTriangles);

    R::Object minuteHand = r.createObject();
    r.setVertexAttribs(minuteHand, 0, 4, unitSquare);
    r.setVertexAttribs(minuteHand, 1, 4, MinuteColors);
    r.setTriangleIndices(minuteHand, 2, forkTriangles);

    R::Object hourHand = r.createObject();
    r.setVertexAttribs(hourHand, 0, 4, unitSquare);
    r.setVertexAttribs(hourHand, 1, 4, hourHandColors);
    r.setTriangleIndices(hourHand, 2, forkTriangles);





    mat4 iScale = scale(mat4(1.0f), vec3(0.01f, 0.04f, 1.0f));
    mat4 iScaleLarge = scale(mat4(1.0f), vec3(0.0125f, 0.125f, 1.0f));
    mat4 iScaleNumber = scale(mat4(1.0f), vec3(0.01f, 0.15f, 1.0f));
    float markingRadiusFork = 0.90f;
    float markingRadiusNumber = 0.7f;


    while (!r.shouldQuit()) {
        Uint32 displayTime = 0;
        auto now = std::chrono::system_clock::now();
        // add 5 hours and 30 minutes to the current time
        now += std::chrono::hours(5);
        now += std::chrono::minutes(30);
        auto hours = std::chrono::duration_cast<std::chrono::hours>(now.time_since_epoch()).count();
        auto minutes = std::chrono::duration_cast<std::chrono::minutes>(now.time_since_epoch()).count();
        auto seconds = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
        hours = hours % 12;
        minutes = minutes % 60;
        seconds = seconds % 60;
        r.clear(vec4(1.0, 1.0, 1.0, 1.0));
        r.useShaderProgram(program);
        for (int i = 0; i < 60; i ++) {
            float markingAngle = radians(i * 6.0f);
            mat4 markingRotate = rotate(mat4(1.0f), markingAngle, vec3(0.0, 0.0, -1.0));
            vec3 markingCenterCoordinate(markingRadiusFork * sin(markingAngle), markingRadiusFork * cos(markingAngle), 0.0f);
            mat4 markingTranslate = translate(mat4(1.0f), markingCenterCoordinate);
            if(i%5==0){
                r.setUniform(program, "transform", markingTranslate * markingRotate * iScaleLarge);
            }
            else{
                r.setUniform(program, "transform", markingTranslate * markingRotate * iScale);
            }
            r.drawObject(fork);
        }
        // I
        MARK_2D_I(30.0f, 1, 1.05,r);
        // II
        MARK_2D_I(60.0f, 2, 1,r);
        MARK_2D_I(60.0f, 3, 1.05,r);


        // III
        MARK_2D_I(90.0f, 4, 1.0,r);
        MARK_2D_I(90.0f, 5, 1.05,r);
        MARK_2D_I(90.0f, 6, 1.10,r);

        // IV
        MARK_2D_I(120.0f, 7, 1.0,r);
        MARK_2D_V(120.f, 8, 1, 1.1,r);
        MARK_2D_V(120.f, 9, -1, 1.1,r);
        // V
        MARK_2D_V(150.f, 10, 1, 1.05,r);
        MARK_2D_V(150.f, 11, -1, 1.05,r);

        // VI
        MARK_2D_V(180.f, 12, 1, 0.90,r);
        MARK_2D_V(180.f, 13, -1, 0.90,r);
        float markingAngle14 = radians(180.0f);
        vec3 markingCenterCoordinate14(0.05f, markingRadiusNumber * cos(markingAngle14), 0.0f);
        mat4 markingTranslate14 = translate(mat4(1.0f), markingCenterCoordinate14);
        r.setUniform(program, "transform", markingTranslate14 * iScaleNumber);
        r.drawObject(fork);

        // VII
        MARK_2D_V(210.f, 15, 1, 1.05,r);
        MARK_2D_V(210.f, 16, -1, 1.05,r);
        MARK_2D_I(210.0f,17, 0.85,r);
        MARK_2D_I(210.0f,18, 0.80,r);


        // VIII
        MARK_2D_V(240.f, 19, 1, 1.05,r);
        MARK_2D_V(240.f, 20, -1, 1.05,r);
        MARK_2D_I(240.0f,21, 0.95,r);
        MARK_2D_I(240.0f,22, 0.90,r);
        MARK_2D_I(240.0f,23, 0.85,r);

        // IX
        MARK_2D_I(270.0f,24, 1.1,r);
        MARK_2D_X(270.f, 25, 1, 1.05,r);
        MARK_2D_X(270.f, 26, -1, 1.05,r);

        // X
        MARK_2D_X(300.f, 27, 1, 1.05,r);
        MARK_2D_X(300.f, 28, -1, 1.05,r);


        // XI
        MARK_2D_X(330.f, 29, 1, 1.0,r);
        MARK_2D_X(330.f, 30, -1, 1.0,r);
        MARK_2D_I(330.0f,31, 0.90,r);


        // XII
        MARK_2D_X(360.f, 32, 1, 1.0,r);
        MARK_2D_X(360.f, 33, -1, 1.0,r);

        float markingAngle34 = radians(360.0f);
        vec3 markingCenterCoordinate34((markingRadiusNumber * sin(markingAngle34)*0.95) + 0.05 , markingRadiusNumber * cos(markingAngle34), 0.0f);
        mat4 markingTranslate34 = translate(mat4(1.0f), markingCenterCoordinate34);
        r.setUniform(program, "transform", markingTranslate34 * iScaleNumber);
        r.drawObject(fork);

        vec3 markingCenterCoordinate35((markingRadiusNumber * sin(markingAngle34)*0.95) + 0.08 , markingRadiusNumber * cos(markingAngle34), 0.0f);
        mat4 markingTranslate35 = translate(mat4(1.0f), markingCenterCoordinate35);
        r.setUniform(program, "transform", markingTranslate35 * iScaleNumber);
        r.drawObject(fork);


        Uint32 secondPassed = seconds;
        float secondHandRadius = 0.40f;
        mat4 secondHandScale = scale(mat4(1.0f), vec3(0.025f, 0.8f, 1.0f));
        float secondHandAngle = radians(secondPassed * 6.0f);
        mat4 secondHandRotate = rotate(mat4(1.0f), secondHandAngle, vec3(0.0, 0.0, -1.0f));
        vec3 secondHandCenterCoordinate(secondHandRadius * sin(secondHandAngle), secondHandRadius * cos(secondHandAngle), 0.0f);
        mat4 secondHandTranslate = translate(mat4(1.0f), secondHandCenterCoordinate);
        r.setUniform(program, "transform", secondHandTranslate * secondHandRotate * secondHandScale);
        r.drawObject(secondHand);

        float minutePassed = minutes*1.0f + (float)seconds/60.0f;
        float minuteHandRadius = 0.30f;
        mat4 minuteHandScale = scale(mat4(1.0f), vec3(0.05f, 0.6f, 1.0f));
        float minuteHandAngle = radians(minutePassed * 6.0f);
        mat4 minuteHandRotate = rotate(mat4(1.0f), minuteHandAngle, vec3(0.0, 0.0, -1.0f));
        vec3 minuteHandCenterCoordinate(minuteHandRadius * sin(minuteHandAngle), minuteHandRadius * cos(minuteHandAngle), 0.0f);
        mat4 minuteHandTranslate = translate(mat4(1.0f), minuteHandCenterCoordinate);
        r.setUniform(program, "transform", minuteHandTranslate * minuteHandRotate * minuteHandScale);
        r.drawObject(minuteHand);

        float hourPassed = hours*1.0f + (float)minutes/60.0f;
        float hourHandRadius = 0.15f;
        mat4 hourHandScale = scale(mat4(1.0f), vec3(0.075f, 0.3f, 1.0f));
        float hourHandAngle = radians(hourPassed * 30.0f);
        mat4 hourHandRotate = rotate(mat4(1.0f), hourHandAngle, vec3(0.0, 0.0, -1.0f));
        vec3 hourHandCenterCoordinate(hourHandRadius * sin(hourHandAngle), hourHandRadius * cos(hourHandAngle), 0.0f);
        mat4 hourHandTranslate = translate(mat4(1.0f), hourHandCenterCoordinate);
        r.setUniform(program, "transform", hourHandTranslate * hourHandRotate * hourHandScale);
        r.drawObject(hourHand);



        r.show();
    }



    
    r.deleteShaderProgram(program);
    return EXIT_SUCCESS;
}
