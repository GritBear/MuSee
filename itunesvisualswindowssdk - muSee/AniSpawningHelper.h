#pragma once
#include "AniStructs.h"
#include "StatisticalFunctions.h"

//Multiple method of spawning animations coordinates are enclosed
//Diagonal Rectangular Spawn
//traditional animation spawning logic
void RectDia2Point2DSpawn(float x1, float y1,float x2,float y2, float xVel, float yVel, bool flip, deque<Vector3> &position, deque<Vector3> &speed, float width = 100, float height = 100);

//actual radius is return for the caller method to judge spawning condition
bool RectDiaRadConstraintSpawning(float x1, float y1,float x2,float y2, float xVel, float yVel, float previousAngleInRad, bool flip, AStruct &theStruct);

//Tree Branch Spawn
void TreeBranchSpawn(Vector3 root, AStruct &theStruct);