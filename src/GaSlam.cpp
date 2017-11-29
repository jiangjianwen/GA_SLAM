#include "GaSlam.hpp"

#include <pcl/common/transforms.h>
#include <pcl/filters/voxel_grid.h>

namespace ga_slam {

GaSlam::GaSlam(const Map& globalMap)
        : globalMap_(globalMap) {
    filteredCloud_.reset(new PointCloud);
}

bool GaSlam::setParameters(
        double mapSizeX, double mapSizeY,
        double robotPositionX, double robotPositionY,
        double mapResolution, double voxelSize,
        double minElevation, double maxElevation) {
    mapSizeX_ = mapSizeX;
    mapSizeY_ = mapSizeY;
    robotPositionX_ = robotPositionX;
    robotPositionY_ = robotPositionY;
    mapResolution_ = mapResolution;
    voxelSize_ = voxelSize;
    minElevation_ = minElevation;
    maxElevation_ = maxElevation;

    return true;
}

void GaSlam::registerData(
        const Pose& pose,
        const Pose& tf,
        const PointCloud::ConstPtr& pointCloud) {
    downsamplePointCloud(pointCloud);
    transformPointCloudToMap(pose, tf);
    cropPointCloudToMap();
}

void GaSlam::fuseMap(void) {}

void GaSlam::correctPose(void) {}

void GaSlam::translateMap(const Pose& deltaPose) {}

void GaSlam::updateMap(const PointCloud::ConstPtr& pointCloud) {}

void GaSlam::downsamplePointCloud(const PointCloud::ConstPtr& inputCloud) {
    pcl::VoxelGrid<pcl::PointXYZ> voxelGrid;

    voxelGrid.setInputCloud(inputCloud);
    voxelGrid.setLeafSize(voxelSize_, voxelSize_, voxelSize_);
    voxelGrid.filter(*filteredCloud_);
}

void GaSlam::transformPointCloudToMap(const Pose& pose, const Pose& tf) {
    Pose poseRotation, tfWithPose;
    double roll, pitch, yaw;

    pcl::getEulerAngles(pose, roll, pitch, yaw);
    pcl::getTransformation(0., 0., 0., -roll, -pitch, 0., poseRotation);
    tfWithPose = poseRotation * tf;

    pcl::transformPointCloud(*filteredCloud_, *filteredCloud_, tfWithPose);
}

void GaSlam::cropPointCloudToMap(void) {}

}  // namespace ga_slam

