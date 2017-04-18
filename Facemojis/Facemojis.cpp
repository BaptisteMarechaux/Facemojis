// Facemojis.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <opencv2\core\core.hpp>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\video\video.hpp>

cv::Mat nextInput;

/// <summary>
/// <para>L'image à utiliser.</para>
/// </summary>
enum Mode
{
	None , /// Pas d'image
	Normal , /// Image de base
	Smile /// Image avec un sourire
};

/// <summary>
/// <para>Pour placer une image en surimpression de la vidéo.</para>
/// <param name="position">Le centre du cercle à remplir avec une image.</param>
/// <param name="angle">L'angle de rotation de l'image à incorporer.</param>
/// <param name="radius">Le scale de l'image à effectuer.</param>
/// <param name="mode">L'image à utiliser.</param>
/// </summary>
void placePicture ( cv::Mat position , float angle , float radius , Mode mode )
{
	// prototype
}

/// <summary>
/// <para>Pour placer une image en surimpression de la vidéo.</para>
/// <param name="position">Le centre du cercle à remplir avec une image.</param>
/// <param name="angle">L'angle de rotation de l'image à incorporer.</param>
/// <param name="mode">L'image à utiliser.</param>
/// </summary>
void placePiture ( cv::Mat position , float angle , Mode mode )
{
	placePicture ( position , angle , 10.0 , mode );
}

/// <summary>
/// <para>Pour placer une image en surimpression de la vidéo.</para>
/// <param name="position">Le centre du cercle à remplir avec une image.</param>
/// <param name="mode">L'image à utiliser.</param>
/// </summary>
void placePiture ( cv::Mat position , Mode mode )
{
	placePicture ( position , 0.0 , mode );
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

