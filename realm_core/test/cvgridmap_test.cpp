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

#include <iostream>
#include <realm_core/cv_grid_map.h>

// gtest
#include <gtest/gtest.h>

using namespace realm;

TEST(CvGridMap, CopyConstructor)
{
  // For this test we create a CvGridMap and use the copy constructor to create a copy. Then we check if all members
  // in the copy are equal to the original.
  CvGridMap map(cv::Rect2d(5, 10, 20, 25), 2.0);
  map.add("layer_double", cv::Mat(map.size(), CV_64F, 3.1415));
  map.add("layer_char", cv::Mat(map.size(), CV_8UC1, 125));

  CvGridMap mapCopy(map);

  EXPECT_DOUBLE_EQ(mapCopy.resolution(), 2.0);
  EXPECT_DOUBLE_EQ(mapCopy.roi().x, map.roi().x);
  EXPECT_DOUBLE_EQ(mapCopy.roi().y, map.roi().y);
  EXPECT_DOUBLE_EQ(mapCopy.roi().width, map.roi().width);
  EXPECT_DOUBLE_EQ(mapCopy.roi().height, map.roi().height);
  EXPECT_EQ(mapCopy.size().width, 11);
  EXPECT_EQ(mapCopy.size().height, 14);
  EXPECT_DOUBLE_EQ(map["layer_double"].at<double>(5, 5), 3.1415);
  EXPECT_EQ(map["layer_char"].at<char>(5, 5), 125);
}

TEST(CvGridMap, CopyAssign)
{
  // For this test we create a CvGridMap and use the assignment operator to create a copy. Then we check if all members
  // in the copy are equal to the original.
  CvGridMap map(cv::Rect2d(5, 10, 20, 25), 2.0);
  map.add("layer_double", cv::Mat(map.size(), CV_64F, 3.1415));
  map.add("layer_char", cv::Mat(map.size(), CV_8UC1, 125));

  CvGridMap mapCopy = map;

  EXPECT_DOUBLE_EQ(mapCopy.resolution(), 2.0);
  EXPECT_DOUBLE_EQ(mapCopy.roi().x, map.roi().x);
  EXPECT_DOUBLE_EQ(mapCopy.roi().y, map.roi().y);
  EXPECT_DOUBLE_EQ(mapCopy.roi().width, map.roi().width);
  EXPECT_DOUBLE_EQ(mapCopy.roi().height, map.roi().height);
  EXPECT_EQ(mapCopy.size().width, 11);
  EXPECT_EQ(mapCopy.size().height, 14);
  EXPECT_DOUBLE_EQ(map["layer_double"].at<double>(5, 5), 3.1415);
  EXPECT_EQ(map["layer_char"].at<char>(5, 5), 125);
}

TEST(CvGridMap, AtIndex)
{
  CvGridMap map(cv::Rect2d(0, 0, 20, 10), 0.5);
  map.add("layer_double", cv::Mat(map.size(), CV_64F, 3.1415));

  // Get the index at the corner elements
  cv::Rect2d roi = map.roi();
  cv::Point2i ulc = map.atIndex(cv::Point2d(0.0, roi.height));
  cv::Point2i urc = map.atIndex(cv::Point2d(roi.width, roi.height));
  cv::Point2i lrc = map.atIndex(cv::Point2d(roi.width, 0.0));
  cv::Point2i llc = map.atIndex(cv::Point2d(0.0, 0.0));

  cv::Size2i size = map.size();
  EXPECT_EQ(ulc.x, 0);
  EXPECT_EQ(ulc.y, 0);
  EXPECT_EQ(urc.x, size.width-1);
  EXPECT_EQ(urc.y, 0);
  EXPECT_EQ(lrc.x, size.width-1);
  EXPECT_EQ(lrc.y, size.height-1);
  EXPECT_EQ(llc.x, 0);
  EXPECT_EQ(llc.y, size.height-1);
}

TEST(CvGridMap, ContinuousIndexing)
{
  // We noticed a bug, induced by rounding errors. Sometimes the access to the index within a matrix from world
  // coordinates was referring to the same matrix element twice and skipping one after that. This usually happened
  // due to a cast from double to integer without rounding it before. Therefore this test is checking if all elements
  // of a world grid are referring to a unique matrix element when we move with step size 'resolution'
  CvGridMap map(cv::Rect2d(0.0, 0.0, 1000.0, 1000.0), 0.1);
  map.add("layer_char", cv::Mat(map.size(), CV_8UC1, 125));

  cv::Rect2d roi = map.roi();
  cv::Point2i idx_prior(-1, -1);

  for (int i = 0; i < 10001; ++i)
  {
    cv::Point2i idx = map.atIndex(cv::Point2d(roi.x + static_cast<double>(i)/10, 0));
    EXPECT_TRUE(idx.x != idx_prior.x);
    idx_prior = idx;
  }

  // Do it in both directions once just to make sure
  idx_prior.x = -1; idx_prior.y = -1;
  for (int i = 0; i < 10001; ++i)
  {
    cv::Point2i idx = map.atIndex(cv::Point2d(0, roi.y + static_cast<double>(i)/10));
    EXPECT_TRUE(idx.y != idx_prior.y);
    idx_prior = idx;
  }
}

