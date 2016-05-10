//
//  AvailablePlate.hpp
//  ocr
//
//  Created by Rajathi, Shabana on 4/3/16.
//  Copyright © 2016 Rajathi, Shabana. All rights reserved.
//

#ifndef AvailablePlate_hpp
#define AvailablePlate_hpp

#include <stdio.h>

#include <string>

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////
class AvailablePlate
{
public:
    // member variables //
    cv::Mat PlateImage;
    cv::Mat GrayscaleImage;
    cv::Mat ThresholdImage;
    
    cv::RotatedRect rr_LocationOfPlate;
    
    std::string strCharacters;
    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    static bool DescendingSort_By_NumberOfCharacters(const AvailablePlate &ppLeft, const AvailablePlate &ppRight)
    {
        return(ppLeft.strCharacters.length() > ppRight.strCharacters.length());
    }
    
};



#endif /* AvailablePlate_hpp */


