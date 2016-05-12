//
//  main.cpp
//  clustering
//
//  Created by QiaoTian on 12/31/14.
//  Copyright (c) 2014 State Key Laboratory of Intelligent Technology and Systems. All rights reserved.
//

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cvaux.hpp>
#include <opencv/cv.h>

//int main(int argc, char** argv)
//{
//    IplImage * pInpImg = 0;
//    
//    // Load an image from file - change this based on your image name
//    pInpImg = cvLoadImage("my_image.jpg", CV_LOAD_IMAGE_UNCHANGED);
//    if(!pInpImg)
//    {
//        fprintf(stderr, "failed to load input image\n");
//        return -1;
//    }
//    
//    // Write the image to a file with a different name,
//    // using a different image format -- .png instead of .jpg
//    if( !cvSaveImage("my_image_copy.png", pInpImg) )
//    {
//        fprintf(stderr, "failed to write image file\n");
//    }
//    
//    // Remember to free image memory after using it!
//    cvReleaseImage(&pInpImg);
//    
//    return 0;
//}

//#include "iostream"
//#include "vector"
//#include "math.h"
//
//using namespace std;
//using namespace cv;
//
//#define WINDOW_WIDTH 1000
//#define WINDOW_HEIGHT 1000
//
//#define DATA_MAX 40
//
//#define WINDOW_RHO  0.08
//
//#define DIM 3
//#define elif else if
//
//#ifndef bool
//#define bool int
//#define false ((bool)0)
//#define true  ((bool)1)
//#endif
//
//enum input_source {
//    flame,
//    compound,
//    d31,
//    aggregation,
//    jain,
//    pathbased,
//    r15,
//    spiral,
//    
//};
//
//struct qtPoint3d {
//    double x;
//    double y;
//    double z;
//    qtPoint3d(double xin, double yin, double zin) : x(xin), y(yin), z(zin) {}
//};
//
//float dist[1000][1000];
//FILE *input;
//vector<vector<double> > data;
//vector<qtPoint3d> points;
//int tag[1000];//标记所属分类 上限1000个点
//vector<vector<qtPoint3d>> clusters;//盛放最终分类完成后的点集
//vector<int> center;//聚类中心
//float maxdist;//两点之间最大距离
//input_source datatype;
//
//
//void dataPro(vector<vector<double> > &src, vector<qtPoint3d> &dst){
//    for(int i = 0; i < src.size(); i++){
//        qtPoint3d pt(src[i][0], src[i][1], src[i][2]);
//        dst.push_back(pt);
//    }
//}
//
//double getDistance(qtPoint3d &pt1, qtPoint3d &pt2){
//    double tmp = pow(pt1.x - pt2.x, 2) + pow(pt1.y - pt2.y, 2) + pow(pt1.z - pt2.z, 2);
//    return pow(tmp, 0.5);
//}
//
//double getdc(vector<qtPoint3d> &data, double neighborRateLow, double neighborRateHigh){
//    int nSamples = (int)data.size();
//    int nLow = neighborRateLow * nSamples * nSamples;
//    int nHigh = neighborRateHigh * nSamples * nSamples;
//    double dc = 0.0;
//    int neighbors = 0;
//    //cout<<"nLow = "<<nLow<<", nHigh = "<<nHigh<<endl;
//    while(neighbors < nLow || neighbors > nHigh){
//        //while(dc <= 1.0){
//        neighbors = 0;
//        for(int i = 0; i < nSamples - 1; i++){
//            for(int j = i + 1; j < nSamples; j++){
//                if(getDistance(data[i], data[j]) <= dc) ++neighbors;
//                if(neighbors > nHigh) goto DCPLUS;
//            }
//        }
//        DCPLUS: dc += 0.03;
//        cout<<"dc = "<<dc<<", neighbors = "<<neighbors<<endl;
//    }
//    return dc;
//}
//
//vector<int> getLocalDensity(vector<qtPoint3d> &points, double dc){
//    int nSamples = (int)points.size();
//    vector<int> rho(nSamples, 0);
//    for(int i = 0; i < nSamples - 1; i++){
//        for(int j = i + 1; j < nSamples; j++){
//            if(getDistance(points[i], points[j]) < dc){
//                ++rho[i];
//                ++rho[j];
//            }
//        }
//        //cout<<"getting rho. Processing point No."<<i<<endl;
//    }
//    return rho;
//}
//
//vector<double> getDistanceToHigherDensity(vector<qtPoint3d> &points, vector<int> rho){
//    int nSamples = (int)points.size();
//    vector<double> delta(nSamples, 0.0);
//    
//    for(int i = 0; i < nSamples; i++){
//        double dist = 0.0;
//        bool flag = false;
//        for(int j = 0; j < nSamples; j++){
//            if(i == j) continue;
//            if(rho[j] > rho[i]){
//            //if (delta[j] > delta[i]) {
//                double tmp = getDistance(points[i], points[j]);
//                if(!flag){
//                    dist = tmp;
//                    flag = true;
//                }else dist = tmp < dist ? tmp : dist;
//            }
//        }
//        if(!flag){
//            for(int j = 0; j < nSamples; j++){
//                double tmp = getDistance(points[i], points[j]);
//                dist = tmp > dist ? tmp : dist;
//            }
//        }
//        delta[i] = dist;
//        //cout<<"getting delta. Processing point No."<<i<<endl;
//    }
//    return delta;
//}
//
////bool myfunction(int i,int j)
////{
////    return (i > j);
////}
//void getCluster(vector<qtPoint3d> &points, vector<int> &rho, vector<double> &delta) {
//    //对rho进行降序排列, 记录index变化
//    int size = (int)rho.size();
//    //找到密度最大点 更新dist
//    int maxrhoInx = 0;
//    for (int i = 0; i<size; ++i) {
//        if (rho[i]>rho[maxrhoInx]) {
//            maxrhoInx = i;
//        }
//    }
//    delta[maxrhoInx] = maxdist;
//    
//    vector<int> rhoCopy;
//    vector<double> deltaCopy;
//    for (int i = 0; i< size; ++i) {
//        rhoCopy.push_back(rho[i]);
//    }
//    for (int i = 0; i< size; ++i) {
//        deltaCopy.push_back(delta[i]);
//    }
//    int rhoInx[1000];
//    for (int i = 0; i<size; ++i) {
//        rhoInx[i] = i;// 初始化
//    }
//    //sort(rho.begin(), rho.end(), myfunction);//降序
//    double indextemp;
//    int rhotemp;
//    double deltatemp;
//    for(int i = 0; i< size-1;i++) {
//        for (int j=i+1; j<size; ++j) {
//            if (deltaCopy[i]<deltaCopy[j]) {
//                indextemp = rhoInx[i];
//                rhoInx[i] = rhoInx[j];
//                rhoInx[j] = indextemp;
//                
//                deltatemp = deltaCopy[i];
//                deltaCopy[i] = deltaCopy[j];
//                deltaCopy[j] = deltatemp;
//                
//                rhotemp = rhoCopy[i];
//                rhoCopy[i] = rhoCopy[j];
//                rhoCopy[j] = rhotemp;
//            }
//        }
//    }
//    for(int i = 0; i< size-1;i++) {
//        for (int j=i+1; j<size; ++j) {
//            if (rhoCopy[i]<rhoCopy[j]) {
//                indextemp = rhoInx[i];
//                rhoInx[i] = rhoInx[j];
//                rhoInx[j] = indextemp;
//                
//                rhotemp = rhoCopy[i];
//                rhoCopy[i] = rhoCopy[j];
//                rhoCopy[j] = rhotemp;
//                
//                deltatemp = deltaCopy[i];
//                deltaCopy[i] = deltaCopy[j];
//                deltaCopy[j] = deltatemp;
//            }
//        }
//    }
//    
//    for (int i = 0; i< rho.size(); ++i) {
//        cout << rhoCopy[i] << " " << deltaCopy[i] << endl;
//    }
//    
//    vector<vector<int>> clusters;
//    int count = (int)rho.size(); // total number of points
//    //sort(rho.begin(), rho.end());
//    
//    float threshRho = 0.0;
//    float threshDelta = 0;
//    if (datatype == flame) {
//        threshRho = 14;
//        threshDelta = 8;
//    }
//    else if (datatype == spiral) {
//        threshRho = 10;
//        threshDelta = 7;
//    } else if (datatype == compound){
//        threshRho = 21;
//        threshDelta = 6;
//    }
//    
//    
//    int newTag = 0;
//    // 
//    for (int i = 0; i<count; ++i) {
//        if (delta[rhoInx[i]]>threshDelta && rho[rhoInx[i]]>threshRho) {// is a center point
//            vector<int>single;
//            single.push_back(rhoInx[i]);
//            clusters.push_back(single);
//            center.push_back(rhoInx[i]);
//            tag[rhoInx[i]] = newTag++;//0\1\2\3...
//            
//        }
//        else if (delta[rhoInx[i]]>threshDelta && rho[rhoInx[i]]<= threshRho) { // a isolated point
//            tag[rhoInx[i]] = -1;
//        }
//        else {
//            int neighbour = i-1;// set neighbour i+1, and update it
//            if (neighbour == 0 || neighbour == -1) {
//                neighbour = 0;
//                tag[i] = tag[rhoInx[0]];
//            } else {
//                for (int j = 0; j < neighbour; ++j) {
//                    if(dist[rhoInx[i]][rhoInx[neighbour]]>dist[rhoInx[i]][rhoInx[j]]) neighbour = j;
//                }
//                tag[rhoInx[i]] = tag[rhoInx[neighbour]];
//            }
//            tag[rhoInx[i]] = tag[rhoInx[neighbour]];
//        }
//    }
//}
//
//void draw(vector<qtPoint3d> &mpoints, vector<int> &rho, vector<double>delta, vector<int> &mcenter){
//    IplImage *image = cvCreateImage(cvSize(WINDOW_WIDTH, WINDOW_HEIGHT), 8, 3);
//    cvLine(image, cvPoint(0, WINDOW_HEIGHT/2), cvPoint(WINDOW_WIDTH, WINDOW_HEIGHT/2), CV_RGB(255, 255, 255));
//    cvLine(image, cvPoint(WINDOW_WIDTH/2, 0), cvPoint(WINDOW_WIDTH/2, WINDOW_HEIGHT), CV_RGB(255, 255, 255));
//    int count = (int)mpoints.size();
//    
//
//    /* draw center */
//    if (datatype == spiral) {
//        /* draw original points */
//        for (int i = 0; i<count; ++i) {
//            cvCircle(image, cvPoint(mpoints[i].x/WINDOW_RHO, WINDOW_HEIGHT/2 - points[i].y/WINDOW_RHO), 3, CV_RGB(255, 255, 0));
//        }
//        /* draw rho & delta */
//        for (int i = 0; i<count ; ++i) {
//            cvCircle(image, cvPoint(rho[i]*20+WINDOW_WIDTH/2, WINDOW_HEIGHT/2 - delta[i]*20), 3, CV_RGB(0, 255, 0));
//        }
//        for (int i = 0; i<count; ++i) {
//            if (tag[i] == -1) {
//                cvCircle(image, cvPoint(points[i].x/WINDOW_RHO, WINDOW_HEIGHT - points[i].y/WINDOW_RHO), 3, CV_RGB(255, 255, 255));
//            }
//            else if (tag[i] == 0){
//                cvCircle(image, cvPoint(points[i].x/WINDOW_RHO, WINDOW_HEIGHT - points[i].y/WINDOW_RHO), 3, CV_RGB(255, 0, 0));
//            }
//            else if (tag[i] == 1) {
//                cvCircle(image, cvPoint(points[i].x/WINDOW_RHO, WINDOW_HEIGHT - points[i].y/WINDOW_RHO), 3, CV_RGB(0, 255, 0));
//            }
//            else if (tag[i] == 2) {
//                cvCircle(image, cvPoint(points[i].x/WINDOW_RHO, WINDOW_HEIGHT - points[i].y/WINDOW_RHO), 3, CV_RGB(0, 0, 255));
//            }
//            else if (tag[i] == 3) {
//                cvCircle(image, cvPoint(points[i].x/WINDOW_RHO, WINDOW_HEIGHT - points[i].y/WINDOW_RHO), 3, CV_RGB(255, 255, 0));
//            } else {
//                cvCircle(image, cvPoint(points[i].x/WINDOW_RHO, WINDOW_HEIGHT - points[i].y/WINDOW_RHO), 3, CV_RGB(0, 255, 255));
//            }
//        }
//        for(int i = 0; i<center.size(); ++i){
//            cvCircle(image, cvPoint(points[mcenter[i]].x/WINDOW_RHO, WINDOW_HEIGHT - points[mcenter[i]].y/WINDOW_RHO), 10, CV_RGB(150, 255, 45));
//        }
//    }
//    else if (datatype == flame) {
//        /* draw original points */
//        for (int i = 0; i<count; ++i) {
//            cvCircle(image, cvPoint(mpoints[i].x*1.5/WINDOW_RHO+100, WINDOW_HEIGHT/2 - points[i].y*1.5/WINDOW_RHO+150), 3, CV_RGB(255, 255, 0));
//        }
//        /* draw rho & delta */
//        for (int i = 0; i<count ; ++i) {
//            cvCircle(image, cvPoint(rho[i]*20+WINDOW_WIDTH/2, WINDOW_HEIGHT/2 - delta[i]*20), 3, CV_RGB(0, 255, 0));
//        }
//        for (int i = 0; i<count; ++i) {
//            if (tag[i] == -1) {
//                cvCircle(image, cvPoint(points[i].x*1.5/WINDOW_RHO+100, WINDOW_HEIGHT - points[i].y*1.5/WINDOW_RHO+150), 3, CV_RGB(255, 255, 255));
//            }
//            else if (tag[i] == 0){
//                cvCircle(image, cvPoint(points[i].x*1.5/WINDOW_RHO+100, WINDOW_HEIGHT - points[i].y*1.5/WINDOW_RHO+150), 3, CV_RGB(255, 0, 0));
//            }
//            else if (tag[i] == 1) {
//                cvCircle(image, cvPoint(points[i].x*1.5/WINDOW_RHO+100, WINDOW_HEIGHT - points[i].y*1.5/WINDOW_RHO+150), 3, CV_RGB(0, 255, 0));
//            }
//            else if (tag[i] == 2) {
//                cvCircle(image, cvPoint(points[i].x*1.5/WINDOW_RHO+100, WINDOW_HEIGHT - points[i].y*1.5/WINDOW_RHO+150), 3, CV_RGB(0, 0, 255));
//            }
//            else if (tag[i] == 3) {
//                cvCircle(image, cvPoint(points[i].x*1.5/WINDOW_RHO+100, WINDOW_HEIGHT - points[i].y*1.5/WINDOW_RHO+150), 3, CV_RGB(255, 255, 0));
//            } else {
//                cvCircle(image, cvPoint(points[i].x*1.5/WINDOW_RHO+100, WINDOW_HEIGHT - points[i].y*1.5/WINDOW_RHO+150), 3, CV_RGB(0, 255, 255));
//            }
//        }
//        for(int i = 0; i<center.size(); ++i){
//            cvCircle(image, cvPoint(points[mcenter[i]].x/WINDOW_RHO, WINDOW_HEIGHT - points[mcenter[i]].y/WINDOW_RHO), 10, CV_RGB(150, 255, 45));
//        }
//    } else if (datatype == compound){
//        /* draw original points */
//        for (int i = 0; i<count; ++i) {
//            cvCircle(image, cvPoint(mpoints[i].x/WINDOW_RHO-60, WINDOW_HEIGHT/2 - points[i].y/WINDOW_RHO-100), 3, CV_RGB(255, 255, 0));
//        }
//        /* draw rho & delta */
//        for (int i = 0; i<count ; ++i) {
//            cvCircle(image, cvPoint(rho[i]*15+WINDOW_WIDTH/2, WINDOW_HEIGHT/2 - delta[i]*15), 3, CV_RGB(0, 255, 0));
//        }
//        for (int i = 0; i<count; ++i) {
//            if (tag[i] == -1) {
//                cvCircle(image, cvPoint(points[i].x/WINDOW_RHO-60, WINDOW_HEIGHT - points[i].y/WINDOW_RHO-100), 3, CV_RGB(255, 255, 255));
//            }
//            else if (tag[i] == 0){
//                cvCircle(image, cvPoint(points[i].x/WINDOW_RHO-60, WINDOW_HEIGHT - points[i].y/WINDOW_RHO-100), 3, CV_RGB(255, 0, 0));
//            }
//            else if (tag[i] == 1) {
//                cvCircle(image, cvPoint(points[i].x/WINDOW_RHO-60, WINDOW_HEIGHT - points[i].y/WINDOW_RHO-100), 3, CV_RGB(0, 255, 0));
//            }
//            else if (tag[i] == 2) {
//                cvCircle(image, cvPoint(points[i].x/WINDOW_RHO-60, WINDOW_HEIGHT - points[i].y/WINDOW_RHO-100), 3, CV_RGB(0, 0, 255));
//            }
//            else if (tag[i] == 3) {
//                cvCircle(image, cvPoint(points[i].x/WINDOW_RHO-60, WINDOW_HEIGHT - points[i].y/WINDOW_RHO-100), 3, CV_RGB(255, 255, 0));
//            } else {
//                cvCircle(image, cvPoint(points[i].x/WINDOW_RHO-60, WINDOW_HEIGHT - points[i].y/WINDOW_RHO-100), 3, CV_RGB(0, 255, 255));
//            }
//        }
//        /* draw center */
//        for(int i = 0; i<center.size(); ++i){
//            cvCircle(image, cvPoint(points[mcenter[i]].x/WINDOW_RHO-60, WINDOW_HEIGHT - points[mcenter[i]].y/WINDOW_RHO-100), 10, CV_RGB(150, 255, 45));
//        }
//    }
//    cvShowImage("window", image);
//    cvWaitKey();
//}
//
//void init(){
//    //input  = fopen("/Users/qiaotian/Desktop/flame.txt", "r");
//    input  = fopen("/Users/qiaotian/Desktop/Compound.txt", "r");
//    //input  = fopen("/Users/qiaotian/Desktop/D31.txt", "r");
//    //input  = fopen("/Users/qiaotian/Desktop/Aggregation.txt", "r");
//    //input  = fopen("/Users/qiaotian/Desktop/jain.txt", "r");
//    //input  = fopen("/Users/qiaotian/Desktop/pathbased.txt", "r");
//    //input  = fopen("/Users/qiaotian/Desktop/R15.txt", "r");
//    //input  = fopen("/Users/qiaotian/Desktop/spiral.txt", "r");
//    datatype = compound;
//    double tpdouble;
//    int counter = 0;
//    while(1){
//        if(fscanf(input, "%lf", &tpdouble)==EOF) break;
//        if (counter / 3 >= data.size()) {
//            vector<double> tpvec;
//            data.push_back(tpvec);
//        }
//        data[counter/3].push_back(tpdouble);
//        ++ counter;
//    }
//    fclose(input);
//    random_shuffle(data.begin(), data.end());
//    
//    dataPro(data, points);
//    
//    maxdist = 0;
//    for (int i = 0 ; i < counter/3; ++i) {
//        dist[i][i] = 0;
//        for (int j = i+1; j< counter/3; ++j) {
//            dist[i][j] = sqrt(pow(points[i].x-points[j].x, 2.0)+pow(points[i].y-points[j].y, 2.0));
//            dist[j][i] = dist[i][j];
//            if (dist[i][j] > maxdist) {
//                maxdist = dist[i][j];
//            }
//            if(j == 399) {
//                cout << "haha" << endl;
//            }
//        }
//    }
//    // initialize the tag
//    for (int i = 0; i<counter/3; ++i) {
//        tag[i] = -1;//0---unclassified
//    }
//}
//
//int main(int argc, char** argv)
//{
//    init();
//    long start, end;
//    start = clock();
//    
//    double dc = getdc(points, 0.016, 0.020);
//    //double dc = 0.36;
//    vector<int> rho = getLocalDensity(points, dc);
//    vector<double> delta = getDistanceToHigherDensity(points, rho);
//    
//    getCluster(points, rho, delta);
//    draw(points, rho, delta, center);
//    
//    //saveToTxt(rho, delta);
//    //now u get the cluster centers
//    end = clock();
//    //cout<<"used time: "<<((double)(end - start)) / CLOCKS_PER_SEC<<endl;
//    return 0;
//}

