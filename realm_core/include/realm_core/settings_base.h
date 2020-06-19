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

#ifndef PROJECT_SETTINGS_H
#define PROJECT_SETTINGS_H

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <unordered_map>
#include <exception>

#include <realm_core/structs.h>

namespace realm
{

/*!
 * @brief Idea of this class is to provide a base for a wide range of settings files, e.g. for camera calibration,
 *        stage settings, implementations within the stage and everything else that might need settings somehow.
 *        Especially the ability to load a quickly defined settings class from a .yaml file is the main motivation
 *        for this implementation.
 *        The general usage is intended to work as follows:
 *        1) Derive a class from this one, e.g.
 *              class ExampleSettings : public SettingsBase
 *              {
 *                  ...
 *              };
 *        2) Add parameters to your special settings in the constructor. This is important because the ability to add
 *           Parameters is protected and only accessible for children of SettingsBase, e.g.
 *              ExampleSettings()
 *              {
 *                  add("a_param", Parameter_t<int>{"3", "This is a help description"});
 *              }
 *        3) Either create a settings file in code and set every parameter by hand (not recommended),
 *           or write a .yaml file containing all of the parameters of your ExampleSettings, give it valid values and
 *           load them in your code using "loadFromFile".
 *        4) Optional: Note that the base class is also able to sneak parameters from the file, if necessary. This can
 *           be quite useful, if you have several derivations from the SettingsBase. A good example is the camera settings.
 *           CameraSettings is derived from SettingsBase.
 *           PinholeSettings is derived from CameraSettings.
 *           You can now provide a factory class your general "CameraSettings" and dynamically check what parameter file
 *           was provided by the user to be loaded, e.g.
 *              std::string type = sneakParameterFromFile<std::string>("type", "/path/to/yaml"))
 *              if (type == "pinhole")
 *                  do stuff;
 *              if (type == "fisheye")
 *                  do other stuff;
 *              ...
 */
class SettingsBase
{
  public:
    class Variant
    {
      friend SettingsBase;
      enum class VariantType
      {
        INT,
        DOUBLE,
        STRING
      };
      using Ptr = std::unique_ptr<Variant>;
    public:
      explicit Variant(const Parameter_t<int> &p) : _type(VariantType::INT), _int_container(p) {};
      explicit Variant(const Parameter_t<double> &p) : _type(VariantType::DOUBLE), _double_container(p) {};
      explicit Variant(const Parameter_t<std::string> &p) : _type(VariantType::STRING), _string_container(p) {};

      int toInt() const
      {
        if (_type == VariantType::INT) return _int_container.value;
        if (_type == VariantType::DOUBLE) return static_cast<int>(_double_container.value);
        if (_type == VariantType::STRING) throw(std::bad_cast());
      }

      float toFloat() const
      {
        if (_type == VariantType::INT) return static_cast<float>(_int_container.value);
        if (_type == VariantType::DOUBLE) return static_cast<float>(_double_container.value);
        if (_type == VariantType::STRING) throw(std::bad_cast());
      }

      double toDouble() const
      {
        if (_type == VariantType::INT) return static_cast<double>(_int_container.value);
        if (_type == VariantType::DOUBLE) return _double_container.value;
        if (_type == VariantType::STRING) throw(std::bad_cast());
      }

      std::string toString() const
      {
        if (_type == VariantType::INT) return std::to_string(_int_container.value);
        if (_type == VariantType::DOUBLE) return std::to_string(_double_container.value);
        if (_type == VariantType::STRING) return _string_container.value;
      }

      std::string help() const
      {
        if (_type == VariantType::INT) return _int_container.help;
        if (_type == VariantType::DOUBLE) return _double_container.help;
        if (_type == VariantType::STRING) return _string_container.help;
      }

    private:
      VariantType _type;
      Parameter_t<std::string> _string_container;
      Parameter_t<double> _double_container;
      Parameter_t<int> _int_container;
    };

  public:

    Variant operator[](const std::string &key) const;

    Variant get(const std::string &key) const;

    /*!
     * @brief Overloaded function to set a parameter of type int
     * @param key Parameter name to be set
     * @param val Value to be assigned to the parameter of name "param_name"
     */
    void set(const std::string &key, int val);

    /*!
     * @brief Overloaded function to set a parameter of type double
     * @param key Parameter name to be set
     * @param val Value to be assigned to the parameter of name "param_name"
     */
    void set(const std::string &key, double val);

    /*!
     * @brief Overloaded function to set a parameter of type string
     * @param key Parameter name to be set
     * @param val Value to be assigned to the parameter of name "param_name"
     */
    void set(const std::string &key, const std::string &val);

    /*!
     * @brief Basic functionality to load the settings from a .yaml file. For every parameter that was added in the
     *        derived class a parameter is searched for and set. If the parameter is not found, default values are kept.
     * @param filepath Absolute path to the .yaml file
     */
    void loadFromFile(const std::string &filepath);

    /*!
     * @brief Check if settings file has a certain parameter
     * @param param_name Name of the parameter to be checked
     * @return true if exists
     */
    bool has(const std::string &param_name) const;

    /*!
     * @brief Prints all parameters, their default values and the provided help description
     */
    void print();

    /*!
     * @brief Function to read parameters from a file. Can be used with the base class, so settings file can provide
     *        their own type as parameter (e.g. type: pinhole) and then be loaded dynamically.
     * @tparam T Type of the parameter to be sneaked
     * @param key Name of the parameter to be sneaked
     * @param filepath Absolute path to the settings file
     * @return Value of the parameter
     */
    template <typename T>
    static T sneakParameterFromFile(const std::string &key, const std::string &filepath)
    {
      cv::FileStorage fs(filepath, cv::FileStorage::READ);
      T val;
      if (fs.isOpened())
      {
        fs[key] >> val;
      }
      else
        throw std::invalid_argument("Error: Sneaking parameter from file " + filepath + " failed!");
      return val;
    }

  protected:
    /*!
     * @brief Add functionality for derived class to customize the settings. Can not be called from outside, to force
     *        explicit definition.
     * @param key Name of the parameter to be added
     * @param param Integer parameter containing value and description
     */
    void add(const std::string &key, const Parameter_t<int> &param);

    /*!
     * @brief Add functionality for derived class to customize the settings. Can not be called from outside, to force
     *        explicit definition.
     * @param key Name of the parameter to be added
     * @param param Double parameter containing value and description
     */
    void add(const std::string &key, const Parameter_t<double> &param);

    /*!
     * @brief Add functionality for derived class to customize the settings. Can not be called from outside, to force
     *        explicit definition.
     * @param key Name of the parameter to be added
     * @param param String parameter containing value and description
     */
    void add(const std::string &key, const Parameter_t<std::string> &param);

  private:

    //! Container for parameters
    std::unordered_map<std::string, Variant::Ptr> _parameters;
};

}

#endif //PROJECT_SETTINGS_H
