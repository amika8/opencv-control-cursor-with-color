#pragma comment(lib, "winmm.lib") //for audio

#include<opencv2/opencv.hpp>

#include<iostream>

#include <Windows.h>

#include <stdlib.h>

#include <mmsystem.h>





using namespace cv;
using namespace std;

Point computeCentroid(const Mat& blueMask) {
    Moments m = moments(blueMask, true);
    Point center(m.m10 / m.m00, m.m01 / m.m00);
    return center;
}

Point incrementCircle(Point &ball) {
    
    //ball.x++;
    ball.y=ball.y+15;
    return ball;
}

void play()
{
    PlaySound(TEXT("BubbleSound.wav"), NULL, SND_FILENAME | SND_ASYNC); // plays sound.wav once
}


bool inRange(unsigned low, unsigned high, unsigned x) {
    return (low <= x && x <= high);
}

Mat ballon = imread("ballon.jpg", -1);


int main() {


    Mat myImage;//Declaring a matrix to load the frames
    VideoCapture cap(0);//Declaring an object to capture stream of frames
    if (!cap.isOpened()) { //This section prompt an error message if no video stream is found
        cout << "No video stream detected" << endl;
        system("pause");
        return-1;
    }


    Point ball(100, 0);
    int score = 0;

    while (true) { //Taking an everlasting loop to show the video//

        cap >> myImage;

        if (myImage.empty()) { //Breaking the loop if no video frame is detected//
            break;
        }

        //Clone de l'image original pour pouvoir comparer visuellement
        Mat resultat = myImage.clone();
        Mat game = myImage.clone();

        Mat blur;

        //Conversion de l'image RGB en HSV
        Mat resultatHsv;
        cvtColor(resultat, resultatHsv, COLOR_BGR2HSV);


        // Ajout d'un masque. HSV avec une teinte (hue) focalisé sur le rouge.
        //Mat1b mask1, mask2;
        //inRange(resultatHsv, Scalar(0, 100, 20), Scalar(10, 255, 255), mask1);
        //inRange(resultatHsv, Scalar(160, 100, 20), Scalar(179, 255, 255), mask2);

        //Mat1b mask = mask1 | mask2; // | : tout a 1 sauf quand mask 1 et 2 sont a 0.

        Mat1b blueMask;
        inRange(resultatHsv, Scalar(78, 158, 124), Scalar(138, 255, 255), blueMask);

        circle(blueMask, computeCentroid(blueMask), 7, Scalar(255, 0, 0), -1);

        putText(game, //target image
            "SCORE : "+ std::to_string(score), //text
            Point((game.cols / 2) - 100, 30), //top-left position
            FONT_HERSHEY_DUPLEX,
            1.0,
            CV_RGB(118, 185, 0), //font color
            2);


        cout << "balle : " << ball.y << " | " << ball.x << endl;
        cout << "souris : " << computeCentroid(blueMask).y << " | " << computeCentroid(blueMask).x << endl;


        if ( ((inRange(ball.y - 20, ball.y + 20, computeCentroid(blueMask).y-35)) && (inRange(ball.x - 20, ball.x + 20, computeCentroid(blueMask).x-150)))){
            ball.y = 0;
            ball.x = rand() % 500+50;
            play();
            score += 100;
        }


        if (ball.y > (game.rows + 10)) {
            ball.y = 0;
            ball.x = rand() % 500;

            score -= 50;
            //Point ball(rand() % 100, 0);
        }


        //cout << "windows y size : " << game.rows << endl;
        //cout << "windows x size : " << game.cols << endl;

        //cout << "ball position y : " << ball.y << endl;
        //cout << "ball position x : " << ball.x << endl;

        circle(game, incrementCircle(ball), 25, Scalar(rand() % 255, rand() % 255, rand() % 255), -1);



        SetCursorPos(computeCentroid(blueMask).x, computeCentroid(blueMask).y);


        imshow("Video Player - original image", myImage);//Showing the original video//
        //imshow("Video Player - resultat image HSV", resultatHsv);//Showing the result//
        //imshow("Video Player - result", blueMask);//Showing the result//
        imshow("Video Player - game", game);

        char c = (char)waitKey(25);//Allowing 25 milliseconds frame processing time and initiating break condition//
        if (c == 27) { //If 'Esc' is entered break the loop//
            break;
        }
        
        
    }
    cap.release();//Releasing the buffer memory//
    return 0;


}