TEST(CvGridMap, IndexROI)
{
  CvGridMap map(cv::Rect2d(632524, 14634666, 1000, 1200), 0.1);
  map.add("layer_double", cv::Mat(map.size(), CV_64F, 3.1415));

  EXPECT_EQ(map.atIndexROI(map.roi()).width, map.size().width);
  EXPECT_EQ(map.atIndexROI(map.roi()).height, map.size().height);
}

TEST(CvGridMap, AtPosition2D)
{
  CvGridMap map(cv::Rect2d(632524, 14634666, 1000, 1200), 0.1);
  map.add("layer_double", cv::Mat(map.size(), CV_64F, 3.1415));

  cv::Size2i size = map.size();
  cv::Point2d ulc = map.atPosition2d(0, 0);
  cv::Point2d urc = map.atPosition2d(0, size.width-1);
  cv::Point2d lrc = map.atPosition2d(size.height-1, size.width-1);
  cv::Point2d llc = map.atPosition2d(size.height-1, 0);

  EXPECT_DOUBLE_EQ(ulc.x, 632524.0);
  EXPECT_DOUBLE_EQ(urc.x, 632524.0 + 1000.0);
  EXPECT_DOUBLE_EQ(lrc.x, 632524.0 + 1000.0);
  EXPECT_DOUBLE_EQ(llc.x, 632524.0);
  EXPECT_DOUBLE_EQ(ulc.y, 14634666 + 1200.0);
  EXPECT_DOUBLE_EQ(urc.y, 14634666 + 1200.0);
  EXPECT_DOUBLE_EQ(lrc.y, 14634666);
  EXPECT_DOUBLE_EQ(llc.y, 14634666);
}

TEST(CvGridMap, AtPosition3D)
{
  CvGridMap map(cv::Rect2d(632524, 14634666, 1000, 1200), 0.1);
  map.add("layer_double", cv::Mat(map.size(), CV_64F, 3.1415));

  // Set the corner elements to a different value
  cv::Mat layer_data = map["layer_double"];
  layer_data.at<double>(0, 0) = 4.1415;
  layer_data.at<double>(0, layer_data.cols-1) = 5.1415;
  layer_data.at<double>(layer_data.rows-1, layer_data.cols-1) = 6.1415;
  layer_data.at<double>(layer_data.rows-1, 0) = 7.1415;

  cv::Size2i size = map.size();
  cv::Point3d ulc = map.atPosition3d(0, 0, "layer_double");
  cv::Point3d urc = map.atPosition3d(0, size.width-1, "layer_double");
  cv::Point3d lrc = map.atPosition3d(size.height-1, size.width-1, "layer_double");
  cv::Point3d llc = map.atPosition3d(size.height-1, 0, "layer_double");

  EXPECT_DOUBLE_EQ(ulc.x, 632524.0);
  EXPECT_DOUBLE_EQ(urc.x, 632524.0 + 1000.0);
  EXPECT_DOUBLE_EQ(lrc.x, 632524.0 + 1000.0);
  EXPECT_DOUBLE_EQ(llc.x, 632524.0);
  EXPECT_DOUBLE_EQ(ulc.y, 14634666 + 1200.0);
  EXPECT_DOUBLE_EQ(urc.y, 14634666 + 1200.0);
  EXPECT_DOUBLE_EQ(lrc.y, 14634666);
  EXPECT_DOUBLE_EQ(llc.y, 14634666);
  EXPECT_DOUBLE_EQ(ulc.z, 4.1415);
  EXPECT_DOUBLE_EQ(urc.z, 5.1415);
  EXPECT_DOUBLE_EQ(lrc.z, 6.1415);
  EXPECT_DOUBLE_EQ(llc.z, 7.1415);
}

TEST(CvGridMap, AddSubmap_Overwrite)
{
  // Now we check if two CvGridMaps can be added to one another. This is rather complex, as there is different layers
  // which might be added with different data types. This is not robust for all possible cases, so it is advised to
  // stick to the standard use cases. At first we add a Submap that is already contained in the other.
  CvGridMap map1(cv::Rect2d(5, 10, 20, 28), 2.0);
  map1.add("layer_double", cv::Mat(map1.size(), CV_64F, 3.1415));
  map1.add("layer_char", cv::Mat(map1.size(), CV_8UC1, 125));

  CvGridMap map2(cv::Rect2d(9, 13, 10, 14), 2.0);
  map2.add("layer_double", cv::Mat(map2.size(), CV_64F, 6.1415));
  map2.add("layer_char", cv::Mat(map2.size(), CV_8UC1, 250));

  map1.add(map2, REALM_OVERWRITE_ALL, false);

  EXPECT_DOUBLE_EQ(map1["layer_double"].at<double>(7, 5), 6.1415);
  EXPECT_EQ(map1["layer_char"].at<uint8_t>(7, 5), 250);
}

