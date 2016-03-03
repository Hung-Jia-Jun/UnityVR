/*#include <opencv/highgui.h>
#include "camerads.h"
int main()
{
CvCapture * cap = cvCreateCameraCapture(0);
IplImage * img;//=cvLoadImage("a.png");
cvNamedWindow("a");

while (1) {
img = cvQueryFrame(cap);
cvShowImage("a", img);
cvWaitKey(33);
}
return 0;
}*/
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include<iostream>
#include<fstream>
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
	Mat imgTmp;
	cap.read(imgTmp);
	Mat imgLines = Mat::zeros(imgTmp.size(), CV_8UC3);;
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
	/*要寫入文字檔的座標*/
	int iLastX = -1;
	int iLastY = -1;

	int i_Yellow_LastX = -1;
	int i_Yellow_LastY = -1;

	int i_Green_LastX = -1;
	int i_Green_LastY = -1;
	/*要寫入文字檔的座標*/
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


		inRange(imgHSV, Scalar(/*低色相*/iLowBlue,  /*低彩度*/50, /*低亮度*/VLowBlue), Scalar(/*高色相*/iHighBlue, /*高彩度*/255, VHighBlue), imgThresholded); //Blue
		inRange(imgHSV, Scalar(/*低色相*/iLowYellow,/*低彩度*/50, /*低亮度*/VLowYellow), Scalar(/*高色相*/iHighYellow, /*高彩度*/255, VHighYellow), YellowimgThresholded); //Yellow
		inRange(imgHSV, Scalar(/*低色相*/iLowGreen, /*低彩度*/50, /*低亮度*/VLowGreen), Scalar(/*高色相*/iHighGreen, /*高彩度*/255, VHighGreen), GreenimgThresholded); //Green
																																						/**************************************************************************************************************/
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		//morphological closing (fill small holes in the foreground)
		dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));//對顯示的物件侵蝕
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));//對顯示的物件膨脹

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


		Moments oMoments = moments(imgThresholded);//藍色光球的視窗


		double dM01 = oMoments.m01;
		double dM10 = oMoments.m10;
		double dArea = oMoments.m00;

		Moments oMoments1 = moments(YellowimgThresholded);//黃色光球的視窗
		double dM02 = oMoments1.m01;
		double dM20 = oMoments1.m10;
		double dArea2 = oMoments1.m00;


		Moments oMoments2 = moments(GreenimgThresholded);//綠色光球的視窗
		double dM03 = oMoments2.m01;//用來表示圖像階矩  01 10 為一階矩
		double dM30 = oMoments2.m10;
		double dArea3 = oMoments2.m00;//零階矩

		// if the area <= 10000, I consider that the there are no object in the image and it's because of the noise, the area is not zero 
		if (dArea > 10000)
		{
			//calculate the position of the ball
			int posX = dM10 / dArea;
			int posY = dM01 / dArea;

			int YellowposX = dM20 / dArea2;
			int YellowposY = dM02 / dArea2;

			int GreenposX = dM30 / dArea3;
			int GreenposY = dM03 / dArea3;

			if (iLastX >= 0 && iLastY >= 0 && posX >= 0 && posY >= 0)
			{
				
				//line(imgLines, /*起始點*/Point(posX, posY),/*終點*/ Point(iLastX, iLastY), /*線色*/Scalar(0, 0, 255), /*線寬*/2);//打開這行可以在螢幕上畫線
				/************寫入藍色光球文字檔**********/
				char filenameX[] = "E:/BlueX.txt";//開啟藍色光球的Xtxt
				char filenameY[] = "E:/BlueY.txt";//開啟藍色光球的Ytxt

				fstream fpX;
				fpX.open(filenameX, ios::out);//開啟檔案
				fpX << posX << endl;//寫入藍色光球的X軸
				fstream fpY;
				fpY.open(filenameY, ios::out);//開啟檔案
				fpY << posY << endl;//寫入藍色光球的Y軸
				/************寫入藍色光球文字檔**********/
			}

			iLastX = posX;//要讓他在螢幕上面畫線的時候才要開啟
			iLastY = posY;//要讓他在螢幕上面畫線的時候才要開啟



			if (i_Yellow_LastX >= 0 && i_Yellow_LastY >= 0 && YellowposX >= 0 && YellowposY >= 0)
			{
				/************寫入黃色光球文字檔**********/
				char filename_Yellow_X[] = "E:/X.txt";//開啟黃色光球的Xtxt
				char filename_Yellow_Y[] = "E:/Y.txt";//開啟黃色光球的Ytxt

				fstream fp_Yellow_X;
				fp_Yellow_X.open(filename_Yellow_X, ios::out);//開啟檔案
				fp_Yellow_X << YellowposX << endl;//寫入黃色光球的X軸
				fstream fp_Yellow_Y;
				fp_Yellow_Y.open(filename_Yellow_Y, ios::out);//開啟檔案
				fp_Yellow_Y << YellowposY << endl;//寫入黃色光球的Y軸
										  /************寫入黃色光球文字檔**********/
			}
			i_Yellow_LastX = YellowposX;
			i_Yellow_LastY = YellowposY;

			if (i_Green_LastX >= 0 && i_Green_LastY >= 0 &&GreenposX >= 0 && GreenposY >= 0)
			{
				/************寫入綠色光球文字檔**********/
				char filename_Green_X[] = "E:/GreenX.txt";//開啟綠色光球的Xtxt
				char filename_Green_Y[] = "E:/GreenY.txt";//開啟綠色光球的Ytxt

				fstream fp_Green_X;
				fp_Green_X.open(filename_Green_X, ios::out);//開啟檔案
				fp_Green_X << GreenposX << endl;//寫入綠色光球的X軸
				fstream fp_Green_Y;
				fp_Green_Y.open(filename_Green_Y, ios::out);//開啟檔案
				fp_Green_Y << GreenposY << endl;//寫入綠色光球的Y軸
										 /************寫入綠色光球文字檔**********/
			}
			i_Green_LastX = GreenposX;
			i_Green_LastY = GreenposY;
		}



		imshow("Blue", imgThresholded); //show the thresholded image
		imshow("Yellow", YellowimgThresholded); //show the thresholded image
		imshow("Green", GreenimgThresholded); //show the thresholded image
		imgOriginal = imgOriginal + imgLines;
		imshow("Original", imgOriginal); //show the original image
		if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}

	return 0;
	
}
