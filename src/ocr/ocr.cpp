/* textdetection.cpp
 *
 * A demo program of the Extremal Region Filter algorithm described in
 * Neumann L., Matas J.: Real-Time Scene Text Localization and Recognition, CVPR 2012
 *
 * Created on: Sep 23, 2013
 *     Author: Lluis Gomez i Bigorda <lgomez AT cvc.uab.es>
 */

#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include  "opencv2/text.hpp"
#include  "opencv2/highgui.hpp"
#include  "opencv2/imgproc.hpp"
#include "opencv2/core/utility.hpp"
#include  <vector>
#include  <iostream>
#include  <iomanip>
#include "opencv/cv.h"
#include "../util/output_ocr.hpp"
#include "../util/decoded_text.hpp"
#include "../util/box.hpp"

using namespace std;
using namespace cv;
using namespace cv::text;

stringstream nullout;

#if defined DEBUG 
#define LOG cout
#else
#define LOG nullout
#endif

class Regions{
    public:
        vector<vector<ERStat> > regions;
        vector<Rect> groups_boxes;
        vector< vector<Vec2i> > region_groups;
};

class decodedTxtRegion{
    public:
        int x1,x2,y1,y2;
        vector<string> words;
        vector<float> confidences;
};

class outputOCR{
    public:
        int x1,x2,y1,y2;
        vector<decodedTxtRegion> decodedText;
    
};

Rect groups_draw(Mat &src, vector<Rect> &groups);

void er_show(vector<Mat> &channels, vector<vector<ERStat> > &regions);

void er_draw(vector<Mat> &channels, vector<vector<ERStat> > &regions, vector<Vec2i> group, Mat& segmentation);

vector<Rect> computeGroupsWithMinArea(Mat &src,vector<Mat> &channels,float minArea);

vector<DecodedText> doOCR(string languageFile, Mat &image,vector<Mat> channels,vector<vector<ERStat> > regions,vector< vector<Vec2i> > nm_region_groups,vector<Rect> nm_boxes);

Regions computeRegionGroups(Mat &src,vector<Mat> &channels,float minArea);

OutputOCR detectAndDecode(string languageFile, Mat &src);


outputOCR Ocr(string path,Rect box) {
    // namedWindow("grouping",WINDOW_NORMAL);
    // read the image
    Mat src = imread(path);
    // clone the image
    Mat clonesrc = src.clone();
    // crop the image
    Mat croppedImage = clonesrc(Rect(box.tl().x, box.tl().y, (box.br().x - box.tl().x), (box.br().y - box.tl().y)));
    // convert image to gray
    cvtColor(croppedImage, croppedImage, COLOR_RGB2GRAY);
    
    Mat dst = Mat::zeros(croppedImage.rows+2, croppedImage.cols+2, CV_8UC1);
    
    int thresh= (croppedImage.rows * 2)/10 + 1;
    // do adaptive threshholding
    adaptiveThreshold(croppedImage, croppedImage, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY,thresh , -10);
    // imwrite("/Users/prjha/Documents/git/pranavjha/_scene-text-detector/samples/_3.jpg",src);
    vector<Rect>   boxes;
    vector<string> words;
    vector<float>  confidences;
    
    // run ocr
    string op;
    Ptr<OCRTesseract> ocr = OCRTesseract::create();
    ocr->run(croppedImage, op, &boxes, &words, &confidences, OCR_LEVEL_WORD);
    LOG << "OCR output = " << op <<  " length = " << op.size() << endl;
    vector<decodedTxtRegion> decodedTxtRegions;

    decodedTxtRegion decodedTxt;
    decodedTxt.x1 = box.tl().x;
    decodedTxt.x2 = box.br().x;
    decodedTxt.y1 = box.tl().y;
    decodedTxt.y2 = box.br().y;
    
    decodedTxt.words = words;
    decodedTxt.confidences = confidences;
    decodedTxtRegions.push_back(decodedTxt);
    outputOCR output;
    output.decodedText=decodedTxtRegions;
    return output;
}

OutputOCR* Ocr(char* buffer, unsigned int len, string languageFile)
{
    Mat temp(1, len, CV_8UC3, buffer);
    Mat src = imdecode(temp, 1);
    if ( src.data == NULL )   
    {
        LOG << "Unable to load image." << endl;
        return NULL;
    }
    return new OutputOCR(detectAndDecode(languageFile, src));
}

OutputOCR* Ocr (string path, string languageFile) {
    // namedWindow("grouping",WINDOW_NORMAL);
    //Mat src = imread(path);
    char* buffer = NULL;
    unsigned int len = 0;
    ifstream f;     
    f.open(path.c_str(), std::ios::binary);
    if(!f.is_open())  
    {
        LOG << "lol" << endl;
    }  
    else  
    { 
        f.seekg(0, std::ios::end);
        len = f.tellg(); 
        f.seekg(0, std::ios::beg);

        buffer = new char[len];    
        f.read(buffer, len);            

        f.close();
    }
    
    Mat temp(1, len, CV_8UC3, buffer);
    Mat src = imdecode(temp, 1);
    if ( src.data == NULL )   
    {
        LOG << "Unable to decode image." << endl;
        return NULL;
    }
    LOG << src.type() << endl;
    LOG << CV_8UC3 << endl;
    LOG << "image loaded" << endl;
    return new OutputOCR(detectAndDecode(languageFile, src));
}

