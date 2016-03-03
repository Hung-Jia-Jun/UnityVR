#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	VideoCapture cap(0); //capture the video from web cam


	if (!cap.isOpened())  // if not success, exit program
	{
		cout << "Cannot open the web cam" << endl;
		return -1;
	}

	namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"
	int iLowGreen = 84;
	int iHighGreen = 101;

	int VLowGreen = 203;
	int VHighGreen = 255;

	int iLowYellow = 26;
	int iHighYellow = 36;

	int VLowYellow = 168;
	int VHighYellow = 255;

	int iLowBlue = 93;
	int iHighBlue = 103;

	int VLowBlue = 217;
	int VHighBlue = 255;

	//Create trackbars in "Control" window
	cvCreateTrackbar("綠色低色相", "Control", &iLowGreen, 255); //Green (0 - 179)
	cvCreateTrackbar("綠色高色相", "Control", &iHighGreen, 179);

	cvCreateTrackbar("綠色低亮度", "Control", &VLowGreen, 255); //Green (0 - 179)
	cvCreateTrackbar("綠色高亮度", "Control", &VHighGreen, 179);

	cvCreateTrackbar("黃色低色相", "Control", &iLowYellow, 255); //Yellow (0 - 255)
	cvCreateTrackbar("黃色高色相", "Control", &iHighYellow, 255);

	cvCreateTrackbar("黃色低亮度", "Control", &VLowYellow, 255); //Yellow (0 - 255)
	cvCreateTrackbar("黃色高亮度", "Control", &VHighYellow, 255);

	cvCreateTrackbar("藍色低色相", "Control", &iLowBlue, 255); //Blue (0 - 255)
	cvCreateTrackbar("藍色高色相", "Control", &iHighBlue, 255);

	cvCreateTrackbar("藍色低亮度", "Control", &VLowBlue, 255); //Blue (0 - 255)
	cvCreateTrackbar("藍色高亮度", "Control", &VHighBlue, 255);
	while (true)
	{
		Mat imgOriginal;

		bool bSuccess = cap.read(imgOriginal); // read a new frame from video
		if (!bSuccess) //if not success, break loop
		{
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}

		Mat imgHSV;

		cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

		Mat imgThresholded;
		Mat YellowimgThresholded;
		Mat GreenimgThresholded;

		inRange(imgHSV, Scalar(/*低色相*/iLowBlue,  /*低彩度*/50, /*低亮度*/VLowBlue),   Scalar(/*高色相*/iHighBlue, /*高彩度*/255, VHighBlue), imgThresholded); //Blue
		inRange(imgHSV, Scalar(/*低色相*/iLowYellow,/*低彩度*/50, /*低亮度*/VLowYellow), Scalar(/*高色相*/iHighYellow, /*高彩度*/255, VHighYellow), YellowimgThresholded); //Yellow
		inRange(imgHSV, Scalar(/*低色相*/iLowGreen, /*低彩度*/50, /*低亮度*/VLowGreen),  Scalar(/*高色相*/iHighGreen, /*高彩度*/255, VHighGreen), GreenimgThresholded); //Green

																																				   /**************************************************************************************************************/
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		//morphological closing (fill small holes in the foreground)
		dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		/**************************************************************************************************************/

		erode(YellowimgThresholded, YellowimgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(YellowimgThresholded, YellowimgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		//morphological closing (fill small holes in the foreground)
		dilate(YellowimgThresholded, YellowimgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(YellowimgThresholded, YellowimgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		/*****************************************************************************************************************/

		/**************************************************************************************************************/

		erode(GreenimgThresholded, GreenimgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(GreenimgThresholded, GreenimgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		//morphological closing (fill small holes in the foreground)
		dilate(GreenimgThresholded, GreenimgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(GreenimgThresholded, GreenimgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		/*****************************************************************************************************************/

		imshow("Blue", imgThresholded); //show the thresholded image
		imshow("Yellow", YellowimgThresholded); //show the thresholded image
		imshow("Green", GreenimgThresholded); //show the thresholded image


		if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}

	return 0;

}
