// Facemojis.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <opencv2\core\core.hpp>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\video\video.hpp>

const cv::String NEUTRAL_PATH = "./Emoji/neutral.png";
const cv::String SMILE_PATH = "./Emoji/smile.png";
const cv::String WINK_PATH = "./Emoji/wink.png";
const cv::String SAD_PATH = "./Emoji/sad.png";

cv::Mat nextInput;

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
/// <param name="position">Le centre du cercle à remplir avec une image.</param>
/// <param name="angle">L'angle de rotation de l'image à incorporer.</param>
/// <param name="radius">Le scale de l'image à effectuer.</param>
/// <param name="mode">L'image à utiliser.</param>
/// </summary>
void placePicture ( int x , int y , float angle , float radius , Mode mode )
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
	cv::Mat img = cv::imread ( pathtoload );
	cv::imshow ( "Output" , img );
	// 2 resize l'image
	// 3 rotate l'image
	// 4 appliquer l'image
}

/// <summary>
/// <para>Pour placer une image en surimpression de la vidéo.</para>
/// <param name="position">Le centre du cercle à remplir avec une image.</param>
/// <param name="angle">L'angle de rotation de l'image à incorporer.</param>
/// <param name="mode">L'image à utiliser.</param>
/// </summary>
void placePicture ( int x , int y , float angle , Mode mode )
{
	placePicture ( x , y , angle , 10.0f , mode );
}

/// <summary>
/// <para>Pour placer une image en surimpression de la vidéo.</para>
/// <param name="position">Le centre du cercle à remplir avec une image.</param>
/// <param name="mode">L'image à utiliser.</param>
/// </summary>
void placePicture ( int x , int y , Mode mode )
{
	placePicture ( x , y , 0.0f , mode );
}

int video ( char* videoname )
{

	cv::VideoCapture cap;

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

		placePicture ( 0 , 0 , NEUTRAL );

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
	return r;
}

