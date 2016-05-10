//
//  main.cpp
//  ocr
//
//  Created by Rajathi, Shabana on 4/3/16.
//  Copyright © 2016 Rajathi, Shabana. All rights reserved.
//

#include <fstream>
#include "Begin.hpp"

using namespace std;

/*namespace std
{
    int round(double d)
    {
        return static_cast<int>(d + 0.5);
    }
}*/

///////////////////////////////////////////////////////////////////////////////////////////////////
int begin(void)
{
    
    bool KNNsuccessful = KNNLoadAndTrain();           // KNN training
    
    if (KNNsuccessful == false)
    {
        std::cout << std::endl << std::endl << "error: error: KNN traning was not successful" << std::endl << std::endl;
        return(0);
    }
    
    cv::Mat originalImage;
    
    ifstream file("/users/srajathi/Desktop/ocr1.jpg");
    
    if(!file.is_open())
        cout << "file not open" << endl;
    else cout << "file open" << endl;
    
    std::cout << "testing" << std::endl;
    
    originalImage = cv::imread("/users/srajathi/Desktop/2.png");         // open image
    
    
    if (originalImage.empty())
    {
        std::cout << "error: image not read from file\n\n";
        return(0);
        
    }
    
    std::vector<AvailablePlate> vectorOfAvailablePlates = ExtractPlatesInScene(originalImage);          // detect plates
    
    vectorOfAvailablePlates = FindCharactersInPlates(vectorOfAvailablePlates);             // detect Characters in plates
    
    cv::imshow("originalImage", originalImage);
    
    if (vectorOfAvailablePlates.empty())
    {
        std::cout << std::endl << "no license plates were detected" << std::endl;
    }
    
    else
    {           // else
        //  here >>>>>>vector of possible plates has at least one plate
        
        // sort the vector of plates in DESCENDING order
        std::sort(vectorOfAvailablePlates.begin(), vectorOfAvailablePlates.end(), AvailablePlate::DescendingSort_By_NumberOfCharacters);
        
        
        AvailablePlate licensePlate = vectorOfAvailablePlates.front();
        
        cv::imshow("PlateImage", licensePlate.PlateImage);            // show--- crop and threshold of plate
        cv::imshow("ThresholdImage", licensePlate.ThresholdImage);
        
        
        
        /// Final cropped Plate
        //  Store to desktop
        
         cv::imwrite("/users/srajathi/Desktop/LicensePlate.jpg", licensePlate.PlateImage);
         cv::imwrite("/users/srajathi/Desktop/LicensePlateThreshold.jpg", licensePlate.ThresholdImage);
        
        
        
        if (licensePlate.strCharacters.length() == 0)
        {
            std::cout << std::endl << "no characters were detected" << std::endl << std::endl;
            return(0);
        }
        
        BorderingPlateRedRectangle(originalImage, licensePlate);
        
        std::cout << std::endl << "License Plate >>> from Image >>> = " << licensePlate.strCharacters << std::endl;
        std::cout << std::endl << "-----------------------------------------" << std::endl;
        
        LicensePlateNoOnImage_write(originalImage, licensePlate);
        
        cv::imshow("originalImage", originalImage);
        cv::imwrite("originalImage.png", originalImage);
    }
    
    cv::waitKey(0);
    return(0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//drawing the red rectangle around the detected plate



void BorderingPlateRedRectangle(cv::Mat &originalImage, AvailablePlate &licensePlate)
{
    cv::Point2f p2fRectPoints[4];
    licensePlate.rr_LocationOfPlate.points(p2fRectPoints);
    
    
    
    for (int i = 0; i < 4; i++)
    {
        cv::line(originalImage, p2fRectPoints[i], p2fRectPoints[(i + 1) % 4], SCALAR_COLOUR_RED, 2);
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//final writing of read license no unto the image
void LicensePlateNoOnImage_write(cv::Mat &originalImage, AvailablePlate &licensePlate)
{
    cv::Point ptCenterOfTextArea;
    cv::Point ptLowerLeftTextOrigin;
    
    int intFontFace = CV_FONT_HERSHEY_SIMPLEX;
    double  FontScale = (double)licensePlate.PlateImage.rows / 30.0;
    int intFontThickness = (int)std::round( FontScale * 1.5);
    int intBaseline = 0;
    
    
    cv::Size textSize = cv::getTextSize(licensePlate.strCharacters, intFontFace,  FontScale, intFontThickness, &intBaseline);
    
    ptCenterOfTextArea.x = (int)licensePlate.rr_LocationOfPlate.center.x;
    
    if (licensePlate.rr_LocationOfPlate.center.y < (originalImage.rows * 0.75))
    {
        ptCenterOfTextArea.y = (int)std::round(licensePlate.rr_LocationOfPlate.center.y) + (int)std::round((double)licensePlate.PlateImage.rows * 1.6);
    }
    else
    {
        ptCenterOfTextArea.y = (int)std::round(licensePlate.rr_LocationOfPlate.center.y) - (int)std::round((double)licensePlate.PlateImage.rows * 1.6);
    }
    
    ptLowerLeftTextOrigin.x = (int)(ptCenterOfTextArea.x - (textSize.width / 2));
    ptLowerLeftTextOrigin.y = (int)(ptCenterOfTextArea.y + (textSize.height / 2));
    
    // write text on image
    cv::putText(originalImage, licensePlate.strCharacters, ptLowerLeftTextOrigin, intFontFace,  FontScale, SCALAR_COLOUR_YELLOW, intFontThickness);
}