TEST(CvGridMap, AddSubmap_NoExtend)
{
  // While in the prior test we checked, if we can add a submap that is already contained within the reference map, we
  // will now add a submap whose region of interest (roi) is extending the reference map. We will first try adding the
  // map without allowing to extend the reference (note how map2's x/y is different to prior test):
  CvGridMap map1(cv::Rect2d(5, 10, 20, 28), 2.0);
  map1.add("layer_double", cv::Mat(map1.size(), CV_64F, 3.1415));
  map1.add("layer_char", cv::Mat(map1.size(), CV_8UC1, 125));

  CvGridMap map2(cv::Rect2d(20, 31, 10, 14), 2.0);
  map2.add("layer_double", cv::Mat(map2.size(), CV_64F, 6.1415));
  map2.add("layer_char", cv::Mat(map2.size(), CV_8UC1, 250));

  map1.add(map2, REALM_OVERWRITE_ALL, false);

  EXPECT_DOUBLE_EQ(map1["layer_double"].at<double>(0, 9), 6.1415);
  EXPECT_EQ(map1["layer_char"].at<uchar>(0, 9), 250);
}

TEST(CvGridMap, AddSubmap_Extend)
{
  // While in the prior test we checked, if we can add a submap that is already contained within the reference map, we
  // will now add a submap whose region of interest (roi) is extending the reference map. We will first try adding the
  // map without allowing to extend the reference (note how map2's x/y is different to prior test):
  CvGridMap map1(cv::Rect2d(5, 10, 20, 28), 2.0);
  map1.add("layer_double", cv::Mat(map1.size(), CV_64F, 3.1415));
  map1.add("layer_char", cv::Mat(map1.size(), CV_8UC1, 125));

  CvGridMap map2(cv::Rect2d(20, 31, 10, 14), 2.0);
  map2.add("layer_double", cv::Mat(map2.size(), CV_64F, 6.1415));
  map2.add("layer_char", cv::Mat(map2.size(), CV_8UC1, 250));

  map1.add(map2, REALM_OVERWRITE_ALL, true);

  EXPECT_DOUBLE_EQ(map1["layer_double"].at<double>(2, 9), 6.1415);
  EXPECT_EQ(map1["layer_char"].at<uchar>(2, 9), 250);
}

TEST(CvGridMap, Overlap)
{
  //
  CvGridMap map1(cv::Rect2d(0, 0, 20, 30), 1.0);
  map1.add("layer_double", cv::Mat(map1.size(), CV_64F, 3.1415));
  map1.add("layer_char", cv::Mat(map1.size(), CV_8UC1, 125));

  CvGridMap map2(cv::Rect2d(10, 15, 20, 30), 1.0);
  map2.add("layer_double", cv::Mat(map2.size(), CV_64F, 6.1415));

  CvGridMap::Overlap overlap = map1.getOverlap(map2);

  cv::Rect2d roi1 = overlap.first->roi();
  cv::Rect2d roi2 = overlap.second->roi();

  cv::Size2i size1 = overlap.first->size();
  cv::Size2i size2 = overlap.second->size();

  EXPECT_TRUE(overlap.first->exists("layer_double"));
  EXPECT_TRUE(overlap.first->exists("layer_char"));
  EXPECT_TRUE(overlap.second->exists("layer_double"));
  EXPECT_FALSE(overlap.second->exists("layer_char"));
  EXPECT_DOUBLE_EQ(roi1.x, 10.0);
  EXPECT_DOUBLE_EQ(roi1.y, 15.0);
  EXPECT_DOUBLE_EQ(roi1.width, 10.0);
  EXPECT_DOUBLE_EQ(roi1.height, 15.0);
  EXPECT_EQ(size1.width, 11);
  EXPECT_EQ(size1.height, 16);
  EXPECT_DOUBLE_EQ(roi1.x, roi2.x);
  EXPECT_DOUBLE_EQ(roi1.y, roi2.y);
  EXPECT_DOUBLE_EQ(roi1.width, roi2.width);
  EXPECT_DOUBLE_EQ(roi1.height, roi2.height);
  EXPECT_DOUBLE_EQ(size1.width, size2.width);
  EXPECT_DOUBLE_EQ(size1.height, size2.height);
}