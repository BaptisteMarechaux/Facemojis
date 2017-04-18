// Facemojis.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <opencv2\core\core.hpp>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\video\video.hpp>

cv::Mat nextInput;

int video(char* videoname) {

	cv::VideoCapture cap;

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
	return r;
}

