//
//  AvailableChar.hpp
//  ocr
//
//  Created by Rajathi, Shabana on 4/3/16.
//  Copyright © 2016 Rajathi, Shabana. All rights reserved.
//

#ifndef AvailableChar_hpp
#define AvailableChar_hpp

#include <stdio.h>

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////
class AvailableChar
{
public:
    
    // member variables //
    std::vector<cv::Point> contour;
    
    cv::Rect RectangleBound;
    
    int CenterX;
    int CenterY;
    
    double DiagonalSize;
    double AspectRatio;
    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    static bool sortCharacters_LeftToRight(const AvailableChar &pcLeft, const AvailableChar & pcRight)
    {
        return(pcLeft.CenterX < pcRight.CenterX);
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    bool operator == (const AvailableChar& otherAvailableChar) const
    {
        if (this->contour == otherAvailableChar.contour) return true;
        else return false;
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    bool operator != (const AvailableChar& otherAvailableChar) const
    {
        if (this->contour != otherAvailableChar.contour) return true;
        else return false;
    }
    
    // function prototypes //
    AvailableChar(std::vector<cv::Point> _contour);
    
};


#endif /* AvailableChar_hpp */
