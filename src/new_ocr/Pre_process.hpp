//
//  Pre_process.hpp
//  ocr
//
//  Created by Rajathi, Shabana on 4/3/16.
//  Copyright © 2016 Rajathi, Shabana. All rights reserved.
//

#ifndef Pre_process_hpp
#define Pre_process_hpp

#include <stdio.h>


#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

// global variables //
const cv::Size SIZE_OF_GAUSSIAN_SMOOTH_FILTER = cv::Size(5, 5);
const int BLOCK_SIZE_OF_ADAPTIVE_THRESH = 19;
const int WEIGHT_OF_ADAPTIVE_THRESH = 9;

// function prototypes //

void Pre_Process(cv::Mat &image_Original, cv::Mat &GrayscaleImage, cv::Mat &ThresholdImage);

cv::Mat extract_Value(cv::Mat &image_Original);

cv::Mat maximize_Contrast(cv::Mat &GrayscaleImage);



#endif /* Pre_process_hpp */
