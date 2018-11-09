/*
 * BaseFrame.h
 *
 *  Created on: Oct 31, 2018
 *      Author: sujiwo
 */

#ifndef _BASEFRAME_H_
#define _BASEFRAME_H_


#include <Eigen/Eigen>
#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>

#include "CameraPinholeParams.h"
#include "utilities.h"


class MapPoint;


class BaseFrame
{
public:
	BaseFrame();
	virtual ~BaseFrame();

	const Pose& pose() const
	{ return mPose; }

	void setPose (const Eigen::Vector3d &p, const Eigen::Quaterniond &q)
	{ mPose = Pose::from_Pos_Quat(p, q); }

	void setPose (const Pose &p)
	{ mPose = p; }

	Eigen::Vector3d position() const
	{ return mPose.position(); }

	Eigen::Quaterniond orientation() const
	{ return mPose.orientation(); }

	// Project to 2D
	Eigen::Vector2d project (const Eigen::Vector3d &pt3) const;

	Eigen::Vector2d project (const MapPoint &pt3) const;

	// Transform a point in World coordinate to Frame-centric one
	Eigen::Vector3d transform (const Eigen::Vector3d &pt3) const;

	void setCameraParam(const CameraPinholeParams *c)
	{ cameraParam = const_cast<CameraPinholeParams*>(c); }

	CameraPinholeParams getCameraParameters() const
	{ return *cameraParam; }

	/*
	 * This matrix transforms points in World Coordinate to Frame-centric coordinate
	 */
	Eigen::Matrix4d externalParamMatrix4 () const;

	/*
	 * Similar to the above function, only for general pose
	 */
	static Eigen::Matrix4d createExternalParamMatrix4(const Pose &ps);

	Eigen::Matrix<double,3,4> projectionMatrix () const;

	/*
	 * Normal vector; Or, Z-Axis of this frame
	 */
	Eigen::Vector3d normal() const;

	void computeFeatures (cv::Ptr<cv::FeatureDetector> fd, const cv::Mat &mask=cv::Mat());

	const cv::Mat descriptor(kpid r) const
	{ return fDescriptors.row(r).clone(); }

	const cv::KeyPoint keypoint(kpid k) const
	{ return fKeypoints.at(k); }

	cv::Mat allDescriptors() const
	{ return fDescriptors; }

	int numOfKeypoints() const
	{ return fKeypoints.size(); }

	int numOfKeyPoints() const
	{ return fKeypoints.size(); }

	cv::Mat getImage() const
	{ return image; }

	enum PerturbationMode {
		Lateral,
		Longitudinal,
		Vertical
	};

	void perturb (
		PerturbationMode h,
		bool useRandomMotion,
		double displacement=0, double rotationAngle=0);


protected:
	cv::Mat image;

	/*
	 * A word on pose: this variable stores frame's pose in world coordinate system,
	 * with X->left, Y->bottom, Z->front
	 */
	Pose mPose = Pose::Identity();

	CameraPinholeParams *cameraParam = nullptr;

	/*
	 * =================
	 * 2D Image Features
	 */

	cv::Mat fDescriptors;

	std::vector<cv::KeyPoint> fKeypoints;
};

#endif /* _BASEFRAME_H_ */