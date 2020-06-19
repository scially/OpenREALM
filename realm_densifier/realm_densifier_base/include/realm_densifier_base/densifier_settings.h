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

#ifndef PROJECT_DENSIFIER_SETTINGS_H
#define PROJECT_DENSIFIER_SETTINGS_H

#include <realm_core/settings_base.h>
#include <realm_io/utilities.h>


namespace realm
{

/*!
 * @brief Settings files for densifier provide at least "type" and "resizing" parameter. Type identifies the framework
 *        by name, e.g. PSL. Resizing factor sets the downscaling of input images, 1.0 is full sized, 0.5 half sized, ...
 *        Concrete frameworks can specify more parameters. In REALM we load all important informations from .yaml files.
 */
class DensifierSettings : public SettingsBase
{
  public:
    using Ptr = std::shared_ptr<DensifierSettings>;
    using ConstPtr = std::shared_ptr<const DensifierSettings>;
  public:
    DensifierSettings()
    {
      add("type", Parameter_t<std::string>{"", "Framework for 3D scene reconstruction."});
      add("resizing", Parameter_t<double>{0.0, "Resize factor of input images."});
    }
};

class DensifierDummySettings : public DensifierSettings
{
  public:
    DensifierDummySettings()
    {
    }
};


class PlaneSweepSettings : public DensifierSettings
{
  public:
    PlaneSweepSettings()
    {
      add("n_cams", Parameter_t<int>{0, ""});
      add("enable_subpix", Parameter_t<int>{0, ""});
      add("enable_color_match", Parameter_t<int>{0, ""});
      add("enable_out_best_depth", Parameter_t<int>{0, ""});
      add("enable_out_best_cost", Parameter_t<int>{0, ""});
      add("enable_out_cost_vol", Parameter_t<int>{0, ""});
      add("enable_out_uniq_ratio", Parameter_t<int>{0, ""});
      add("scale", Parameter_t<double>{1.0, ""});
      add("nrof_planes", Parameter_t<int>{0, ""});
      add("match_window_size_x", Parameter_t<int>{0, ""});
      add("match_window_size_y", Parameter_t<int>{0, ""});
      add("occlusion_mode", Parameter_t<int>{0, ""});
      add("plane_gen_mode", Parameter_t<int>{0, ""});
      add("match_cost", Parameter_t<int>{0, ""});
      add("subpx_interp_mode", Parameter_t<int>{0, ""});
    }
};


} // namespace realm

#endif //PROJECT_DENSIFIER_SETTINGS_H
