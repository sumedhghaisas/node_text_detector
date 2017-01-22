//
//  FindChars.hpp
//  ocr
//
//  Created by Rajathi, Shabana on 4/3/16.
//  Copyright © 2016 Rajathi, Shabana. All rights reserved.
//

#ifndef FindChars_hpp
#define FindChars_hpp

#include <stdio.h>


#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/ml/ml.hpp>

#include "Main.h"
#include "AvailableChar.hpp"
#include "AvailablePlate.hpp"
#include "Pre_process.hpp"

// global constants ///////////////////////////////////////////////////////////////////////////////

// constants for checkIfAvailableChar, this checks one possible char only (ant not compare to another char)

const int MINIMUM_PIXEL_WIDTH = 2;
const int MINIMUM_PIXEL_HEIGHT = 8;

const double MINIMUM_ASPECT_RATIO = 0.25;
const double MAXIMUM_ASPECT_RATIO = 1.0;

const int MINIMUM_PIXEL_AREA = 80;


// constants for comparing two Characters
const double MINIMUM_DIAGONAL_SIZE_MULTIPLE_AWAY = 0.3;
const double MAXIMUM_DIAGONAL_SIZE_MULTIPLE_AWAY = 5.0;

const double MAXIMUM_CHANGE_IN_AREA = 0.5;
const double MAXIMUM_CHANGE_IN_WIDTH = 0.8;
const double MAXIMUM_CHANGE_IN_HEIGHT = 0.2;
const double MAXIMUM_ANGLE_BETWEEN_CHARS = 12.0;



// other constants
const int MINIMUM_NUMBER_OF_MATCHING_CHARS = 3;
const int RESIZED_CHARACTER_IMAGE_WIDTH = 20;
const int RESIZED_CHARACTER_IMAGE_HEIGHT = 30;
const int MINIMUM_CONTOUR_AREA = 100;


// external global variables //////////////////////////////////////////////////////////////////////
extern const bool Show_Steps;
extern cv::Ptr<cv::ml::KNearest>  NearestKs;


// function prototypes ////////////////////////////////////////////////////////////////////////////

bool KNNLoadAndTrain(void);

std::vector<AvailablePlate> FindCharactersInPlates(std::vector<AvailablePlate> &vectorOfAvailablePlates);

std::vector<AvailableChar> findAvailableCharactersInPlate(cv::Mat &GrayscaleImage, cv::Mat &ThresholdImage);

bool checkIfAvailableChar(AvailableChar &possibleChar);

std::vector<std::vector<AvailableChar> > find_VectorOf_VectorsOfMatching_Characters(const std::vector<AvailableChar> &vectorOfAvailableCharacters);

std::vector<AvailableChar> findVectorOfMatching_Characters(const AvailableChar &possibleChar, const std::vector<AvailableChar> &vectorOfCharacters);

double distance_Between_Characters(const AvailableChar &firstChar, const AvailableChar &secondChar);

double angle_Between_Characters(const AvailableChar &firstChar, const AvailableChar &secondChar);

std::vector<AvailableChar> remove_Inner_Overlapping_Characters(std::vector<AvailableChar> &vectorOfMatching_Characters);

std::string recog_CharactersInPlate(cv::Mat &ThresholdImage, std::vector<AvailableChar> &vectorOfMatching_Characters);



#endif /* FindChars_hpp */
