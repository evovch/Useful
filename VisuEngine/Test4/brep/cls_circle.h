/**

    @class cls_circle

**/

#ifndef CLS_CIRCLE_H
#define CLS_CIRCLE_H

// GLM
#include <glm/glm.hpp>

class cls_model;

class cls_circle
{
public:
	cls_circle();
	~cls_circle();

	void SetRadius(double p_R) { mR = p_R; }
	void SetPosition(double p_x, double p_y, double p_z) { mPosition = glm::dvec3(p_x, p_y, p_z); }

	void BuildModel(cls_model* p_model);

private:
	// Cached triangulation
	//TODO

	glm::dvec3 mPosition;
	glm::dvec3 mNormal;
	glm::dvec3 mRefDir;

	glm::dmat4 mMatrix;

	double mR;

};

#endif // CLS_CIRCLE_H
