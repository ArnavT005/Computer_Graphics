#ifndef ROMAN_NUMBERS
#define ROMAN_NUMBERS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../src/a1.hpp"

namespace R = COL781::Software;


extern float markingRadiusNumber;
extern glm::mat4 iScaleNumber;
extern unsigned int indices[6];
extern glm::vec2 unitSquare[4];
extern class R::Rasterizer r;

void MARK_2D_I(float angle, int i, float scale, R::Rasterizer &r);
void MARK_2D_V(float angle, int i, int j, float scale, R::Rasterizer &r);
void MARK_2D_X(float angle, int i, int j, float scale, R::Rasterizer &r);

#endif
#define MARK_2D_I(angle,i, scale, r) \
    float markingAngle##i = glm::radians(angle); \
    glm::vec3 markingCenterCoordinate##i (markingRadiusNumber * glm::sin(markingAngle##i) * scale, \
                                                            markingRadiusNumber * glm::cos(markingAngle##i), \
                                                            0.0f); \
    glm::mat4 markingTranslate##i = glm::translate(glm::mat4(1.0f), markingCenterCoordinate##i); \
    r.setUniform(program, "transform", markingTranslate##i * iScaleNumber);\
    r.drawObject(fork);








    #define MARK_2D_V(angle,i,j,scale,r) \
    float markingAngle##i = glm::radians(angle); \
    glm::vec3 markingCenterCoordinate##i ((markingRadiusNumber * glm::sin(markingAngle##i) + j*0.0175) * scale, \
                                                            markingRadiusNumber * glm::cos(markingAngle##i), \
                                                            0.0f); \
    glm::mat4 markingTranslate##i = glm::translate(glm::mat4(1.0f), markingCenterCoordinate##i); \
    glm::mat4 markingRotate##i = glm::rotate(glm::mat4(1.0f), glm::radians(j*15.f), glm::vec3(0.0, 0.0, -1.0)); \
    r.setUniform(program, "transform", markingTranslate##i * markingRotate##i* iScaleNumber);\
    r.drawObject(fork);


    #define MARK_2D_X(angle,i,j,scale,r) \
    float markingAngle##i = glm::radians(angle); \
    glm::vec3 markingCenterCoordinate##i ((markingRadiusNumber * glm::sin(markingAngle##i)) * scale, \
                                                            markingRadiusNumber * glm::cos(markingAngle##i), \
                                                            0.0f); \
    glm::mat4 markingTranslate##i = glm::translate(glm::mat4(1.0f), markingCenterCoordinate##i); \
    glm::mat4 markingRotate##i = glm::rotate(glm::mat4(1.0f), glm::radians(j*15.f), glm::vec3(0.0, 0.0, -1.0)); \
    r.setUniform(program, "transform", markingTranslate##i * markingRotate##i* iScaleNumber);\
    r.drawObject(fork);