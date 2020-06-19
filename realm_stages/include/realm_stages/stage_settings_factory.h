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

#ifndef PROJECT_STAGE_SETTINGS_FACTORY_H
#define PROJECT_STAGE_SETTINGS_FACTORY_H

#include <realm_io/utilities.h>
#include <realm_stages/stage_settings.h>

namespace realm
{

class StageSettingsFactory
{
  public:
    static StageSettings::Ptr load(const std::string &stage_type_set, const std::string &filepath);
  private:
    template <typename T>
    static StageSettings::Ptr loadDefault(const std::string &stage_type_set, const std::string &filepath);
};

} // namespace realm

#endif //PROJECT_STAGE_SETTINGS_FACTORY_H
