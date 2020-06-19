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

#ifndef PROJECT_PLANE_FITTER_H
#define PROJECT_PLANE_FITTER_H

#include <realm_core/structs.h>

namespace realm
{

class PlaneFitter
{
  public:
    struct Point
    {
        double x;
        double y;
        double z;
    };
    struct Vector
    {
        double x;
        double y;
        double z;
    };
    typedef Vector Normal;
    struct Plane
    {
        PlaneFitter::Point pt;
        PlaneFitter::Normal n;
    };
  public:
    /*!
     * @brief Function for least squares plane estimation. If provided with exactly 3 points, an exact solution is
     *        computed. Otherwise a least squares formulation based on SVD decomposition is performed.
     * @param points At least the 3 points must be provided
     * @return Fitted plane through the data points
     */
    PlaneFitter::Plane estimate(const std::vector<PlaneFitter::Point> &points);

  private:
    /*!
     * @brief Private function to compute the exact solution for the plane normal from a vector of 3 points
     * @param points Vector of 3 points
     * @return Exact plane normal
     */
    PlaneFitter::Normal computeExactPlaneNormal(const std::vector<PlaneFitter::Point> &points);

    /*!
     * @brief Private function to compute the exact solution for the plane centroid from a vector of 3 points
     * @param points Vector of 3 points
     * @return Exact plane centroid
     */
    PlaneFitter::Point computeExactPlaneCentroid(const std::vector<PlaneFitter::Point> &points);
};

} // namespace realm

#endif //PROJECT_PLANE_FITTER_H