OutputOCR detectAndDecode(string languageFile, Mat &src){
    
    // Extract channels to be processed individually
    vector<Mat> channels;
    computeNMChannels(src, channels);
    
    int cn = (int)channels.size();
    
    // TODO: how did you get this?
    int minArea= 0.0080f;
    
    // Append negative channels to detect ER- (bright regions over dark background)
    for (int c = 0; c < cn-1; c++) {
        channels.push_back(255-channels[c]);
    }
    
    vector<Rect> groups_boxes;
    Regions region=computeRegionGroups(src,channels,minArea);
    groups_boxes=region.groups_boxes;
    if(groups_boxes.size()==0){
        groups_boxes=computeGroupsWithMinArea(src,channels,minArea);
    }
    LOG <<"received boxes: "<<groups_boxes.size()<<endl;
    
    // draw groups
   	Rect group_box= groups_draw(src, groups_boxes);
    imwrite("save.jpg",src);
    vector<DecodedText> decodedTxt=doOCR(languageFile, src,channels,region.regions,region.region_groups,groups_boxes);
   
 
    OutputOCR output(Box(group_box.tl().x, group_box.br().x, group_box.tl().y, group_box.br().y), decodedTxt);

    // imshow("grouping",src);
    // memory clean-up
    if (!groups_boxes.empty()) {
        groups_boxes.clear();
    }
    return output;
}
vector<DecodedText> doOCR(string languageFile, Mat &image,vector<Mat> channels,vector<vector<ERStat> > regions,vector< vector<Vec2i> > nm_region_groups,vector<Rect> nm_boxes){
    // Text Recognition (OCR)
    double t_r = (double)getTickCount();
   
    Ptr<OCRTesseract> ocr = OCRTesseract::create(languageFile.c_str());
    string output;
    Mat out_img;
    Mat out_img_detection;
    Mat out_img_segmentation = Mat::zeros(image.rows+2, image.cols+2, CV_8UC1);
    image.copyTo(out_img);
    image.copyTo(out_img_detection);
    float scale_img  = 600.f/image.rows;
    float scale_font = (float)(2-scale_img)/1.4f;
    vector<string> words_detection;
    
    t_r = (double)getTickCount();
    
    vector<DecodedText> decodedTxtRegions;
    for (int i=0; i<(int)nm_boxes.size(); i++)
    {
        // rectangle(out_img_detection, nm_boxes[i].tl(), nm_boxes[i].br(), Scalar(0,255,255), 3);
        
        Mat group_img = Mat::zeros(image.rows+2, image.cols+2, CV_8UC1);
        er_draw(channels, regions, nm_region_groups[i], group_img);
                Mat group_segmentation;
        group_img.copyTo(group_segmentation);
        //image(nm_boxes[i]).copyTo(group_img);
        group_img(nm_boxes[i]).copyTo(group_img);
        
        copyMakeBorder(group_img,group_img,15,15,15,15,BORDER_CONSTANT,Scalar(0));
        
        vector<Rect>   boxes;
        vector<string> words;
        vector<float>  confidences;
        ocr->run(group_img, output, &boxes, &words, &confidences, OCR_LEVEL_WORD);
        
        output.erase(remove(output.begin(), output.end(), '\n'), output.end());
        LOG << "OCR output = \"" << output << "\" length = " << output.size() << endl;
        
        decodedTxtRegions.push_back(DecodedText(Box(nm_boxes[i].tl().x, nm_boxes[i].br().x, nm_boxes[i].tl().y, nm_boxes[i].br().y), words, confidences));
    }  
    return decodedTxtRegions;
}
vector<Rect> computeGroupsWithMinArea(Mat &src,vector<Mat> &channels,float minArea){
    Regions region= computeRegionGroups(src,channels,minArea);
    vector<Rect> groups_boxes=region.groups_boxes;
    if(groups_boxes.size()==0 && minArea-0.00075 > 0){
        return computeGroupsWithMinArea(src,channels,minArea-0.00075);
    }
    else if(groups_boxes.size()==0 && minArea-0.00015 >0) {
        return computeGroupsWithMinArea(src,channels,minArea-0.00075);
    }
    else{
        return groups_boxes;
    }
}
Regions computeRegionGroups(Mat &src,vector<Mat> &channels,float minArea){
    // Create ERFilter objects with the 1st and 2nd stage default classifiers
    //FIXME
    Ptr<ERFilter> er_filter1 = createERFilterNM1(loadClassifierNM1("Resources/trained_classifierNM1.xml"),100,minArea,0.5f,0.8f,true,0.02f);
    Ptr<ERFilter> er_filter2 = createERFilterNM2(loadClassifierNM2("Resources/trained_classifierNM2.xml"),0.01);
    vector<vector<ERStat> > regions(channels.size());
    // Apply the default cascade classifier to each independent channel (could be done in parallel)
    for (int c=0; c<(int)channels.size(); c++)
    {
        er_filter1->run(channels[c], regions[c]);
        er_filter2->run(channels[c], regions[c]);
    }
    
    // Detect character groups
    vector< vector<Vec2i> > region_groups;
    vector<Rect> groups_boxes;
    erGrouping(src, channels, regions, region_groups, groups_boxes, ERGROUPING_ORIENTATION_HORIZ);
    Regions region;
    region.regions=regions;
    region.groups_boxes=groups_boxes;
    region.region_groups=region_groups;
    
    return region;
}

