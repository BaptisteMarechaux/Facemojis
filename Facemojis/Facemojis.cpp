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

const cv::String NEUTRAL_PATH = "./Emoji/neutral.png";
const cv::String SMILE_PATH = "./Emoji/smile.png";
const cv::String WINK_PATH = "./Emoji/wink.png";
const cv::String SAD_PATH = "./Emoji/sad.png";

cv::Mat nextInput;
cv::Mat gray;
std::string face_cascade_name = "haarcascade_frontalface_alt2.xml";
std::string eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";
std::string smile_cascade_name = "haarcascade_smile.xml";
//std::string mouth_cascade_name = "haarcascade_mcs_mouth.xml";
cv::CascadeClassifier face_cascade;
cv::CascadeClassifier eyes_cascade;
cv::CascadeClassifier smile_cascade;
//cv::CascadeClassifier mouth_cascade;


/// <summary>
/// <para>L'image à utiliser.</para>
/// </summary>
enum Mode
{
	NONE , /// Pas d'image
	NEUTRAL , /// Image de base
	SMILE , /// Image avec un sourire
	WINK , /// Image avec cli d'oeil
	SAD /// Image avec sad
};

/// <summary>
/// <para>Pour placer une image en surimpression de la vidéo.</para>
/// <param name="x">Le centre du cercle à remplir avec une image.</param>
/// <param name="y">Le centre du cercle à remplir avec une image.</param>
/// <param name="angle">L'angle de rotation de l'image à incorporer.</param>
/// <param name="radius">Le scale de l'image à effectuer.</param>
/// <param name="mode">L'image à utiliser.</param>
/// </summary>
void placePicture ( int x , int y , float radius , Mode mode )
{
	// Si pas d'image à mettre en superposition osef !
	if ( mode == NONE )
		return;

	// trouver le path nécessaire
	cv::String pathtoload;
	switch ( mode )
	{
		case SMILE:
			pathtoload = SMILE_PATH;
			break;
		case WINK:
			pathtoload = WINK_PATH;
			break;
		case SAD:
			pathtoload = SAD_PATH;
			break;
		case NEUTRAL:
		default:
			pathtoload = NEUTRAL_PATH;
			break;
	}

	// 1 load l'image nécessaire
	cv::Mat img = cv::imread ( pathtoload , cv::IMREAD_UNCHANGED ) , imgresize , imgrotate;
	// 2 resize l'image
	float percent = radius / img.size ( ).height;
	//imgresize = cv::Mat ( radius , radius , cv::IMREAD_UNCHANGED );
	cv::resize ( img , imgresize , cv::Size ( ) , percent , percent );

	int xpos , ypos;
	int xmin = x - imgresize.size ( ).width / 2;
	int xmax = x + imgresize.size ( ).width / 2;
	if ( xmin < 0 )
	{
		xpos = 0;
	}
	else if ( xmax > nextInput.size ( ).width )
	{
		xpos = nextInput.size ( ).width - imgresize.size ( ).width;
	}
	else
	{
		xpos = xmin;
	}

	int ymin = y - imgresize.size ( ).height / 2;
	int ymax = y + imgresize.size ( ).height / 2;
	if ( ymin < 0 )
	{
		ypos = 0;
	}
	else if ( ymax > nextInput.size ( ).height )
	{
		ypos = nextInput.size ( ).height - imgresize.size ( ).height;
	}
	else
	{
		ypos = ymin;
	}

	// 4 appliquer l'image
	cv::Rect roi ( cv::Point ( xpos , ypos ) , imgresize.size ( ) );
	cv::Mat4b temp = imgresize;
	cv::Mat3b dst = nextInput ( roi );
	// Pour chaque ligne
	for ( int r = 0; r < dst.rows; ++r )
	{
		// pour chaque colonne
		for ( int c = 0; c < dst.cols; ++c )
		{
			// on récupère les channels
			const cv::Vec4b& vf = temp ( r , c );
			if ( vf [ 3 ] > 0 ) // si alpha channel > 0
			{
				// Blending
				cv::Vec3b& vb = dst ( r , c );
				vb [ 0 ] = vf [ 0 ];
				vb [ 1 ] = vf [ 1 ];
				vb [ 2 ] = vf [ 2 ];
			}
		}
	}
}

/// <summary>
/// <para>Pour placer une image en surimpression de la vidéo.</para>
/// <param name="position">Le centre du cercle à remplir avec une image.</param>
/// <param name="mode">L'image à utiliser.</param>
/// </summary>
void placePicture ( int x , int y , Mode mode )
{
	placePicture ( x , y , 50.0f , mode );
}



//cv::RNG rng(12345);

