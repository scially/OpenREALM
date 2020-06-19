/**
* This file is part of OpenREALM.
*
* Copyright (C) 2018 Alexander Kern <laxnpander at gmail dot com> (Braunschweig University of Technology)
* For more information see <https://github.com/laxnpander/OpenREALM>
*
* OpenREALM is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* OpenREALM is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with OpenREALM. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PROJECT_REALM_IMPORT_H
#define PROJECT_REALM_IMPORT_H

#include <fstream>
#include <unordered_map>

#include <realm_core/camera_settings_factory.h>
#include <realm_io/utilities.h>

namespace realm
{
namespace io
{

/*!
 * @brief Function for loading camera from .yaml file. For formatted examples see realm_ros/config/...
 *        Interface for directory + filename version
 * @param directory Directory of the file
 * @param filename Name of the file including suffix
 * @return Loaded camera model
 */
camera::Pinhole loadCameraFromYaml(const std::string &directory, const std::string &filename);

/*!
 * @brief Function for loading camera from .yaml file. For formatted examples see realm_ros/config/...
 *        Interface for filepath one argument
 * @param filepath Absolute path to the file
 * @param fps Optional parameter if you also want to grab the fps in the loader function
 * @return Loaded camera model
 */
camera::Pinhole loadCameraFromYaml(const std::string &filepath, double* fps = nullptr);

/*!
 * @brief Function for loading a georeference from .yaml file. Format is OpenCV YAML standard for matrices. The matrix
 *        should have dimensions 4x4 with (R | t) and the last row (0 0 0 1).
 * @param filepath Absolute path to the file
 * @return Georeference matrix with 4x4 of type CV_64F
 */
cv::Mat  loadGeoreferenceFromYaml(const std::string &filepath);

/*!
 * @brief Function for loading trajectory file in TUM format. Is provided es key/value pair with timestamp/3x4 mat.
 *        Interface for directory + filename version
 * @param directory Directory if the file
 * @param filename Name of the file including suffix
 * @return Key/value pair with timestamp/3x4 pose
 */
std::unordered_map<uint64_t, cv::Mat> loadTrajectoryFromTxtTUM(const std::string &directory,
                                                               const std::string &filename);

/*!
 * @brief Function for loading trajectory file in TUM format. Is provided es key/value pair with timestamp/3x4 mat
 *        Interface for filepath one argument
 * @param directory Directory if the file
 * @param filename Name of the file including suffix
 * @return Key/value pair with timestamp/3x4 pose
 */
std::unordered_map<uint64_t, cv::Mat> loadTrajectoryFromTxtTUM(const std::string &filepath);

/*!
 * @brief Function for loading of surface point cloud from simple txt file with x, y, z format
 * @param filepath Absolute filepath of txt
 * @return Surface points as cv::Mat rowise x,y,z
 */
cv::Mat loadSurfacePointsFromTxt(const std::string &filepath);

} // namespace io
} // namespace realm

#endif //PROJECT_REALM_IMPORT_H