// helper funtions
/*Rect groups_draw(Mat &src, vector<Rect> &groups)
{
    Point tl,br;
    for ( int i=0; i<(int )groups.size(); i++)
    {
        if(i==0){
            tl=groups.at(i).tl();
            br=groups.at(i).br();
        }
        if(groups.at(i).tl().x<tl.x){
            tl.x=groups.at(i).tl().x;
        }
        if(groups.at(i).br().x>br.x){
            br.x=groups.at(i).br().x;
        }
        if(groups.at(i).tl().y<tl.y){
            tl.y=groups.at(i).tl().y;
        }
        if(groups.at(i).br().y>br.y){
            br.y=groups.at(i).br().y;
        }
    }
    tl.x= tl.x-3 >=0 ? tl.x-3 : tl.x;
   tl.y= tl.y-3 >=0 ? tl.y-3 : tl.y;
    br.x= br.x+3 < src.size().width ? br.x+3 : br.x;
    br.y = br.y + 3 < src.size().height? br.y+3:br.y;
    if(groups.size()==0){
        tl.x=0;
        tl.y=0;
        br.x=src.size().width;
        br.y=src.size().height;
    }
    rectangle(src,tl,br,Scalar( 0, 255, 255 ), 3, 8 );
    Rect rect(tl,br);
    return rect;

}*/

Rect groups_draw(Mat &src, vector<Rect> &groups)
{
    for (int i=(int)groups.size()-1; i>=0; i--)
    {
        if (src.type() == CV_8UC3)
            rectangle(src,groups.at(i).tl(),groups.at(i).br(),Scalar( 0, 255, 255 ), 3, 8 );
        else
            rectangle(src,groups.at(i).tl(),groups.at(i).br(),Scalar( 255 ), 3, 8 );
    }
    Rect rect(groups.at(0).tl(),groups.at(0).br());
    return rect;
}

void er_show(vector<Mat> &channels, vector<vector<ERStat> > &regions)
{
    for (int c=0; c<(int)channels.size(); c++)
    {
        Mat dst = Mat::zeros(channels[0].rows+2,channels[0].cols+2,CV_8UC1);
        for (int r=0; r<(int)regions[c].size(); r++)
        {
            ERStat er = regions[c][r];
            if (er.parent != NULL) // deprecate the root region
            {
                int newMaskVal = 255;
                int flags = 4 + (newMaskVal << 8) + FLOODFILL_FIXED_RANGE + FLOODFILL_MASK_ONLY;
                floodFill(channels[c],dst,Point(er.pixel%channels[c].cols,er.pixel/channels[c].cols),
                          Scalar(255),0,Scalar(er.level),Scalar(0),flags);
            }
        }
        char buff[10]; char *buff_ptr = buff;
        sprintf(buff, "channel %d", c);
        imshow(buff_ptr, dst);
    }
    waitKey(-1);
}
void er_draw(vector<Mat> &channels, vector<vector<ERStat> > &regions, vector<Vec2i> group, Mat& segmentation) {
    for (int r=0; r<(int)group.size(); r++) {
        ERStat er = regions[group[r][0]][group[r][1]];
        if (er.parent != NULL) {
            floodFill(
                      channels[group[r][0]],
                      segmentation,Point(er.pixel%channels[group[r][0]].cols,er.pixel/channels[group[r][0]].cols),
                      Scalar(255),
                      0,
                      Scalar(er.level),
                      Scalar(0),
                      (4 + (255 << 8) + FLOODFILL_FIXED_RANGE + FLOODFILL_MASK_ONLY)
                      );
        }
    }
}
//int main(){
//    string path="/Users/prjha/Documents/git/pranavjha/_scene-text-detector/samples/012.jpg";
//    
//    Point tl,br;
//    tl.x=0;tl.y=0;
//    br.x=0;br.y=0;
//    Rect box(tl,br);
//    outputOCR op= Ocr(path, box);
//
//}
