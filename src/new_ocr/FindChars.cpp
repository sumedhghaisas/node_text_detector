//
//  FindChars.cpp
//  ocr
//
//  Created by Rajathi, Shabana on 4/3/16.
//  Copyright © 2016 Rajathi, Shabana. All rights reserved.
//

#include "FindChars.hpp"


// global variables ///////////////////////////////////////////////////////////////////////////////
cv::Ptr<cv::ml::KNearest> NearestKs = cv::ml::KNearest::create();



///////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------------

bool KNNLoadAndTrain(void)
{
    
    cv::Mat matClassifyIntegers;
    cv::FileStorage FileStorageClassify("classify.xml", cv::FileStorage::READ);
    
    if (FileStorageClassify.isOpened() == false)
    {
        std::cout << "error, unable to open training classifications file, exiting program\n\n";
        return(false);
    }
    
    FileStorageClassify["classifications"] >> matClassifyIntegers;
    FileStorageClassify.release();

    
    cv::Mat FlatFloat_ImageTraining;
    cv::FileStorage Image_Training("img.xml", cv::FileStorage::READ);
    
    if (Image_Training.isOpened() == false)
    {
        std::cout << "error, unable to open training images file, exiting program\n\n";
        return(false);
    }
    
    Image_Training["images"] >> FlatFloat_ImageTraining;
    Image_Training.release();
    
    
    // train
    NearestKs->setDefaultK(1);
    NearestKs->train(FlatFloat_ImageTraining, cv::ml::ROW_SAMPLE, matClassifyIntegers);
    
    return true;
}




