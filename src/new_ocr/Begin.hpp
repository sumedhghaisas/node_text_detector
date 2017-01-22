//
//  Main.h
//  ocr
//
//  Created by Rajathi, Shabana on 4/3/16.
//  Copyright © 2016 Rajathi, Shabana. All rights reserved.
//

#ifndef Begin_hpp
#define Begin_hpp


#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include "ExtractPlates.hpp"
#include "AvailablePlate.hpp"
#include "FindChars.hpp"

#include<iostream>

#define SEQUENCE_STEPS

// global constants ///////////////////////////////////////////////////////////////////////////////
const cv::Scalar SCALAR_COLOUR_BLACK = cv::Scalar(0.0, 0.0, 0.0);
const cv::Scalar SCALAR_COLOUR_WHITE = cv::Scalar(255.0, 255.0, 255.0);
const cv::Scalar SCALAR_COLOUR_YELLOW = cv::Scalar(0.0, 255.0, 255.0);
const cv::Scalar SCALAR_COLOUR_GREEN = cv::Scalar(0.0, 255.0, 0.0);
const cv::Scalar SCALAR_COLOUR_RED = cv::Scalar(0.0, 0.0, 255.0);

// function prototypes ////////////////////////////////////////////////////////////////////////////
int begin(void);
void BorderingPlateRedRectangle(cv::Mat &originalImage, AvailablePlate &licensePlate);
void LicensePlateNoOnImage_write(cv::Mat &originalImage, AvailablePlate &licensePlate);



#endif /* Main_h */