void detectAndDisplay ( cv::Mat frame )
{
	std::vector<cv::Rect> faces;
	cv::Mat frame_gray;

	cv::cvtColor ( frame , frame_gray , CV_BGR2GRAY );
	cv::equalizeHist ( frame_gray , frame_gray );

	//Detect Faces
	face_cascade.detectMultiScale ( frame_gray , faces , 1.5 , 4 , 0 | CV_HAAR_SCALE_IMAGE , cv::Size ( 20 , 20 ) );
	//cv::imshow ( "input gray" , frame_gray );
	//cv::waitKey();

	for ( size_t i = 0; i < faces.size ( ); i++ )
	{
		cv::Point center ( faces [ i ].x + faces [ i ].width*0.5 , faces [ i ].y + faces [ i ].height*0.5 );
		//ellipse ( frame , center , cv::Size ( faces [ i ].width*0.5 , faces [ i ].height*0.5 ) , 0 , 0 , 360 , cv::Scalar ( 255 , 0 , 255 ) , 4 , 8 , 0 );
		float radius = 50;
		if ( faces [ i ].width < faces [ i ].height )
		{
			radius = faces [ i ].height;
		}
		else
		{
			radius = faces [ i ].width;
		}

		cv::Mat faceroi = frame_gray ( faces [ i ] );
		cv::Mat divide = faceroi ( cv::Rect ( 0 , faceroi.size ( ).height / 2 , faceroi.size ( ).width , faceroi.size ( ).height / 2 ) );
		//std::vector<cv::Rect> eyes;
		Mode m = NEUTRAL;

		//-- In each face, detect eyes
		/*eyes_cascade.detectMultiScale ( faceroi , eyes , 1.1 , 2 , 0 | CV_HAAR_SCALE_IMAGE , cv::Size ( 30 , 30 ) );

		for ( size_t j = 0; j < eyes.size ( ); j++ )
		{
			cv::Point center ( faces [ i ].x + eyes [ j ].x + eyes [ j ].width*0.5 , faces [ i ].y + eyes [ j ].y + eyes [ j ].height*0.5 );
			int radius = cvRound ( ( eyes [ j ].width + eyes [ j ].height )*0.25 );
			circle ( frame , center , radius , cv::Scalar ( 255 , 0 , 0 ) , 4 , 8 , 0 );
		}*/

		/*std::vector<cv::Rect> mouths;
		mouth_cascade.detectMultiScale ( faceroi , mouths , 1.5 , 4 , 0 | CV_HAAR_SCALE_IMAGE , cv::Size ( 20 , 20 ) );
		//cv::imshow ( "input gray" , frame_gray );
		//cv::waitKey();

		for ( size_t i = 0; i < mouths.size ( ); i++ )
		{*/
		std::vector<cv::Rect> smiles;
		smile_cascade.detectMultiScale ( divide , smiles , 1.1 , 1 , 0 | CV_HAAR_SCALE_IMAGE , cv::Size ( 5 , 5 ) );

		if ( smiles.size ( ) > 0 )
		{
			m = SMILE;
		}
	//}

		placePicture ( center.x , center.y , radius , m );
	}

}

int video ( char* videoname )
{

	cv::VideoCapture cap;

	if ( !face_cascade.load ( face_cascade_name ) )
	{
		printf ( "--(!)Error loading face \n" ); return -1;
	};
	if ( !eyes_cascade.load ( eyes_cascade_name ) )
	{
		printf ( "--(!)Error loading eye \n" ); return -1;
	};
	if ( !smile_cascade.load ( smile_cascade_name ) )
	{
		printf ( "--(!)Error loading smile \n" ); return -1;
	};
	/*if ( mouth_cascade.load ( mouth_cascade_name ) )
	{
		printf ( "--(!)Error loading mouth \n" ); return -1;
	};*/

	cap = cv::VideoCapture ( 1 ); //Camera frontale par défault de ma machine
	if ( !cap.isOpened ( ) )
		cap = cv::VideoCapture ( 0 );

	if ( !cap.isOpened ( ) )  // check if we succeeded
		return -1;

	//recuperer une image depuis cap et la stocker dans nextInput
	cap >> nextInput;
	//tant que nextinput n’est pas vide
	while ( !nextInput.empty ( ) )
	{
		// - > faire les traitements sur l’image (prochaines étapes)
		detectAndDisplay ( nextInput );

		// - > appeler la fonction de dessin
		cv::imshow ( "input" , nextInput );
		// - > recuperer une nouvelle image et la stocker dans nextInput
		cap >> nextInput;
		// - > attendre 10ms que l’utilisateur tape une touche, et quitter si il le fait
		if ( cv::waitKey ( 10 ) >= 0 ) break;
	}
	return 0;
}

int main ( )
{
	int r = video ( NULL );

	system ( "PAUSE" );
	return r;
}