///////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------------
std::vector<AvailablePlate> FindCharactersInPlates(std::vector<AvailablePlate> &vectorOfAvailablePlates)
{
    int Plate_Counter = 0;
    cv::Mat ImageContours;
    std::vector<std::vector<cv::Point> > contours;
    cv::RNG rng;
    
    if (vectorOfAvailablePlates.empty())
    {
        return(vectorOfAvailablePlates);
    }
    
    // at this point --->>>can be sure vector of possible plates has at least one plate
    
    for (auto availablePlate = vectorOfAvailablePlates.begin() ; availablePlate != vectorOfAvailablePlates.end() ; ++availablePlate)
    {            // for each possible available plate
        
        Pre_Process(availablePlate->PlateImage, availablePlate->GrayscaleImage, availablePlate->ThresholdImage);
        // Pre_Process to get grayscale and threshold images
  
        
#ifdef SEQUENCE_STEPS
        cv::imshow("5a", availablePlate->PlateImage);
        cv::imshow("5b", availablePlate->GrayscaleImage);
        cv::imshow("5c", availablePlate->ThresholdImage);
#endif	// SEQUENCE_STEPS
        
        // upscaling size by 60% for better viewing and character recognition
        cv::resize(availablePlate->ThresholdImage, availablePlate->ThresholdImage, cv::Size(), 1.6, 1.6);
        
        // thresholding again to eliminate any gray areas
        cv::threshold(availablePlate->ThresholdImage, availablePlate->ThresholdImage, 0.0, 255.0, CV_THRESH_BINARY | CV_THRESH_OTSU);
      
        
#ifdef SEQUENCE_STEPS
        cv::imshow("5d", availablePlate->ThresholdImage);
#endif	// SEQUENCE_STEPS
        
        
        std::vector<AvailableChar> vectorOfAvailableCharactersInPlate = findAvailableCharactersInPlate(availablePlate->GrayscaleImage, availablePlate->ThresholdImage);
        
        
#ifdef SEQUENCE_STEPS
        ImageContours = cv::Mat(availablePlate->ThresholdImage.size(), CV_8UC3, SCALAR_COLOUR_BLACK);
        contours.clear();
        
        for (auto availableChar = vectorOfAvailableCharactersInPlate.begin() ; availableChar != vectorOfAvailableCharactersInPlate.end() ; ++availableChar)
        {
            contours.push_back(availableChar->contour);
        }
        
        cv::drawContours(ImageContours, contours, -1, SCALAR_COLOUR_WHITE);
        
        cv::imshow("6", ImageContours);
#endif	// SEQUENCE_STEPS
        
        
    
        std::vector<std::vector<AvailableChar> > vectorOfVectorsOfMatching_CharactersInPlate = find_VectorOf_VectorsOfMatching_Characters(vectorOfAvailableCharactersInPlate);
        
        
#ifdef SEQUENCE_STEPS
        ImageContours = cv::Mat(availablePlate->ThresholdImage.size(), CV_8UC3, SCALAR_COLOUR_BLACK);
        contours.clear();
        
        for (auto vectorOfMatching_Characters = vectorOfVectorsOfMatching_CharactersInPlate.begin() ; vectorOfMatching_Characters != vectorOfVectorsOfMatching_CharactersInPlate.end() ; ++vectorOfMatching_Characters)
        {
            int RandomBlue = rng.uniform(0, 256);
            int RandomGreen = rng.uniform(0, 256);
            int RandomRed = rng.uniform(0, 256);
            
            for (auto matching_Char =  vectorOfMatching_Characters->begin() ; matching_Char != vectorOfMatching_Characters->end() ; ++matching_Char)
            {
                contours.push_back(matching_Char->contour);
            }
            cv::drawContours(ImageContours, contours, -1, cv::Scalar((double)RandomBlue, (double)RandomGreen, (double)RandomRed));
        }
        cv::imshow("7", ImageContours);
#endif	// SEQUENCE_STEPS
        
        
        if (vectorOfVectorsOfMatching_CharactersInPlate.size() == 0)
        {
            // if no matching Characters
            
#ifdef SEQUENCE_STEPS
            std::cout << "Characters found in plate number " << Plate_Counter << " = (none), click on any image and press a key to continue . . ." << std::endl;
            Plate_Counter++;
            cv::destroyWindow("8");
            cv::destroyWindow("9");
            cv::destroyWindow("10");
            cv::waitKey(0);
#endif	// SEQUENCE_STEPS
            
            availablePlate->strCharacters = "";
            continue;
        }
        
        
        for (auto vectorOfMatching_Characters = vectorOfVectorsOfMatching_CharactersInPlate.begin() ; vectorOfMatching_Characters != vectorOfVectorsOfMatching_CharactersInPlate.end() ; ++vectorOfMatching_Characters)
        {
            std::sort(vectorOfMatching_Characters->begin(), vectorOfMatching_Characters->end(), AvailableChar::sortCharacters_LeftToRight);
            (*vectorOfMatching_Characters )=remove_Inner_Overlapping_Characters(*vectorOfMatching_Characters);
        }
        
        
#ifdef SEQUENCE_STEPS
        ImageContours = cv::Mat(availablePlate->ThresholdImage.size(), CV_8UC3, SCALAR_COLOUR_BLACK);
        
        for (auto vectorOfMatching_Characters = vectorOfVectorsOfMatching_CharactersInPlate.begin() ; vectorOfMatching_Characters != vectorOfVectorsOfMatching_CharactersInPlate.end() ; ++vectorOfMatching_Characters)
        {
            int RandomBlue = rng.uniform(0, 256);
            int RandomGreen = rng.uniform(0, 256);
            int RandomRed = rng.uniform(0, 256);
            
            contours.clear();
            
            for (auto matching_Char = vectorOfMatching_Characters->begin() ; matching_Char != vectorOfMatching_Characters->end(); ++matching_Char)
            {
                contours.push_back(matching_Char->contour);
            }
            cv::drawContours(ImageContours, contours, -1, cv::Scalar((double)RandomBlue, (double)RandomGreen, (double)RandomRed));
        }
        cv::imshow("8", ImageContours);
#endif	// SEQUENCE_STEPS
        
        
        
        unsigned int LengthOfLongest_Vector_OfCharacters = 0;
        unsigned int IndexOfLongest_Vector_OfCharacters = 0;
        
        // get the index of the vector of matching char with the most Characters
        for (unsigned int i = 0; i < vectorOfVectorsOfMatching_CharactersInPlate.size(); i++)
        {
            if (vectorOfVectorsOfMatching_CharactersInPlate[i].size() > LengthOfLongest_Vector_OfCharacters)
            {
                LengthOfLongest_Vector_OfCharacters = vectorOfVectorsOfMatching_CharactersInPlate[i].size();
                
                IndexOfLongest_Vector_OfCharacters = i;
            }
        }
        
        std::vector<AvailableChar> longestVectorOfMatching_CharactersInPlate = vectorOfVectorsOfMatching_CharactersInPlate[IndexOfLongest_Vector_OfCharacters];
        
        
#ifdef SEQUENCE_STEPS
        ImageContours = cv::Mat(availablePlate->ThresholdImage.size(), CV_8UC3, SCALAR_COLOUR_BLACK);
        
        contours.clear();
        
        for (auto matching_Char = longestVectorOfMatching_CharactersInPlate.begin() ; matching_Char != longestVectorOfMatching_CharactersInPlate.end() ; ++matching_Char)
        {
            contours.push_back(matching_Char->contour);
        }
        cv::drawContours(ImageContours, contours, -1, SCALAR_COLOUR_WHITE);
        
        cv::imshow("9", ImageContours);
#endif	// SEQUENCE_STEPS
        
        
        
        availablePlate->strCharacters = recog_CharactersInPlate(availablePlate->ThresholdImage, longestVectorOfMatching_CharactersInPlate);
        
        
#ifdef SEQUENCE_STEPS
        std::cout << "Characters found in plate number " << Plate_Counter << " = " << availablePlate->strCharacters << ", click on any image and press a key to continue . . ." << std::endl;
        Plate_Counter++;
        cv::waitKey(0);
#endif	// SEQUENCE_STEPS
        
        
    }
   
    
    
#ifdef SEQUENCE_STEPS
				std::cout << std::endl << "char detection complete, click on any image and press a key to continue . . ." << std::endl;
				cv::waitKey(0);
#endif	// SEQUENCE_STEPS
    
    
    return(vectorOfAvailablePlates);
}




