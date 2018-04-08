/**

	@class cls_camera

*/

#ifndef CLS_CAMERA_H
#define CLS_CAMERA_H

// STD
#include <vector>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

// OpenGL
#include <GL/glew.h>

class cls_renderer;

class cls_camera
{
public:
	cls_camera(glm::vec3 p_center, float p_radius);
	~cls_camera();

	//// Camera-to-clip-----------------------------------------------------

public: // Accessors

	/**
	 * @brief Accessor
	 */
	float GetFrAngle(void) const { return mFrustumAngle; }

	/**
	 * @brief Accessor
	 */
	float GetFrNearWidth(void) const { return mFrustumZNearWidth; }

	/**
	 * @brief Accessor
	 */
	float GetParallelBoxSize(void) const { return mParallelBoxSize; }

	/**
	 * @brief Accessor
	 */
	float GetParallelBoxDepth(void) const { return mParallelBoxDepth; }

public: // Modifiers

	/**
	 * @brief Modifier. Switch between parallel and perspective
	 */
	void SwitchProjType(void) { mIsPerspective = !mIsPerspective; }

	/**
	 * @brief Modifier
	 */
	void SetFrAngle(float p_newAngle) { mFrustumAngle = p_newAngle; }

	/**
	 * @brief Modifier
	 */
	void SetFrNearWidth(float p_newNearWidth) { mFrustumZNearWidth = p_newNearWidth; }

	/**
	 * @brief Modifier
	 */
	void SetParallelBoxSize(float p_newParallelBoxSize) { mParallelBoxSize = p_newParallelBoxSize; }

	/**
	 * @brief Modifier
	 */
	void SetParallelBoxDepth(float p_newParallelBoxDepth) { mParallelBoxDepth = p_newParallelBoxDepth; }

	//// Model-to-camera-----------------------------------------------------

public: // Accessors

	/**
	 * @brief Accessor
	 */
	float GetDist(void) const { return mDist; }

	/**
	 * @brief Accessor
	 */
	glm::vec3 GetLookPt(void) const { return mLookPt; }

public: // Modifiers

	/**
	 * @brief Modifier
	 */
	void SetWinSize(int p_width, int p_height) { mWinWidth = p_width; mWinHeight = p_height; }

	/**
	 * @brief Modifier
	 */
	void SetDist(float p_newDist) { mDist = p_newDist; }

	/**
	 * @brief Modifier
	 */
	void SetLookPt(glm::vec3 p_newLookPt) { mLookPt = p_newLookPt; }

	//// Common-----------------------------------------------------

	/**
	 * @brief Manipulation method - reset
	 */
	void Reset(void);

	/**
	 * @brief Manipulation method - pan
	 */
	void Pan(float /*p_curScreenX*/,
			 float /*p_curScreenY*/,
			 float /*p_startScreenX*/,
			 float /*p_startScreenY*/,
			 glm::vec3 /*p_startLookPt*/);

	/**
	 * @brief Manipulation method - center
	 */
	void Center(float /*p_curScreenX*/, float /*p_curScreenY*/);

	/**
	 * @brief Manipulation method - zoom
	 */
	void Zoom(float p_curScreenY,
	          float p_prevScreenY,
	          float p_startScreenY,
	          float p_startFrAngle,
	          float p_startPBS);

	/**
	 * @brief Manipulation method - rotate
	 */
	void Rotate(glm::vec3 p_curLocalDir, glm::vec3 p_startLocalDir);

	/**
	 * @brief Accessor. 3x3 rotation matrix
	 */
	glm::mat3 GetRotM(void) const;

	/**
	 * @brief Accessor. Model-to-camera matrix
	 */
	glm::mat4 GetModelToCamera(void) const;

	/**
	 * @brief Accessor. Camera-to-clip matrix
	 */
	glm::mat4 GetCameraToClip(void) const;

	/**
	 * @brief Accessor. Full MVP matrix (which is Camera-to-clip * Model-to-camera)
	 */
	glm::mat4 GetMVP(void) const;

	/**
	 * @brief Accessor. Viewer standing point
	 */
	glm::vec3 GetViewerPoint(void) const;

	/**
	 * @brief Send camera to GPU
	 */
	//void SendCamToGPU(std::vector<GLuint> p_programs, std::vector<GLuint> p_uniforms); //const;
	void SendCamToGPU(cls_renderer* p_renderer);

private: // Data members

	/**
	 *
	 */
	int mWinWidth;

	/**
	 *
	 */
	int mWinHeight;

	/**
	 * @brief Default value - initialized once during construction, constant through time, used for Reset()
	 */
	glm::vec3 mDefCenter;

	/**
	 * @brief Default value - initialized once during construction, constant through time, used for Reset()
	 */
	float mDefRadius;

	//// Camera-to-clip-----------------------------------------------------

	/**
	 * @brief Current projection mode - perspective or parallel
	 */
	bool mIsPerspective;

	/**
	 * @brief Perspective viewing frustum parameter - angle
	 */
	float mFrustumAngle;

	/**
	 * @brief Perspective viewing frustum parameter - Z near width
	 */
	float mFrustumZNearWidth;

	/**
	 * @brief Parallel projections parameter - box size
	 */
	float mParallelBoxSize;

	/**
	 * @brief Parallel projections parameter - box depth
	 */
	float mParallelBoxDepth;

	//// Model-to-camera-----------------------------------------------------

	/**
	 * @brief Look point
	 */
	glm::vec3 mLookPt;

	/**
	 * @brief Distance to the looking point along the looking direction - defines the camera point
	 */
	float mDist;

	/**
	 * @brief Quaternion, defining the looking direction
	 */
	glm::quat mQua;

};

#endif // CLS_CAMERA_H
