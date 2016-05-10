//
//  ExtractPlates.cpp
//  ocr
//
//  Created by Rajathi, Shabana on 4/3/16.
//  Copyright © 2016 Rajathi, Shabana. All rights reserved.
//

#include "ExtractPlates.hpp"


///////////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------
std::vector<AvailablePlate> ExtractPlatesInScene(cv::Mat &originalImage)
{
    std::vector<AvailablePlate> vectorOfAvailablePlates;
    
    cv::Mat GrayscaleImage;
    cv::Mat ThresholdImage;
    cv::Mat ImageContours(originalImage.size(), CV_8UC3, SCALAR_COLOUR_BLACK);
    
    cv::RNG rng;
    
    cv::destroyAllWindows();
    
#ifdef SEQUENCE_STEPS
				cv::imshow("0", originalImage);
#endif	// SEQUENCE_STEPS
    
    Pre_Process(originalImage, GrayscaleImage, ThresholdImage);
    
    
#ifdef SEQUENCE_STEPS
				cv::imshow("1a", GrayscaleImage);
				cv::imshow("1b", ThresholdImage);
#endif	// SEQUENCE_STEPS
    
    
    std::vector<AvailableChar> vectorOfAvailableCharactersInScene = findAvailableCharactersInScene(ThresholdImage);

    
#ifdef SEQUENCE_STEPS
				std::cout << "step 2 - vectorOfAvailableCharactersInScene.Count = " << vectorOfAvailableCharactersInScene.size() << std::endl;
				ImageContours = cv::Mat(originalImage.size(), CV_8UC3, SCALAR_COLOUR_BLACK);
				std::vector<std::vector<cv::Point> > contours;
    
				for (auto AvailableChar = vectorOfAvailableCharactersInScene.begin() ; AvailableChar != vectorOfAvailableCharactersInScene.end() ; ++AvailableChar)
                {
                    contours.push_back(AvailableChar->contour);
                }
				cv::drawContours(ImageContours, contours, -1, SCALAR_COLOUR_WHITE);
				cv::imshow("2b", ImageContours);
    //

#endif	// SEQUENCE_STEPS
    
    
    std::vector<std::vector<AvailableChar> > vectorOfVectorsOfMatching_CharactersInScene = find_VectorOf_VectorsOfMatching_Characters(vectorOfAvailableCharactersInScene);
    
#ifdef SEQUENCE_STEPS
				std::cout << "step 3 - vectorOfVectorsOfMatching_CharactersInScene.size() = " << vectorOfVectorsOfMatching_CharactersInScene.size() << std::endl;
				ImageContours = cv::Mat(originalImage.size(), CV_8UC3, SCALAR_COLOUR_BLACK);
    
				for (auto vectorOfMatching_Characters = vectorOfVectorsOfMatching_CharactersInScene.begin() ; vectorOfMatching_Characters != vectorOfVectorsOfMatching_CharactersInScene.end() ; ++vectorOfMatching_Characters)
                {
                    int RandomBlue = rng.uniform(0, 256);
                    int RandomGreen = rng.uniform(0, 256);
                    int RandomRed = rng.uniform(0, 256);
                    
                    std::vector<std::vector<cv::Point> > contours;
                    
                    for (auto matchingChar = vectorOfMatching_Characters->begin() ;matchingChar != vectorOfMatching_Characters->end() ; ++matchingChar )
                    {
                        contours.push_back(matchingChar->contour);
                    }
                    cv::drawContours(ImageContours, contours, -1, cv::Scalar((double)RandomBlue, (double)RandomGreen, (double)RandomRed));
                }
				cv::imshow("3", ImageContours);
#endif	// SEQUENCE_STEPS
    
    
    for (auto vectorOfMatching_Characters = vectorOfVectorsOfMatching_CharactersInScene.begin() ; vectorOfMatching_Characters != vectorOfVectorsOfMatching_CharactersInScene.end() ; ++vectorOfMatching_Characters)
    {
        AvailablePlate availablePlate = extract_Plate(originalImage, *vectorOfMatching_Characters);        // extract plate
        
        if (availablePlate.PlateImage.empty() == false)
        {                                              // if plate was found
            vectorOfAvailablePlates.push_back(availablePlate);                    // add to vector of possible available plates
        }
    }
    
    std::cout << std::endl << vectorOfAvailablePlates.size() << " plates found" << std::endl;
    
    
#ifdef SEQUENCE_STEPS
				std::cout << std::endl;
				cv::imshow("4a", ImageContours);
    
				for (unsigned int i = 0; i < vectorOfAvailablePlates.size(); i++)
                {
                    cv::Point2f p2fRectPoints[4];
                    vectorOfAvailablePlates[i].rr_LocationOfPlate.points(p2fRectPoints);
                    
                    for (int j = 0; j < 4; j++)
                    {
                        cv::line(ImageContours, p2fRectPoints[j], p2fRectPoints[(j + 1) % 4], SCALAR_COLOUR_RED, 2);
                    }
                    cv::imshow("4a", ImageContours);
                    
                    std::cout << "possible plate " << i << ", click on image and press any key to continue . . ." << std::endl;
                    cv::imshow("4b", vectorOfAvailablePlates[i].PlateImage);
                    cv::waitKey(0);
                }
    
    
    
				std::cout << std::endl << "plate detection completed, click on any image and press a key to begin char recognition . . ." << std::endl << std::endl;
				cv::waitKey(0);
#endif	// SEQUENCE_STEPS
    
    
    return vectorOfAvailablePlates;
}




