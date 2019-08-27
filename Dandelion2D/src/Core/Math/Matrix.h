#pragma once

#include <Core\Definitions.h>
#include <External\glm\glm.hpp>
#include <External\glm\gtc\matrix_transform.hpp>

using mat2 = glm::mat2x2;
using mat3 = glm::mat3x3;
using mat4 = glm::mat4x4;

using vec3 = glm::vec3;
using vec4 = glm::vec4;

#define TRANSLATION_MATRIX(mat, vec) glm::translate(mat, vec)
#define TRANSLATION_MATRIX_2D(mat, vec2D) TRANSLATION_MATRIX(mat, vec3(vec2D.x, vec2D.y, 0.0f))

#define ROTATION_MATRIX_X(mat, deg) glm::rotate(mat, DEG_TO_RAD(deg), vec3(1.0f, 0.0f, 0.0f))
#define ROTATION_MATRIX_Y(mat, deg) glm::rotate(mat, DEG_TO_RAD(deg), vec3(0.0f, 1.0f, 0.0f))
#define ROTATION_MATRIX_Z(mat, deg) glm::rotate(mat, DEG_TO_RAD(deg), vec3(0.0f, 0.0f, 1.0f))
#define ROTATION_MATRIX_AXIS(mat, deg, axis) glm::rotate(mat, DEG_TO_RAD(deg), axis)

#define ROTATION_MATRIX_2D(mat, deg) ROTATION_MATRIX_Z(mat, deg)

#define SCALE_MATRIX(mat, scale) glm::scale(mat, scale)
#define SCALE_MATRIX_2D(mat, vec2D) SCALE_MATRIX(mat, vec3(vec2D.x, vec2D.y, 1.0f))

#define ORTHOG_MATRIX(left, right, bottom, top, zNear, zFar) glm::ortho(left, right, bottom, top, zNear, zFar)

#define PERSPECTIVE_MATRIX(fovDeg, winWidth, winHeight, zNear, zFar) glm::perspective(DEG_TO_RAD(fovDeg), float(winWidth) / float(winHeight), zNear, zFar)
