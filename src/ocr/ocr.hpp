#include  "opencv2/text.hpp"
#include  "opencv2/highgui.hpp"
#include  "opencv2/imgproc.hpp"
#include "opencv2/core/utility.hpp"
#include  <vector>
#include  <iostream>
#include  <iomanip>
#include "opencv/cv.h"
#include "../util/decoded_text.hpp"
#include "../util/output_ocr.hpp"

using namespace std;
using namespace cv;
using namespace cv::text;

OutputOCR Ocr(std::string);
OutputOCR Ocr(std::string, Rect box);