/*
 * 3calibration.cpp -- Calibrate 3 cameras in a horizontal line together.
 */

#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <stdio.h>
#include <string.h>
#include <time.h>

using namespace cv;
using namespace std;

enum { DETECTION = 0, CAPTURING = 1, CALIBRATED = 2 };

static void help()
{
    printf( "\nThis is a camera calibration sample that calibrates 3 horizontally placed cameras together.\n"
           "Usage: 3calibration\n"
           "     -w <board_width>         # the number of inner corners per one of board dimension\n"
           "     -h <board_height>        # the number of inner corners per another board dimension\n"
           "     [-s <squareSize>]       # square size in some user-defined units (1 by default)\n"
           "     [-o <out_camera_params>] # the output filename for intrinsic [and extrinsic] parameters\n"
           "     [-zt]                    # assume zero tangential distortion\n"
           "     [-a <aspectRatio>]      # fix aspect ratio (fx/fy)\n"
           "     [-p]                     # fix the principal point at the center\n"
           "     [input_data]             # input data - text file with a list of the images of the board\n"
           "\n" );
    
}

static void calcChessboardCorners(Size boardSize, float squareSize, vector<Point3f>& corners)
{
    corners.resize(0);
    
    for( int i = 0; i < boardSize.height; i++ )
        for( int j = 0; j < boardSize.width; j++ )
            corners.push_back(Point3f(float(j*squareSize),
                                      float(i*squareSize), 0));
}

