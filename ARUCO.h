#ifndef ARUCO_H_
#define ARUCO_H_

// Essentials
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
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/calib3d.hpp>  // Include the calib3d module

// Marker Generator
#define MARKER_FOLDER "MarkerImages/marker"
#define CALIBRATION_FOLDER "CalibrationImages"

#define MARKER_PX_SIZE 200
#define MARKER_PX_BORDER 1

// Charuco Board
#define MAX_CALIBRATION_IMAGES 30
#define CAL_X_SQUARES 4
#define CAL_Y_SQUARES 6
#define SQUARE_LENGTH 0.04f // in meters
#define MARKER_LENGTH 0.02f

using namespace cv;
using namespace std;


class ARUCO {
    private:
        // Internal Functions
        
        void drawSquare(Point2f tl, Point2f tr, Point2f br, Point2f bl);
        

    public:
        // ARUCO Library Variables
        Mat frame;
        Mat previewFrame;
        Mat rawImage;
        Mat cameraMatrix;
        Mat distCoeffs;
        Ptr<aruco::Dictionary> dictionary;
        Ptr<aruco::DetectorParameters> parameters;
        Ptr<aruco::GridBoard> gridboard;
        Ptr<aruco::Board> board;

        vector<int> markerIds;
        vector<vector<Point2f>> markerCorners, rejectedCandidates, miniMarkerCorners; 
        vector<Vec3d> rvecs, tvecs;

        

        map<int, Point2f> centers;
        vector<Point2f> imagePoints;
        vector<Point3f> objectPoints;
        Vec3d rvec, tvec;

        Vec3d camtvec, camrvec;
        vector<Point2f> currentCorners;

        // Calibration Variables
        float markerLength = 0.02f; //50 cm
        int numSamples = 0;

        int camId = 0;
        int createId = 0;
        int calibrated = 0;


        // Grid board
        int no_of_x = 2;
        int no_of_y = 2;
        int gapLength = 0.01f;

        // Flags
        
        // Constructors
        ARUCO() {
            //dictionary = aruco::getPredefinedDictionary(aruco::DICT_6X6_250);
            dictionary = aruco::getPredefinedDictionary(aruco::DICT_APRILTAG_36h11);
            parameters =  aruco::DetectorParameters::create();
            
            // Detection Tuning Parameters
            parameters->adaptiveThreshWinSizeStep = 5;  // default is 10
            parameters->minMarkerPerimeterRate = 0.01;  // default is 0.03
            parameters->minCornerDistanceRate = 0.01;    // default is 0.05
            parameters->minDistanceToBorder = 1;    // default is 3

            // parameters->cornerRefinementMethod = cv::aruco::CORNER_REFINE_APRILTAG;
            parameters->cornerRefinementMethod = cv::aruco::CORNER_REFINE_SUBPIX;
            parameters->cornerRefinementMaxIterations = 35; // default is 30
            parameters->cornerRefinementMinAccuracy = 0.01;

            parameters->perspectiveRemovePixelPerCell = 8;  // default is 4 (more pixel per bit for smaller markers)

            // Error handling
            parameters->maxErroneousBitsInBorderRate = 0.3;
            parameters->errorCorrectionRate = 0.6; 
            
            gridboard = aruco::GridBoard::create(no_of_x, no_of_y, 0.02f, 0.01f, dictionary);// no_x, no_y, markerLength, gapLength, dictionary
            board = gridboard.staticCast< aruco::Board>();
        }


        // Marker
        void generateMarkerId(int id);
        void generateCharucoBoard();
        void generateBoard(int ppm, int px_border);

        // Calibration
        int readCalibrationParameters();
        int countCalibrationImages();
        void saveCalibrationImage();
        void removeLastCalibrationImage();
        void removeAllCalibrationImage();
        int beginCalibration();

        // Detection
        void drawCrosshair();
        void getMarkersInFrame(int enableDraw);
        void estimateMarkerPose(int enableDraw);
        void estimateBoardPose(int enableDraw);
        void getSquare(int tl, int tr, int br, int bl, float square_size);

        void applyCorrection(Vec3d &rV);

        // Data Extortion
        void showFeed(float scale);
        void exportData(const std::string& filename);
        
        
};

#endif
