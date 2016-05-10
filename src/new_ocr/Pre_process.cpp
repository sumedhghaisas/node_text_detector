//
//  Pre_process.cpp
//  ocr
//
//  Created by Rajathi, Shabana on 4/3/16.
//  Copyright © 2016 Rajathi, Shabana. All rights reserved.
//

#include "Pre_process.hpp"


///////////////////////////////////////////////////////////////////////////////////////////////////
void Pre_Process(cv::Mat &image_Original, cv::Mat &GrayscaleImage, cv::Mat &ThresholdImage)
{
    
    GrayscaleImage = extract_Value(image_Original);
    cv::Mat image_MaxContrastGrayscale = maximize_Contrast(GrayscaleImage);
    cv::Mat image_Blurred;
    
    cv::GaussianBlur(image_MaxContrastGrayscale, image_Blurred, SIZE_OF_GAUSSIAN_SMOOTH_FILTER, 0);         

    cv::adaptiveThreshold(image_Blurred, ThresholdImage, 255.0, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY_INV, BLOCK_SIZE_OF_ADAPTIVE_THRESH, WEIGHT_OF_ADAPTIVE_THRESH);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
cv::Mat extract_Value(cv::Mat &image_Original)
{
    cv::Mat image_HSV;
    std::vector<cv::Mat> vectorOf_HSV_Images;
    cv::Mat image_Value;
    
    cv::cvtColor(image_Original, image_HSV, CV_BGR2HSV);
    cv::split(image_HSV, vectorOf_HSV_Images);
    image_Value = vectorOf_HSV_Images[2];
    
    return(image_Value);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
cv::Mat maximize_Contrast(cv::Mat &GrayscaleImage)
{
    cv::Mat image_TopHat;
    cv::Mat image_BlackHat;
    cv::Mat GrayscaleImage_plus_TopHat;
    cv::Mat GrayscaleImage_plus_TopHat_minus_BlackHat;
    
    cv::Mat structuring_Element = cv::getStructuringElement(CV_SHAPE_RECT, cv::Size(3, 3));
    
    cv::morphologyEx(GrayscaleImage, image_TopHat, CV_MOP_TOPHAT, structuring_Element);
    cv::morphologyEx(GrayscaleImage, image_BlackHat, CV_MOP_BLACKHAT, structuring_Element);
    
    GrayscaleImage_plus_TopHat = GrayscaleImage + image_TopHat;
    GrayscaleImage_plus_TopHat_minus_BlackHat = GrayscaleImage_plus_TopHat - image_BlackHat;
    
    return(GrayscaleImage_plus_TopHat_minus_BlackHat);
}



