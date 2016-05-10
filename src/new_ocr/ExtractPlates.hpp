//
//  ExtractPlates.hpp
//  ocr
//
//  Created by Rajathi, Shabana on 4/3/16.
//  Copyright © 2016 Rajathi, Shabana. All rights reserved.
//

#ifndef ExtractPlates_hpp
#define ExtractPlates_hpp

#include <stdio.h>


#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include "Main.h"
#include "AvailablePlate.hpp"
#include "AvailableChar.hpp"
#include "Pre_process.hpp"
#include "FindChars.hpp"

// global constants ///////////////////////////////////////////////////////////////////////////////
const double PADDING_FACTOR_PLATE_WIDTH = 1.3;
const double PADDING_FACTOR_PLATE_HEIGHT = 1.5;

// function prototypes ////////////////////////////////////////////////////////////////////////////
std::vector<AvailablePlate> ExtractPlatesInScene(cv::Mat &originalImage);
std::vector<AvailableChar> findAvailableCharactersInScene(cv::Mat &ThresholdImage);
AvailablePlate extract_Plate(cv::Mat &imgOriginal, std::vector<AvailableChar> &vectorOfMatching_Characters);

#endif /* ExtractPlates_hpp */