static bool run3Calibration( vector<vector<Point2f> > imagePoints1,
                            vector<vector<Point2f> > imagePoints2,
                            vector<vector<Point2f> > imagePoints3,
                            Size imageSize, Size boardSize,
                            float squareSize, float aspectRatio,
                            int flags,
                            Mat& cameraMatrix1, Mat& distCoeffs1,
                            Mat& cameraMatrix2, Mat& distCoeffs2,
                            Mat& cameraMatrix3, Mat& distCoeffs3,
                            Mat& R12, Mat& T12, Mat& R13, Mat& T13)
{
    int c, i;
    
    // step 1: calibrate each camera individually
    vector<vector<Point3f> > objpt(1);
    vector<vector<Point2f> > imgpt;
    calcChessboardCorners(boardSize, squareSize, objpt[0]);
    vector<Mat> rvecs, tvecs;
    
    for( c = 1; c <= 3; c++ )
    {
        const vector<vector<Point2f> >& imgpt0 = c == 1 ? imagePoints1 : c == 2 ? imagePoints2 : imagePoints3;
        imgpt.clear();
        int N = 0;
        for( i = 0; i < (int)imgpt0.size(); i++ )
            if( !imgpt0[i].empty() )
            {
                imgpt.push_back(imgpt0[i]);
                N += (int)imgpt0[i].size();
            }
        
        if( imgpt.size() < 3 )
        {
            printf("Error: not enough views for camera %d\n", c);
            return false;
        }
        
        objpt.resize(imgpt.size(),objpt[0]);
        
        Mat cameraMatrix = Mat::eye(3, 3, CV_64F);
        if( flags & CV_CALIB_FIX_ASPECT_RATIO )
            cameraMatrix.at<double>(0,0) = aspectRatio;
        
        Mat distCoeffs = Mat::zeros(5, 1, CV_64F);
        
        double err = calibrateCamera(objpt, imgpt, imageSize, cameraMatrix,
                                     distCoeffs, rvecs, tvecs,
                                     flags|CV_CALIB_FIX_K3/*|CV_CALIB_FIX_K4|CV_CALIB_FIX_K5|CV_CALIB_FIX_K6*/);
        bool ok = checkRange(cameraMatrix) && checkRange(distCoeffs);
        if(!ok)
        {
            printf("Error: camera %d was not calibrated\n", c);
            return false;
        }
        printf("Camera %d calibration reprojection error = %g\n", c, sqrt(err/N));
        
        if( c == 1 )
            cameraMatrix1 = cameraMatrix, distCoeffs1 = distCoeffs;
        else if( c == 2 )
            cameraMatrix2 = cameraMatrix, distCoeffs2 = distCoeffs;
        else
            cameraMatrix3 = cameraMatrix, distCoeffs3 = distCoeffs;
    }
    
    vector<vector<Point2f> > imgpt_right;
    
    // step 2: calibrate (1,2) and (3,2) pairs
    for( c = 2; c <= 3; c++ )
    {
        const vector<vector<Point2f> >& imgpt0 = c == 2 ? imagePoints2 : imagePoints3;
        
        imgpt.clear();
        imgpt_right.clear();
        int N = 0;
        
        for( i = 0; i < (int)std::min(imagePoints1.size(), imgpt0.size()); i++ )
            if( !imagePoints1.empty() && !imgpt0[i].empty() )
            {
                imgpt.push_back(imagePoints1[i]);
                imgpt_right.push_back(imgpt0[i]);
                N += (int)imgpt0[i].size();
            }
        
        if( imgpt.size() < 3 )
        {
            printf("Error: not enough shared views for cameras 1 and %d\n", c);
            return false;
        }
        
        objpt.resize(imgpt.size(),objpt[0]);
        Mat cameraMatrix = c == 2 ? cameraMatrix2 : cameraMatrix3;
        Mat distCoeffs = c == 2 ? distCoeffs2 : distCoeffs3;
        Mat R, T, E, F;
        double err = stereoCalibrate(objpt, imgpt, imgpt_right, cameraMatrix1, distCoeffs1,
                                     cameraMatrix, distCoeffs,
                                     imageSize, R, T, E, F,
                                     TermCriteria(TermCriteria::COUNT, 30, 0),
                                     CV_CALIB_FIX_INTRINSIC);
        printf("Pair (1,%d) calibration reprojection error = %g\n", c, sqrt(err/(N*2)));
        if( c == 2 )
        {
            cameraMatrix2 = cameraMatrix;
            distCoeffs2 = distCoeffs;
            R12 = R; T12 = T;
        }
        else
        {
            R13 = R; T13 = T;
        }
    }
    
    return true;
}

