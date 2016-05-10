//
//  AvailableChar.cpp
//  ocr
//
//  Created by Rajathi, Shabana on 4/3/16.
//  Copyright © 2016 Rajathi, Shabana. All rights reserved.
//

#include "AvailableChar.hpp"


///////////////////////////////////////////////////////////////////////////////////////////////////
AvailableChar::AvailableChar(std::vector<cv::Point> _contour)
{
    contour = _contour;
    
    RectangleBound = cv::boundingRect(contour);
    
    CenterX = (RectangleBound.x + RectangleBound.x + RectangleBound.width) / 2;
    CenterY = (RectangleBound.y + RectangleBound.y + RectangleBound.height) / 2;
    
    DiagonalSize = sqrt(pow(RectangleBound.width, 2.0) + pow(RectangleBound.height, 2.0));
    
    AspectRatio = (float)RectangleBound.width / (float)RectangleBound.height;
}

