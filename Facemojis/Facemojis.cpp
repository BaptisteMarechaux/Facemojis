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
	for ( int r = 0; r < dst.rows; ++r )
	{
		for ( int c = 0; c < dst.cols; ++c )
		{
			const cv::Vec4b& vf = temp ( r , c );
			if ( vf [ 3 ] > 0 ) // alpha channel > 0
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

		placePicture ( 50 , 50 , 50 , SMILE );

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