static bool readStringList( const string& filename, vector<string>& l )
{
    l.resize(0);
    FileStorage fs(filename, FileStorage::READ);
    if( !fs.isOpened() )
        return false;
    FileNode n = fs.getFirstTopLevelNode();
    if( n.type() != FileNode::SEQ )
        return false;
    FileNodeIterator it = n.begin(), it_end = n.end();
    for( ; it != it_end; ++it )
        l.push_back((string)*it);
    return true;
}


int main( int argc, char** argv )
{
    int i, k;
    int flags = 0;
    Size boardSize, imageSize;
    float squareSize = 1.f, aspectRatio = 1.f;
    const char* outputFilename = "out_camera_data.yml";
    const char* inputFilename = 0;
    
    vector<vector<Point2f> > imgpt[3];
    vector<string> imageList;
    
    if(argc < 2)
    {
        help();
        return 1;
    }
    
    
    for( i = 1; i < argc; i++ )
    {
        const char* s = argv[i];
        if( strcmp( s, "-w" ) == 0 )
        {
            if( sscanf( argv[++i], "%u", &boardSize.width ) != 1 || boardSize.width <= 0 )
                return fprintf( stderr, "Invalid board width\n" ), -1;
        }
        else if( strcmp( s, "-h" ) == 0 )
        {
            if( sscanf( argv[++i], "%u", &boardSize.height ) != 1 || boardSize.height <= 0 )
                return fprintf( stderr, "Invalid board height\n" ), -1;
        }
        else if( strcmp( s, "-s" ) == 0 )
        {
            if( sscanf( argv[++i], "%f", &squareSize ) != 1 || squareSize <= 0 )
                return fprintf( stderr, "Invalid board square width\n" ), -1;
        }
        else if( strcmp( s, "-a" ) == 0 )
        {
            if( sscanf( argv[++i], "%f", &aspectRatio ) != 1 || aspectRatio <= 0 )
                return printf("Invalid aspect ratio\n" ), -1;
            flags |= CV_CALIB_FIX_ASPECT_RATIO;
        }
        else if( strcmp( s, "-zt" ) == 0 )
        {
            flags |= CV_CALIB_ZERO_TANGENT_DIST;
        }
        else if( strcmp( s, "-p" ) == 0 )
        {
            flags |= CV_CALIB_FIX_PRINCIPAL_POINT;
        }
        else if( strcmp( s, "-o" ) == 0 )
        {
            outputFilename = argv[++i];
        }
        else if( s[0] != '-' )
        {
            inputFilename = s;
        }
        else
            return fprintf( stderr, "Unknown option %s", s ), -1;
    }
    
    if( !inputFilename ||
       !readStringList(inputFilename, imageList) ||
       imageList.size() == 0 || imageList.size() % 3 != 0 )
    {
        printf("Error: the input image list is not specified, or can not be read, or the number of files is not divisible by 3\n");
        return -1;
    }
    
    Mat view, viewGray;
    Mat cameraMatrix[3], distCoeffs[3], R[3], P[3], R12, T12;
    for( k = 0; k < 3; k++ )
    {
        cameraMatrix[k] = Mat_<double>::eye(3,3);
        cameraMatrix[k].at<double>(0,0) = aspectRatio;
        cameraMatrix[k].at<double>(1,1) = 1;
        distCoeffs[k] = Mat_<double>::zeros(5,1);
    }
    Mat R13=Mat_<double>::eye(3,3), T13=Mat_<double>::zeros(3,1);
    
    FileStorage fs;
    namedWindow( "Image View", 0 );
    
    for( k = 0; k < 3; k++ )
        imgpt[k].resize(imageList.size()/3);
    
    for( i = 0; i < (int)(imageList.size()/3); i++ )
    {
        for( k = 0; k < 3; k++ )
        {
            int k1 = k == 0 ? 2 : k == 1 ? 0 : 1;
            printf("%s\n", imageList[i*3+k].c_str());
            view = imread(imageList[i*3+k], 1);
            
            if(view.data)
            {
                vector<Point2f> ptvec;
                imageSize = view.size();
                cvtColor(view, viewGray, COLOR_BGR2GRAY);
                bool found = findChessboardCorners( view, boardSize, ptvec, CV_CALIB_CB_ADAPTIVE_THRESH );
                
                drawChessboardCorners( view, boardSize, Mat(ptvec), found );
                if( found )
                {
                    imgpt[k1][i].resize(ptvec.size());
                    std::copy(ptvec.begin(), ptvec.end(), imgpt[k1][i].begin());
                }
                //imshow("view", view);
                //int c = waitKey(0) & 255;
                //if( c == 27 || c == 'q' || c == 'Q' )
                //    return -1;
            }
        }
    }
    
    printf("Running calibration ...\n");
    
    run3Calibration(imgpt[0], imgpt[1], imgpt[2], imageSize,
                    boardSize, squareSize, aspectRatio, flags|CV_CALIB_FIX_K4|CV_CALIB_FIX_K5,
                    cameraMatrix[0], distCoeffs[0],
                    cameraMatrix[1], distCoeffs[1],
                    cameraMatrix[2], distCoeffs[2],
                    R12, T12, R13, T13);
    
    fs.open(outputFilename, CV_STORAGE_WRITE);
    
    fs << "cameraMatrix1" << cameraMatrix[0];
    fs << "cameraMatrix2" << cameraMatrix[1];
    fs << "cameraMatrix3" << cameraMatrix[2];
    
    fs << "distCoeffs1" << distCoeffs[0];
    fs << "distCoeffs2" << distCoeffs[1];
    fs << "distCoeffs3" << distCoeffs[2];
    
    fs << "R12" << R12;
    fs << "T12" << T12;
    fs << "R13" << R13;
    fs << "T13" << T13;
    
    fs << "imageWidth" << imageSize.width;
    fs << "imageHeight" << imageSize.height;
    
    Mat Q;
    
    // step 3: find rectification transforms
    double ratio = rectify3Collinear(cameraMatrix[0], distCoeffs[0], cameraMatrix[1],
                                     distCoeffs[1], cameraMatrix[2], distCoeffs[2],
                                     imgpt[0], imgpt[2],
                                     imageSize, R12, T12, R13, T13,
                                     R[0], R[1], R[2], P[0], P[1], P[2], Q, -1.,
                                     imageSize, 0, 0, CV_CALIB_ZERO_DISPARITY);
    Mat map1[3], map2[3];
    
    fs << "R1" << R[0];
    fs << "R2" << R[1];
    fs << "R3" << R[2];
    
    fs << "P1" << P[0];
    fs << "P2" << P[1];
    fs << "P3" << P[2];
    
    fs << "disparityRatio" << ratio;
    fs.release();
    
    printf("Disparity ratio = %g\n", ratio);
    
    for( k = 0; k < 3; k++ )
        initUndistortRectifyMap(cameraMatrix[k], distCoeffs[k], R[k], P[k], imageSize, CV_16SC2, map1[k], map2[k]);
    
    Mat canvas(imageSize.height, imageSize.width*3, CV_8UC3), small_canvas;
    destroyWindow("view");
    canvas = Scalar::all(0);
    
    for( i = 0; i < (int)(imageList.size()/3); i++ )
    {
        canvas = Scalar::all(0);
        for( k = 0; k < 3; k++ )
        {
            int k1 = k == 0 ? 2 : k == 1 ? 0 : 1;
            int k2 = k == 0 ? 1 : k == 1 ? 0 : 2;
            view = imread(imageList[i*3+k], 1);
            
            if(!view.data)
                continue;
            
            Mat rview = canvas.colRange(k2*imageSize.width, (k2+1)*imageSize.width);
            remap(view, rview, map1[k1], map2[k1], CV_INTER_LINEAR);
        }
        printf("%s %s %s\n", imageList[i*3].c_str(), imageList[i*3+1].c_str(), imageList[i*3+2].c_str());
        resize( canvas, small_canvas, Size(1500, 1500/3) );
        for( k = 0; k < small_canvas.rows; k += 16 )
            line(small_canvas, Point(0, k), Point(small_canvas.cols, k), Scalar(0,255,0), 1);
        imshow("rectified", small_canvas);
        int c = waitKey(0);
        if( c == 27 || c == 'q' || c == 'Q' )
            break;
    }
    
    return 0;
}

