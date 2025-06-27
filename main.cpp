#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <time.h>
#include <csignal>
#include <queue>
#include <vector>
#include <iomanip>
#include <cstdio>

#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/calib3d.hpp>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

//#include <ncurses.h>

#include "ARUCO.h"


#include <unistd.h>
#include <sys/time.h>
#define Wait(ts) usleep((int)(1000000*ts))

using namespace std;

void signalHandler (int signum) {
    cout << "Interrupt Handling #" << signum << endl;
    // cleanup and close up stuff here  
    // terminate program  
    exit(signum);
}

int main() {
    ARUCO arucoCam;

    //==========================================================
    // std::string camera = "/base/soc/i2c0mux/i2c@1/imx708@1a";
    // std::string pipeline = "libcamerasrc camera-name=" + camera + " ae-enable=false exposure-time=5000 analogue-gain=2 af-mode=manual lens-position=1.33 ! video/x-raw,width=1536,height=864,format=RGBx ! videoconvert ! video/x-raw,format=BGR ! appsink sync=false max-lateness=0 drop=true";
    

    // cv::VideoCapture cap = cv::VideoCapture(pipeline, cv::CAP_GSTREAMER);
    // if (!cap.isOpened()){
    //     std::cerr << "Error: Unable to open the camera.\n";
    //     return -1;
    // }
    //===========================================================
    cv::VideoCapture cap("/dev/video0", cv::CAP_V4L2);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 360);

    arucoCam.readCalibrationParameters();
    cout << "Calibration Parameters loaded!" << endl;

    arucoCam.generateCharucoBoard();
    cout << "Done charuco" << endl;
    arucoCam.generateBoard(10000, 10);
    cout << "Done board" << endl;
    for (int i = 0; i < 10; i++) {
      arucoCam.generateMarkerId(i);
    }
    cout << "Done marker" << endl;
    //return 0;
    
    while(1){
        cap >> arucoCam.frame;
        

        // arucoCam.drawCrosshair();
        // arucoCam.getMarkersInFrame(1);
        // arucoCam.estimateMarkerPose(1);

        //imshow("Video Feed", arucoCam.frame);
        arucoCam.showFeed(1);
        
        int key = cv::waitKey(1);

        if (key == 's') {
            cout << "Pressed s" << endl;
            arucoCam.saveCalibrationImage();
        }

        else if (key == 'c')
            arucoCam.beginCalibration();

        else if (key == 'x')
            break;
    }
    
    return 0;
    
}

    /*
    struct timespec start, end;
    long elapsed = 0;
    char text[16];
    
    clock_gettime(CLOCK_MONOTONIC, &start);
    

    //ENTER FUNCTION HERE THAT YOU WANT TO MEASURE


    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000;

    // PRINT OUTPUT HERE
    sprintf(text, "%ld", elapsed);
    mvwaddstr(SCREEN, 8, 16, text);
    wrefresh(SCREEN);
    */
