// Facemojis.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <opencv2\core\core.hpp>
#include <opencv2\opencv.hpp>
#include "opencv2/objdetect/objdetect.hpp"
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\video\video.hpp>

#include <iostream>
#include <string>
#include <vector>

cv::Mat nextInput;
cv::Mat gray;
std::string face_cascade_name = "haarcascade_frontalface_alt.xml";
std::string eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";
cv::CascadeClassifier face_cascade;
cv::CascadeClassifier eyes_cascade;

//cv::RNG rng(12345);

void detectAndDisplay(cv::Mat frame)
{
	std::vector<cv::Rect> faces;
	cv::Mat frame_gray;

	cv::cvtColor(frame, frame_gray, CV_BGR2GRAY);
	cv::equalizeHist(frame_gray, frame_gray);
	//Detect Faces
	face_cascade.detectMultiScale(frame_gray, faces);
	cv::imshow("input", frame_gray);
	cv::waitKey();

	//for (size_t i = 0; i < faces.size(); i++)
	//{
	//	cv::Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
	//	ellipse(frame, center, cv::Size(faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, cv::Scalar(255, 0, 255), 4, 8, 0);

	//	cv::Mat faceROI = frame_gray(faces[i]);
	//	std::vector<cv::Rect> eyes;

	//	//-- In each face, detect eyes
	//	eyes_cascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));

	//	for (size_t j = 0; j < eyes.size(); j++)
	//	{
	//		cv::Point center(faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5);
	//		int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
	//		circle(frame, center, radius, cv::Scalar(255, 0, 0), 4, 8, 0);
	//	}
	//}

}

int video(char* videoname) {

	cv::VideoCapture cap;
	
	if (!face_cascade.load(face_cascade_name)) { printf("--(!)Error loading face \n"); return -1; };
	if (!eyes_cascade.load(eyes_cascade_name)) { printf("--(!)Error loading eye \n"); return -1; };

	cap = cv::VideoCapture(1); //Camera frontale par défault de ma machine
	if (!cap.isOpened())
		cap = cv::VideoCapture(0);
	
	if (!cap.isOpened())  // check if we succeeded
		return -1;

	//recuperer une image depuis cap et la stocker dans nextInput
	cap >> nextInput;
	//tant que nextinput n’est pas vide
	while (!nextInput.empty())
	{
		// - > faire les traitements sur l’image (prochaines étapes)
		//detectAndDisplay(nextInput);

		// - > appeler la fonction de dessin
		cv::imshow("input", nextInput);
		// - > recuperer une nouvelle image et la stocker dans nextInput
		cap >> nextInput;
		// - > attendre 10ms que l’utilisateur tape une touche, et quitter si il le fait
		if (cv::waitKey(10) >= 0) break;
	}
	return 0;
}

int main()
{
	int r = video(NULL);

	system("PAUSE");
	return r;
}

