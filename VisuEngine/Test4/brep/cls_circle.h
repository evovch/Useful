/**

    @class cls_circle

**/

#ifndef CLS_CIRCLE_H
#define CLS_CIRCLE_H

#include <glm/glm.hpp>

class cls_model;

class cls_circle
{
public:
	cls_circle();
	~cls_circle();

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
