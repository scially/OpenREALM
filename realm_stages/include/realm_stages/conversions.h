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

#ifndef PROJECT_STAGE_CONVERSIONS_H
#define PROJECT_STAGE_CONVERSIONS_H

#include <realm_core/cv_grid_map.h>
#include <realm_core/structs.h>

namespace realm
{

/*!
 * @brief Function for conversion of visual reconstructed data to a point cloud as cv::Mat with row() = x,y,z,b,g,r,nx,ny,nz
 * @param img3d Matrix with 3 channel double precision data (accessed by cv::Vec3d) for 3d points
 * @param color Color informations
 * @param normals Matrix with 3 channel single precision normal map
 * @param mask Mask for valid elements
 * @return cv::Mat with row() = x,y,z,b,g,r,nx,ny,nz
 */
cv::Mat cvtToPointCloud(const cv::Mat &img3d, const cv::Mat &color, const cv::Mat &normals, const cv::Mat &mask);

/*!
 * @brief Function for converting a grid map to a point cloud as cv::Mat with row() = x,y,z,b,g,r,nx,ny,nz
 * @param map Grid map with root at global x,y and dimensions of width/height
 * @param layer_elevation Elevation layer: Grid map gives x,y coordinates, layer the elevation/z data.
 * @param layer_color Color layer (optional)
 * @param layer_normals Surface normal corresponding to elevation layer (optional
 * @return cv::Mat with row() = x,y,z,b,g,r,nx,ny,nz
 */
cv::Mat cvtToPointCloud(const CvGridMap &map,
                        const std::string &layer_elevation,
                        const std::string &layer_color,
                        const std::string &layer_normals,
                        const std::string &layer_mask);

/*!
 * @brief Function for converting a grid map to a mesh using triangle vertex ids
 * @param map Grid map to be converted into a mesh
 * @param layer_elevation Elevation layer used for height informations
 * @param layer_color Color layer to be used for vertices
 * @param vertex_ids Ids of the mesh triangles, 3 ids always form one triangle. Must have been build BEFORE call of this
 *                   function, e.g. with delaunay's "buildMesh"
 * @return Vector of faces
 */
std::vector<Face> cvtToMesh(const CvGridMap &map,
                            const std::string &layer_elevation,
                            const std::string &layer_color,
                            const std::vector<cv::Point2i> &vertex_ids);

} // namespace realm

#endif //PROJECT_STAGE_CONVERSIONS_H
