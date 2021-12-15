#pragma once
#include <engine/engine.h>

class Interpolation {
	Interpolation() {};
	
public:
	glm::vec3 linear(glm::vec3 startPos, glm::vec3 endPos, float startTime, float endTime, float curTime);
};

/*
* Calculate position between two transitions, start, end by creating a vector, displacement
* and extending it from 0 to 1 (its original length)
* 
* This 0-1 number is calculated by finding the fraction of travelTime and travelledTime
*/
glm::vec3 Interpolation::linear(glm::vec3 startPos, glm::vec3 endPos, float startTime, float endTime, float curTime) 
{
	//If current time is larger than travelled time, return endPos as it is supposed to be done by now
	if (curTime > endTime) { return endPos; }

	//Find out how quick to move each part 
	float travelDur = endTime - startTime;
	float timePosition = curTime - startTime;
	
	float travelledTime = timePosition / travelDur;
	//Find travel vector
	glm::vec3 travelDistance = endPos - startPos;
	//find out how large the displacement is
	glm::vec3 movement = travelledTime * travelDistance;
	//Add displacement to original position
	glm::vec3 position = startPos + movement;

	return position;
}