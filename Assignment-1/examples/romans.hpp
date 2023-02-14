#ifndef ROMAN_NUMBERS
#define ROMAN_NUMBERS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

extern float markingRadius;
extern glm::mat4 markingScale;
extern glm::vec3 markingColor;
extern unsigned int indices[6];
extern glm::vec2 unitSquare[4];
// extern class SoftwareRasterizer *pSoftwareRasterizer;
extern class R::Rasterizer r;

void MARK_2D_I(float angle, int i, float scale);
void MARK_2D_V(float angle, int i, int j, float scale);
void MARK_2D_X(float angle, int i, int j, float scale);

#endif
#define MARK_2D_I(angle,i, scale) \
    float markingAngle##i = glm::radians(angle); \
    glm::vec3 markingCenterCoordinate##i (markingRadius * glm::sin(markingAngle##i) * scale, \
                                                            markingRadius * glm::cos(markingAngle##i), \
                                                            0.0f); \
    glm::mat4 markingTranslate##i = glm::translate(glm::mat4(1.0f), markingCenterCoordinate##i); \
    
    pSoftwareRasterizer->setCustom2d(markingTranslate##i * markingScale); \
    pSoftwareRasterizer->rasterizeArbitraryShape2D(unitSquare, indices, markingColor, 2);



    #define MARK_2D_V(angle,i,j,scale) \
    float markingAngle##i = glm::radians(angle); \
    glm::vec3 markingCenterCoordinate##i ((markingRadius * glm::sin(markingAngle##i) + j*0.0175) * scale, \
                                                            markingRadius * glm::cos(markingAngle##i), \
                                                            0.0f); \
    glm::mat4 markingTranslate##i = glm::translate(glm::mat4(1.0f), markingCenterCoordinate##i); \
    glm::mat4 markingRotate##i = glm::rotate(glm::mat4(1.0f), glm::radians(j*15.f), glm::vec3(0.0, 0.0, -1.0)); \
    pSoftwareRasterizer->setCustom2d(markingTranslate##i * markingRotate##i* markingScale); \
    pSoftwareRasterizer->rasterizeArbitraryShape2D(unitSquare, indices, markingColor, 2);


    #define MARK_2D_X(angle,i,j,scale) \
    float markingAngle##i = glm::radians(angle); \
    glm::vec3 markingCenterCoordinate##i ((markingRadius * glm::sin(markingAngle##i)) * scale, \
                                                            markingRadius * glm::cos(markingAngle##i), \
                                                            0.0f); \
    glm::mat4 markingTranslate##i = glm::translate(glm::mat4(1.0f), markingCenterCoordinate##i); \
    glm::mat4 markingRotate##i = glm::rotate(glm::mat4(1.0f), glm::radians(j*15.f), glm::vec3(0.0, 0.0, -1.0)); \
    pSoftwareRasterizer->setCustom2d(markingTranslate##i * markingRotate##i* markingScale); \
    pSoftwareRasterizer->rasterizeArbitraryShape2D(unitSquare, indices, markingColor, 2);