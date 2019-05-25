#ifndef PENGUINS_PHYSICS_H
#define PENGUINS_PHYSICS_H

#include "GameWindow.h"
#include <cglm.h>
#include "Utilities.h"

void physicsMouseRaycast(int mouseX, int mouseY, vec3 outHitPoint, Sprite **outHitSprite);
#endif //PENGUINS_PHYSICS_H