///////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------------
std::vector<AvailableChar> findAvailableCharactersInScene(cv::Mat &ThresholdImage)
{
    std::vector<AvailableChar> vectorOfAvailableCharacters;
    
    cv::Mat ImageContours(ThresholdImage.size(), CV_8UC3, SCALAR_COLOUR_BLACK);
    int CountOfAvailableCharacters = 0;
    cv::Mat ThresholdImageCopy = ThresholdImage.clone();
    std::vector<std::vector<cv::Point> > contours;
    
    cv::findContours(ThresholdImageCopy, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
    
    for (unsigned int i = 0; i < contours.size(); i++)
    {
        // for each contour
        
#ifdef SEQUENCE_STEPS
        cv::drawContours(ImageContours, contours, i, SCALAR_COLOUR_WHITE);
#endif	// SEQUENCE_STEPS
        
        AvailableChar availableChar(contours[i]);
        
        if (checkIfAvailableChar(availableChar))
        {
            CountOfAvailableCharacters++;
            vectorOfAvailableCharacters.push_back(availableChar);
        }
    }
    
    
#ifdef SEQUENCE_STEPS
    std::cout << std::endl << "contours.size() = " << contours.size() << std::endl;
    std::cout << "step 2 - intCountOfValidAvailableCharacters = " << CountOfAvailableCharacters << std::endl;
    cv::imshow("2a", ImageContours);
#endif	// SEQUENCE_STEPS

    
    return(vectorOfAvailableCharacters);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------
AvailablePlate extract_Plate(cv::Mat &ImageOriginal, std::vector<AvailableChar> &vectorOfMatching_Characters)
{
    AvailablePlate availablePlate;
    
    // sorting Characters from left to right based on x position
    std::sort(vectorOfMatching_Characters.begin(), vectorOfMatching_Characters.end(), AvailableChar::sortCharacters_LeftToRight);
    
    // calculating the center point of the plate
    double PlateCenterX = (double)(vectorOfMatching_Characters[0].CenterX + vectorOfMatching_Characters[vectorOfMatching_Characters.size() - 1].CenterX) / 2.0;
    double PlateCenterY = (double)(vectorOfMatching_Characters[0].CenterY + vectorOfMatching_Characters[vectorOfMatching_Characters.size() - 1].CenterY) / 2.0;
    
    cv::Point2d p2d_Plate_Center(PlateCenterX, PlateCenterY);
    
    // calculating plate width and height
    int Plate_Width = (int)((vectorOfMatching_Characters[vectorOfMatching_Characters.size() - 1].RectangleBound.x + vectorOfMatching_Characters[vectorOfMatching_Characters.size() - 1].RectangleBound.width - vectorOfMatching_Characters[0].RectangleBound.x) * PADDING_FACTOR_PLATE_WIDTH);
    
    double TotalOfCharacterHeights = 0;
    
    for (auto Matching_Characters = vectorOfMatching_Characters.begin() ; Matching_Characters != vectorOfMatching_Characters.end() ; ++Matching_Characters)
    {
        TotalOfCharacterHeights = TotalOfCharacterHeights + Matching_Characters->RectangleBound.height;
    }
    
    double  AverageCharacterHeight = (double)TotalOfCharacterHeights / vectorOfMatching_Characters.size();
    
    int Plate_Height = (int)( AverageCharacterHeight * PADDING_FACTOR_PLATE_HEIGHT);
    
    // calculating correction angle of plate region
    double  Opposite = vectorOfMatching_Characters[vectorOfMatching_Characters.size() - 1].CenterY - vectorOfMatching_Characters[0].CenterY;
    double  Hypotenuse = distance_Between_Characters(vectorOfMatching_Characters[0], vectorOfMatching_Characters[vectorOfMatching_Characters.size() - 1]);
    double  CorrectionAngleInRad = asin( Opposite /  Hypotenuse);
    double  CorrectionAngleInDeg =  CorrectionAngleInRad * (180.0 / CV_PI);
    
    
    // assigning rotated rect member variable of possible plate
    availablePlate.rr_LocationOfPlate = cv::RotatedRect(p2d_Plate_Center, cv::Size2f((float)Plate_Width, (float)Plate_Height), (float) CorrectionAngleInDeg);
    
    
    cv::Mat Rotation_Matrix;
    cv::Mat ImageRotated;
    cv::Mat ImageCropped;
    
    Rotation_Matrix = cv::getRotationMatrix2D(p2d_Plate_Center,  CorrectionAngleInDeg, 1.0);
    
    cv::warpAffine(ImageOriginal, ImageRotated, Rotation_Matrix, ImageOriginal.size());            // rotate the entire image
    
    // crop out required plate portion of the rotated image
    cv::getRectSubPix(ImageRotated, availablePlate.rr_LocationOfPlate.size, availablePlate.rr_LocationOfPlate.center, ImageCropped);
    
    availablePlate.PlateImage = ImageCropped;            
    
    return(availablePlate);
}