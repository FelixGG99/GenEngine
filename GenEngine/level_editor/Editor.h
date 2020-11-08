#pragma once
#ifndef EDITOR_H
#define EDITOR_H

#include "util/vec.h"
#include "Shader.h"

void setGradientColor(const vec3& top, const vec3& bot);
void drawAxis(const Shader& shader);
void drawFloorPlane(const Shader& shader, const mat4x4 view, const mat4x4 projection);

void xMeasures(const Shader& shader);
#endif // !EDITOR_H