///////////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------
std::vector<AvailableChar> findAvailableCharactersInPlate(cv::Mat &GrayscaleImage, cv::Mat &ThresholdImage)
{
    
    std::vector<AvailableChar> vectorOfAvailableCharacters;
    cv::Mat ThresholdImageCopy;
    std::vector<std::vector<cv::Point> > contours;
    ThresholdImageCopy = ThresholdImage.clone();
    
    cv::findContours(ThresholdImageCopy, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
    
    
    for (auto contouring = contours.begin() ; contouring != contours.end() ; ++contouring)
    {
        AvailableChar availableChar(*contouring);
        
        if (checkIfAvailableChar(availableChar))
        {                // if contour is a possible char
            vectorOfAvailableCharacters.push_back(availableChar);      // add to vector of possible Characters
        }
    }
    
    return(vectorOfAvailableCharacters);
}




///////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------------
bool checkIfAvailableChar(AvailableChar &availableChar)
{
    
    if (availableChar.RectangleBound.area() > MINIMUM_PIXEL_AREA &&
        availableChar.RectangleBound.width > MINIMUM_PIXEL_WIDTH &&
        availableChar.RectangleBound.height > MINIMUM_PIXEL_HEIGHT &&
        MINIMUM_ASPECT_RATIO < availableChar.AspectRatio &&
        availableChar.AspectRatio < MAXIMUM_ASPECT_RATIO)
    {
        return(true);
    }
    else
    {
        return(false);
    }
}



///////////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------
std::vector<std::vector<AvailableChar> > find_VectorOf_VectorsOfMatching_Characters(const std::vector<AvailableChar> &vectorOfAvailableCharacters)
{
    
    
    
    std::vector<std::vector<AvailableChar> > vectorOfVectorsOfMatching_Characters;
    
    for (auto availableChar = vectorOfAvailableCharacters.begin() ; availableChar != vectorOfAvailableCharacters.end() ; ++availableChar)
    {
        
        // finding all Characters in the big vector that matches current char
        std::vector<AvailableChar> vectorOfMatching_Characters = findVectorOfMatching_Characters(*availableChar, vectorOfAvailableCharacters);
        
        vectorOfMatching_Characters.push_back(*availableChar);
        
        // if the vector is not long enough to constitute a possible plate
        if (vectorOfMatching_Characters.size() < MINIMUM_NUMBER_OF_MATCHING_CHARS)
        {
            continue;
        }
        // here>>>>means, the current vector is a "group" or "cluster" of matching Characters
        vectorOfVectorsOfMatching_Characters.push_back(vectorOfMatching_Characters);
        
        
        // remove the current vector of matching Characters from the big vector so that the same Characters are not used twice
        std::vector<AvailableChar> vectorOfAvailableCharactersWithCurrentMatchesRemoved;
        
        for (auto poss_Char = vectorOfAvailableCharacters.begin() ; poss_Char != vectorOfAvailableCharacters.end() ; ++poss_Char)
        {
            if (std::find(vectorOfMatching_Characters.begin(), vectorOfMatching_Characters.end(), *poss_Char) == vectorOfMatching_Characters.end())
            {
                vectorOfAvailableCharactersWithCurrentMatchesRemoved.push_back(*poss_Char);
            }
        }
        
        std::vector<std::vector<AvailableChar> > recursive_VectorOf_VectorsOfMatching_Characters;
        
        // recursive call
        recursive_VectorOf_VectorsOfMatching_Characters = find_VectorOf_VectorsOfMatching_Characters(vectorOfAvailableCharactersWithCurrentMatchesRemoved);
        
        for (auto recursive_VectorOfMatching_Characters = recursive_VectorOf_VectorsOfMatching_Characters.begin() ; recursive_VectorOfMatching_Characters!= recursive_VectorOf_VectorsOfMatching_Characters.end() ; ++recursive_VectorOfMatching_Characters)
        {
            vectorOfVectorsOfMatching_Characters.push_back(*recursive_VectorOfMatching_Characters);
        }
        
        break;
    }
    
    return(vectorOfVectorsOfMatching_Characters);
}




///////////////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------
std::vector<AvailableChar> findVectorOfMatching_Characters(const AvailableChar &availableChar, const std::vector<AvailableChar> &vectorOfCharacters)
{
    
    std::vector<AvailableChar> vectorOfMatching_Characters;
    
    for (auto Possible_Character_Match = vectorOfCharacters.begin() ; Possible_Character_Match != vectorOfCharacters.end() ; ++Possible_Character_Match )
    {
       
        if ((*Possible_Character_Match) == availableChar)
        {
            continue;
        }
        
        // computing to see if Characters are a match
        double  Distance_Between_Characters = distance_Between_Characters(availableChar, *Possible_Character_Match);
        double  Angle_Between_Characters = angle_Between_Characters(availableChar, *Possible_Character_Match);
        double  ChangeInArea = (double)abs(Possible_Character_Match->RectangleBound.area() - availableChar.RectangleBound.area()) / (double)availableChar.RectangleBound.area();
        double  ChangeInWidth = (double)abs(Possible_Character_Match->RectangleBound.width - availableChar.RectangleBound.width) / (double)availableChar.RectangleBound.width;
        double  ChangeInHeight = (double)abs(Possible_Character_Match->RectangleBound.height - availableChar.RectangleBound.height) / (double)availableChar.RectangleBound.height;
        
        // check if Characters match
        if ( Distance_Between_Characters < (availableChar.DiagonalSize * MAXIMUM_DIAGONAL_SIZE_MULTIPLE_AWAY) &&
            Angle_Between_Characters < MAXIMUM_ANGLE_BETWEEN_CHARS &&
            ChangeInArea < MAXIMUM_CHANGE_IN_AREA &&
            ChangeInWidth < MAXIMUM_CHANGE_IN_WIDTH &&
            ChangeInHeight < MAXIMUM_CHANGE_IN_HEIGHT)
        {
            vectorOfMatching_Characters.push_back(*Possible_Character_Match);
        }
    }
    
    return(vectorOfMatching_Characters);
}





///////////////////////////////////////////////////////////////////////////////////////////////////
double distance_Between_Characters(const AvailableChar &firstChar, const AvailableChar &secondChar)
{
    int X = abs(firstChar.CenterX - secondChar.CenterX);
    int Y = abs(firstChar.CenterY - secondChar.CenterY);
    
    return(sqrt(pow(X, 2.0) + pow(Y, 2.0)));
}




///////////////////////////////////////////////////////////////////////////////////////////////////
double angle_Between_Characters(const AvailableChar &firstChar, const AvailableChar &secondChar)
{
    double  Adj = abs(firstChar.CenterX - secondChar.CenterX);
    double  Opp = abs(firstChar.CenterY - secondChar.CenterY);
    
    double  AngleInRad = atan( Opp /  Adj);
    
    double  AngleInDeg =  AngleInRad * (180.0 / CV_PI);
    
    return( AngleInDeg);
}





///////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<AvailableChar> remove_Inner_Overlapping_Characters(std::vector<AvailableChar> &vectorOfMatching_Characters)
{
    std::vector<AvailableChar> vectorOfMatching_CharactersWithInnerCharRemoved(vectorOfMatching_Characters);
    
    for (auto currentChar = vectorOfMatching_Characters.begin(); currentChar != vectorOfMatching_Characters.end() ; ++currentChar )
    {
        for (auto otherChar = vectorOfMatching_Characters.begin() ; otherChar != vectorOfMatching_Characters.end() ; ++otherChar)
        {
            if ((*currentChar) != (*otherChar) )
            {
                if (distance_Between_Characters(*currentChar, *otherChar) < (currentChar->DiagonalSize * MINIMUM_DIAGONAL_SIZE_MULTIPLE_AWAY))
                {
                    // overlapping Characters are found
                    if (currentChar->RectangleBound.area() < otherChar->RectangleBound.area())
                        // if current char is smaller than other char
                    {
                        
                        std::vector<AvailableChar>::iterator currentCharIterator = std::find(vectorOfMatching_CharactersWithInnerCharRemoved.begin(), vectorOfMatching_CharactersWithInnerCharRemoved.end(), *currentChar);
                        
                        
                        if (currentCharIterator != vectorOfMatching_CharactersWithInnerCharRemoved.end())
                        {
                            vectorOfMatching_CharactersWithInnerCharRemoved.erase(currentCharIterator);       //remove the char
                        }
                    }
                    else
                    {
                        std::vector<AvailableChar>::iterator otherCharIterator = std::find(vectorOfMatching_CharactersWithInnerCharRemoved.begin(), vectorOfMatching_CharactersWithInnerCharRemoved.end(), *otherChar);
                        // if iterator did not get to end, then the char was found in the vector
                        if (otherCharIterator != vectorOfMatching_CharactersWithInnerCharRemoved.end())
                        {
                            vectorOfMatching_CharactersWithInnerCharRemoved.erase(otherCharIterator);         //remove the char
                        }
                    }
                }
            }
        }
    }
    
    return(vectorOfMatching_CharactersWithInnerCharRemoved);
}




///////////////////////////////////////////////////////////////////////////////////////////////////
std::string recog_CharactersInPlate(cv::Mat &ThresholdImage, std::vector<AvailableChar> &vectorOfMatching_Characters)
{
    std::string strCharacters;
    
    cv::Mat ThresholdImageColor;
    
    // sort Characters from left to right
    std::sort(vectorOfMatching_Characters.begin(), vectorOfMatching_Characters.end(), AvailableChar::sortCharacters_LeftToRight);
    
    cv::cvtColor(ThresholdImage, ThresholdImageColor, CV_GRAY2BGR);       // color version of threshold image
    
    for (auto currentChar = vectorOfMatching_Characters.begin() ; currentChar != vectorOfMatching_Characters.end() ; ++currentChar)
    {
        cv::rectangle(ThresholdImageColor, currentChar->RectangleBound, SCALAR_COLOUR_GREEN, 2);
        cv::Mat  ROItoBeCloned = ThresholdImage(currentChar->RectangleBound);
        cv::Mat  ROI =  ROItoBeCloned.clone();
        cv::Mat  ROIResized;
        // resize image >>>>> for char recognition
        cv::resize( ROI,  ROIResized, cv::Size(RESIZED_CHARACTER_IMAGE_WIDTH, RESIZED_CHARACTER_IMAGE_HEIGHT));
        
        cv::Mat ROIFloat;
        ROIResized.convertTo(ROIFloat, CV_32FC1);			// convert Mat to float>>>>>>>>> call to findNearest
        cv::Mat ROIFlattenedFloat = ROIFloat.reshape(1, 1); // flatten Matrix into one row
        cv::Mat CurrentChar(0, 0, CV_32F);                  // declare Mat to read current char >>>> b/c findNearest requires a Mat
        
        NearestKs->findNearest(ROIFlattenedFloat, 1, CurrentChar);
        float fltCurrentChar = (float)CurrentChar.at<float>(0, 0);
        strCharacters = strCharacters + char(int(fltCurrentChar));
    }
    
#ifdef SEQUENCE_STEPS
				cv::imshow("10", ThresholdImageColor);
#endif	// SEQUENCE_STEPS
    
    return(strCharacters);              
}