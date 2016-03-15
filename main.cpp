#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
	VideoCapture video(0);    //Obtener imagenes desde la camara integrada
	if (!video.isOpened())    //Si no esta lista la camara... Termina
	{
		cout << "No se puede iniciar la camara" << endl;
		return -1;
	}

	Mat frame, prevFrame, movement;
	bool bSuccess = video.read(frame);  //Lee una nueva imagen del bufer

	if (!bSuccess)  //Si hay un error en la captura de imagen... Termina
	{
		cout << "No se puede tener captura del video" << endl;
		return -1;
	}
	prevFrame = frame.clone();
	while (true)
	{
		video >> frame;
		subtract(frame, prevFrame, movement);
		threshold(movement, movement, 10, 255, CV_THRESH_BINARY);

		//Elimina los objetos pequeños (Tratamiento morfologico -> Apertura)
		erode(movement, movement, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(movement, movement, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		//Rellena los espacios pequeños (Tratamiento morfologico -> Cerradura)
		dilate(movement, movement, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(movement, movement, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		cvtColor(movement, movement, CV_BGR2GRAY);

		imshow("Video", frame);
		imshow("Movimiento", movement);
		if (!frame.empty())
		{
			prevFrame = frame.clone();
		}
		if (waitKey(30) == 27)  //Si se presiona la tecla 'esc' por 30 ms, termina
		{
			cout << "Tecla \"esc\" presionada por el usuario" << endl;
			break;
		}
	}
	return 0;
}