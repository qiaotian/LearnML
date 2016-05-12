//
//  main.cpp
//  cluster_dp
//
//  Created by QiaoTian on 1/1/15.
//  Copyright (c) 2015 State Key Laboratory of Intelligent Technology and Systems. All rights reserved.
//

#include <iostream>
#include <opencv/cv.h>
#include <cvaux.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <algorithm>

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000

#define DATA_MAX 40

#define WINDOW_RHO  0.08

using namespace std;
using namespace cv;

enum clusterid {
    UNKNOWN,
    CLUSTER_1,
    CLUSTER_2,
    CLUSTER_3,
    CLUSTER_4,
    CLUSTER_5,
    CLUSTER_6,
    CLUSTER_7,
    CLUSTER_8,
    CLUSTER_9,
    CLUSTER_10
};

enum input_source {
    flame,
    compound,
    d31,
    aggregation,
    jain,
    pathbased,
    r15,
    spiral,
    featuredata,
};

typedef struct Node{
    int id;         // the given index
    float x;        // the given position x
    float y;        // the given position y
    int cluster;    // the given cluster id
    
    Node *neigh;    // the generated neigh
    
    int rho;        // the generated rho
    double delta;   // the generated delta
    int tag;        // the generated cluster id
}Node;

vector<Node> points;    // the imported original points
vector<Node> centers;   // the generated centers
double deltathresh = 6; //
int rhothresh = 27;     //
double dc = 1.40;
input_source datatype = flame;

double getDistance(Node &pt1, Node &pt2) {
    double dist;
    dist = sqrt(pow(pt1.x-pt2.x, 2)+pow(pt1.y-pt2.y, 2));
    return dist;
}
bool importData(vector<Node> &mpoints) {
    Node point;
    
    int counter = 0;
    char filename[50];
    if (datatype == spiral) {
        sprintf(filename, "/Users/qiaotian/Desktop/spiral.txt");
    }
    else if (datatype == compound) {
        sprintf(filename, "/Users/qiaotian/Desktop/Compound.txt");
    }
    else if (datatype == flame) {
        sprintf(filename, "/Users/qiaotian/Desktop/flame.txt");
    }
    else if (datatype == aggregation) {
        sprintf(filename, "/Users/qiaotian/Desktop/Aggregation.txt");
    }
    else if (datatype == featuredata) {
        sprintf(filename, "/Users/qiaotian/Desktop/layer3");
    }
    
    FILE *fp = fopen(filename, "r");
    if (fp) {
        while(fscanf(fp, "%f %f %d", &point.x, &point.y, &point.cluster) != EOF) {
            point.id = counter;
            point.rho = 0;
            point.delta = 0.0f;
            point.tag = UNKNOWN;    // default: unknown cluster id
            point.neigh = NULL;     // default: no neighbor
            mpoints.push_back(point);
            ++counter;
        }
    }
    else {
        cout << "open file error!" << endl;
        return false;
    }
    return true;
}

double getdc(vector<Node> &mpoints, double neighborRateLow, double neighborRateHigh){
    int nSamples = (int)mpoints.size();
    int nLow = neighborRateLow * nSamples * nSamples;
    int nHigh = neighborRateHigh * nSamples * nSamples;
    double dc = 0.0;
    int neighbors = 0;
    //cout<<"nLow = "<<nLow<<", nHigh = "<<nHigh<<endl;
    while(neighbors < nLow || neighbors > nHigh){
        neighbors = 0;
        for(int i = 0; i < nSamples - 1; i++){
            for(int j = i + 1; j < nSamples; j++){
                if(getDistance(mpoints[i], mpoints[j]) <= dc) ++neighbors;
                if(neighbors > nHigh) goto DCPLUS;
            }
        }
    DCPLUS: dc += 0.03;
        cout<<"dc = "<<dc<<", neighbors = "<<neighbors<<endl;
    }
    return dc;
}
void calRho(vector<Node> &mpoints) {
    //dc = getdc(mpoints, 0.016, 0.02);
    int count = (int)mpoints.size();
    for (int i=0; i<count-1; ++i) {
        for (int j=0; j<count; ++j) {
            if (getDistance(mpoints[i], mpoints[j]) < dc) {
                ++mpoints[i].rho;
                ++mpoints[j].rho;
            }
        }
    }
}
bool comparison(Node pt1, Node pt2){
    return (pt1.rho > pt2.rho);
}
void calDelta(vector<Node> &mpoints) {
    // find the max rho index
    int minIndex = 0;
    int count = (int)mpoints.size();
    for (int i=0; i<count; ++i) {
        if (mpoints[i].rho > mpoints[minIndex].rho) {
            minIndex = i;
        }
    }
    sort(mpoints.begin(), mpoints.end(), comparison);
    // find the min dist of every point and max dist of all points
    double maxdist = 0.0f;
    for (int i=0; i<count; ++i) {
        double mindist = 10000;
        for (int j = i-1; j>=0; --j) {
            double temp = getDistance(points[j], points[i]);
            if (temp > maxdist) {
                maxdist = temp;
            }
            if (temp < mindist) {
                mindist = temp;
                mpoints[i].neigh = &mpoints[j];
            }
        }
//        for (int j = 0; j<count; ++j) {
//            if (i == j) {
//                continue;
//            }
//            if (mpoints[i].rho > mpoints[i].rho){
//                double temp = getDistance(points[j], points[i]);
//                if (temp > maxdist) {
//                    maxdist = temp;
//                }
//                if (temp < mindist) {
//                    mindist = temp;
//                    mpoints[i].neigh = &mpoints[j];
//                }
//            }
//        }
        mpoints[i].delta = mindist;
    }
    mpoints[0].delta = maxdist;
}

void getTag(vector<Node> &mpoints, Node *currentNode) {
    if (currentNode->tag == UNKNOWN) {
        if(currentNode->neigh->tag == UNKNOWN) {
            getTag(points, currentNode->neigh);
        }
        currentNode->tag = currentNode->neigh->tag;
    }
}

bool init() {
    // set input datasource
    datatype = aggregation;
    
    // set the threshhold and dc
    if (datatype == flame) {
        deltathresh = 6; //
        rhothresh = 27;     //
        dc = 1.40;
    }
    else if (datatype == compound) {
        deltathresh = 6;
        rhothresh = 25;
        dc = 1.4;
    }
    else if (datatype == jain) {
        
    }
    else if (datatype == aggregation) {
        deltathresh = 6;
        rhothresh = 25;
        dc = 1.4;
    }
    else if (datatype == spiral){
        // 螺旋形曲线调参需要先调节dc观察rho-delta图像变化
        // 确定良好的图像中心以后在确定相应地delta和rho的阈值
        deltathresh = 5;
        rhothresh = 30;
        dc = 3;
    }

    if(!importData(points)) return false;
    return true;
}
void getCenter(vector<Node> &mpoints) {
    int clusterid = 0;
    int count = (int)mpoints.size();
    for (int i=0; i<count; ++i) {
        if (mpoints[i].rho >= rhothresh && mpoints[i].delta >= deltathresh) {
            mpoints[i].tag = ++clusterid;
            centers.push_back(mpoints[i]);
        }
    }
}
void clusterize(vector<Node> &mpoints) {
    int count = (int)mpoints.size();
    for (int i=count-1; i>=0; --i) {
        if (mpoints[i].tag == UNKNOWN) {
            getTag(mpoints, &mpoints[i]);
        }
    }
}

void draw(vector<Node> &mpoints){
    IplImage *image = cvCreateImage(cvSize(WINDOW_WIDTH, WINDOW_HEIGHT), 8, 3);
    cvLine(image, cvPoint(0, WINDOW_HEIGHT/2), cvPoint(WINDOW_WIDTH, WINDOW_HEIGHT/2), CV_RGB(255, 255, 255));
    cvLine(image, cvPoint(WINDOW_WIDTH/2, 0), cvPoint(WINDOW_WIDTH/2, WINDOW_HEIGHT), CV_RGB(255, 255, 255));
    int count = (int)mpoints.size();
    
    
    /* draw center */
    if (datatype == spiral) {
        /* draw original points */
        for (int i = 0; i<count; ++i) {
            cvCircle(image, cvPoint(mpoints[i].x/WINDOW_RHO, WINDOW_HEIGHT/2 - mpoints[i].y/WINDOW_RHO), 3, CV_RGB(255, 255, 255));
        }
        /* draw rho & delta */
        for (int i = 0; i<count ; ++i) {
            cvCircle(image, cvPoint(mpoints[i].rho*8+WINDOW_WIDTH/2, WINDOW_HEIGHT/2 - mpoints[i].delta*10), 3, CV_RGB(0, 255, 0));
        }
        for (int i = 0; i<count; ++i) {
            if (mpoints[i].tag == -1) {
                cvCircle(image, cvPoint(mpoints[i].x/WINDOW_RHO, WINDOW_HEIGHT - mpoints[i].y/WINDOW_RHO), 3, CV_RGB(255, 255, 255));
            }
            else if (mpoints[i].tag == 0){
                cvCircle(image, cvPoint(mpoints[i].x/WINDOW_RHO, WINDOW_HEIGHT - mpoints[i].y/WINDOW_RHO), 3, CV_RGB(255, 0, 0));
            }
            else if (mpoints[i].tag == 1) {
                cvCircle(image, cvPoint(mpoints[i].x/WINDOW_RHO, WINDOW_HEIGHT - mpoints[i].y/WINDOW_RHO), 3, CV_RGB(0, 255, 0));
            }
            else if (mpoints[i].tag == 2) {
                cvCircle(image, cvPoint(mpoints[i].x/WINDOW_RHO, WINDOW_HEIGHT - mpoints[i].y/WINDOW_RHO), 3, CV_RGB(0, 0, 255));
            }
            else if (mpoints[i].tag == 3) {
                cvCircle(image, cvPoint(mpoints[i].x/WINDOW_RHO, WINDOW_HEIGHT - mpoints[i].y/WINDOW_RHO), 3, CV_RGB(255, 255, 0));
            }
            else if (mpoints[i].tag == 4) {
                cvCircle(image, cvPoint(mpoints[i].x/WINDOW_RHO, WINDOW_HEIGHT - mpoints[i].y/WINDOW_RHO), 3, CV_RGB(255, 0, 255));
            }
            else {
                cvCircle(image, cvPoint(mpoints[i].x/WINDOW_RHO, WINDOW_HEIGHT - mpoints[i].y/WINDOW_RHO), 3, CV_RGB(0, 255, 255));
            }
        }
        for(int i = 0; i<centers.size(); ++i){
            cvCircle(image, cvPoint(centers[i].x/WINDOW_RHO, WINDOW_HEIGHT - centers[i].y/WINDOW_RHO), 10, CV_RGB(150, 255, 45));
            cvCircle(image, cvPoint(centers[i].rho*8+WINDOW_WIDTH/2, WINDOW_HEIGHT/2 - centers[i].delta*10), 10, CV_RGB(255, 0, 0));
        }
    }
    else if (datatype == flame) {
        /* draw original points */
        for (int i = 0; i<count; ++i) {
            cvCircle(image, cvPoint(mpoints[i].x*1.5/WINDOW_RHO+100, WINDOW_HEIGHT/2 - mpoints[i].y*1.5/WINDOW_RHO+150), 3, CV_RGB(255, 255, 255));
        }
        /* draw rho & delta */
        for (int i = 0; i<count ; ++i) {
            cvCircle(image, cvPoint(mpoints[i].rho*20+WINDOW_WIDTH/2, WINDOW_HEIGHT/2 - mpoints[i].delta*160), 3, CV_RGB(0, 255, 0));
        }
        for (int i = 0; i<count; ++i) {
            if (mpoints[i].tag == -1) {
                cvCircle(image, cvPoint(mpoints[i].x*1.5/WINDOW_RHO+100, WINDOW_HEIGHT - mpoints[i].y*1.5/WINDOW_RHO+150), 3, CV_RGB(255, 255, 255));
            }
            else if (mpoints[i].tag == 0){
                cvCircle(image, cvPoint(mpoints[i].x*1.5/WINDOW_RHO+100, WINDOW_HEIGHT - mpoints[i].y*1.5/WINDOW_RHO+150), 3, CV_RGB(255, 0, 0));
            }
            else if (mpoints[i].tag == 1) {
                cvCircle(image, cvPoint(mpoints[i].x*1.5/WINDOW_RHO+100, WINDOW_HEIGHT - mpoints[i].y*1.5/WINDOW_RHO+150), 3, CV_RGB(0, 255, 0));
            }
            else if (mpoints[i].tag == 2) {
                cvCircle(image, cvPoint(mpoints[i].x*1.5/WINDOW_RHO+100, WINDOW_HEIGHT - mpoints[i].y*1.5/WINDOW_RHO+150), 3, CV_RGB(0, 0, 255));
            }
            else if (mpoints[i].tag == 3) {
                cvCircle(image, cvPoint(mpoints[i].x*1.5/WINDOW_RHO+100, WINDOW_HEIGHT - mpoints[i].y*1.5/WINDOW_RHO+150), 3, CV_RGB(255, 255, 0));
            } else {
                cvCircle(image, cvPoint(mpoints[i].x*1.5/WINDOW_RHO+100, WINDOW_HEIGHT - mpoints[i].y*1.5/WINDOW_RHO+150), 3, CV_RGB(0, 255, 255));
            }
            
//            if (mpoints[i].id == 2) {
//                cvCircle(image, cvPoint(mpoints[i].x*1.5/WINDOW_RHO+100, WINDOW_HEIGHT - mpoints[i].y*1.5/WINDOW_RHO+150), 10, CV_RGB(0, 255, 255));
//            }
            
        }
        for(int i = 0; i<centers.size(); ++i){
            //cvCircle(image, cvPoint(centers[i].rho*20+WINDOW_WIDTH/2, WINDOW_HEIGHT/2 - centers[i].delta*20), 10, CV_RGB(0, 255, 0));
            cvCircle(image, cvPoint(centers[i].x*1.5/WINDOW_RHO+100, WINDOW_HEIGHT - centers[i].y*1.5/WINDOW_RHO+150), 10, CV_RGB(255, 255, 0));
        }
        
    } else if (datatype == compound){
        /* draw original points */
        for (int i = 0; i<count; ++i) {
            cvCircle(image, cvPoint(mpoints[i].x/WINDOW_RHO-60, WINDOW_HEIGHT/2 - mpoints[i].y/WINDOW_RHO-100), 3, CV_RGB(255, 255, 255));
        }
        /* draw rho & delta */
        for (int i = 0; i<count ; ++i) {
            cvCircle(image, cvPoint(mpoints[i].rho*15+WINDOW_WIDTH/2, WINDOW_HEIGHT/2 - mpoints[i].delta*15), 3, CV_RGB(0, 255, 0));
        }
        for (int i = 0; i<count; ++i) {
            if (mpoints[i].tag == -1) {
                cvCircle(image, cvPoint(mpoints[i].x/WINDOW_RHO-60, WINDOW_HEIGHT - mpoints[i].y/WINDOW_RHO-100), 3, CV_RGB(255, 255, 255));
            }
            else if (mpoints[i].tag == 0){
                cvCircle(image, cvPoint(mpoints[i].x/WINDOW_RHO-60, WINDOW_HEIGHT - mpoints[i].y/WINDOW_RHO-100), 3, CV_RGB(255, 0, 0));
            }
            else if (mpoints[i].tag == 1) {
                cvCircle(image, cvPoint(mpoints[i].x/WINDOW_RHO-60, WINDOW_HEIGHT - mpoints[i].y/WINDOW_RHO-100), 3, CV_RGB(0, 255, 0));
            }
            else if (mpoints[i].tag == 2) {
                cvCircle(image, cvPoint(mpoints[i].x/WINDOW_RHO-60, WINDOW_HEIGHT - mpoints[i].y/WINDOW_RHO-100), 3, CV_RGB(0, 0, 255));
            }
            else if (mpoints[i].tag == 3) {
                cvCircle(image, cvPoint(mpoints[i].x/WINDOW_RHO-60, WINDOW_HEIGHT - mpoints[i].y/WINDOW_RHO-100), 3, CV_RGB(255, 255, 0));
            } else {
                cvCircle(image, cvPoint(mpoints[i].x/WINDOW_RHO-60, WINDOW_HEIGHT - mpoints[i].y/WINDOW_RHO-100), 3, CV_RGB(0, 255, 255));
            }
        }
        /* draw center */
        for(int i = 0; i<centers.size(); ++i){
            cvCircle(image, cvPoint(centers[i].x/WINDOW_RHO-60, WINDOW_HEIGHT - centers[i].y/WINDOW_RHO-100), 10, CV_RGB(150, 255, 45));
        }

    }
    else if (datatype == aggregation){
        /* draw original points */
        for (int i = 0; i<count; ++i) {
            cvCircle(image, cvPoint(mpoints[i].x/WINDOW_RHO, WINDOW_HEIGHT/2 - mpoints[i].y/WINDOW_RHO-80), 3, CV_RGB(255, 255, 255));
        }
        /* draw rho & delta */
        for (int i = 0; i<count ; ++i) {
            cvCircle(image, cvPoint(mpoints[i].rho*10+WINDOW_WIDTH/2, WINDOW_HEIGHT/2 - mpoints[i].delta*30), 3, CV_RGB(0, 255, 0));
        }
        for (int i = 0; i<count; ++i) {
            if (mpoints[i].tag == -1) {
                cvCircle(image, cvPoint(mpoints[i].x/WINDOW_RHO, WINDOW_HEIGHT - mpoints[i].y/WINDOW_RHO-80), 3, CV_RGB(255, 255, 255));
            }
            else if (mpoints[i].tag == 0){
                cvCircle(image, cvPoint(mpoints[i].x/WINDOW_RHO, WINDOW_HEIGHT - mpoints[i].y/WINDOW_RHO-80), 3, CV_RGB(255, 0, 0));
            }
            else if (mpoints[i].tag == 1) {
                cvCircle(image, cvPoint(mpoints[i].x/WINDOW_RHO, WINDOW_HEIGHT - mpoints[i].y/WINDOW_RHO-80), 3, CV_RGB(0, 255, 0));
            }
            else if (mpoints[i].tag == 2) {
                cvCircle(image, cvPoint(mpoints[i].x/WINDOW_RHO, WINDOW_HEIGHT - mpoints[i].y/WINDOW_RHO-80), 3, CV_RGB(0, 0, 255));
            }
            else if (mpoints[i].tag == 3) {
                cvCircle(image, cvPoint(mpoints[i].x/WINDOW_RHO, WINDOW_HEIGHT - mpoints[i].y/WINDOW_RHO-80), 3, CV_RGB(255, 255, 0));
            }
            else if (mpoints[i].tag == 4) {
                cvCircle(image, cvPoint(mpoints[i].x/WINDOW_RHO, WINDOW_HEIGHT - mpoints[i].y/WINDOW_RHO-80), 3, CV_RGB(0, 255, 255));
            }
            else if (mpoints[i].tag == 5) {
                cvCircle(image, cvPoint(mpoints[i].x/WINDOW_RHO, WINDOW_HEIGHT - mpoints[i].y/WINDOW_RHO-80), 3, CV_RGB(135, 135, 15));
            }
            else if (mpoints[i].tag == 6) {
                cvCircle(image, cvPoint(mpoints[i].x/WINDOW_RHO, WINDOW_HEIGHT - mpoints[i].y/WINDOW_RHO-80), 3, CV_RGB(255, 72, 77));
            }
            else if (mpoints[i].tag == 7) {
                cvCircle(image, cvPoint(mpoints[i].x/WINDOW_RHO, WINDOW_HEIGHT - mpoints[i].y/WINDOW_RHO-80), 3, CV_RGB(201, 94, 131));
            }
            else if (mpoints[i].tag == 8) {
                cvCircle(image, cvPoint(mpoints[i].x/WINDOW_RHO, WINDOW_HEIGHT - mpoints[i].y/WINDOW_RHO-80), 3, CV_RGB(251, 178, 14));
            }
        }
        /* draw center */
        for(int i = 0; i<centers.size(); ++i){
            cvCircle(image, cvPoint(centers[i].x/WINDOW_RHO, WINDOW_HEIGHT - centers[i].y/WINDOW_RHO-80), 10, CV_RGB(150, 255, 45));
            cvCircle(image, cvPoint(centers[i].rho*10+WINDOW_WIDTH/2, WINDOW_HEIGHT/2 - centers[i].delta*30), 10, CV_RGB(255, 0, 0));
        }
        
    }
    cvShowImage("window", image);
}

bool load_feature_data(char *filepath, int feature_count, int column, int row, vector<float> &rgb){
    FILE *fp = fopen(filepath, "r");
    if (fp) {
        fscanf(fp, "%d %d %d", &feature_count, &column, &row);
        float temp=0;
        while (fscanf(fp, "%f", &temp)!=EOF) {
            rgb.push_back(temp);
        }
        return true;
    }
    
    return false;
}

int main()
{
//    if(!init()) return 0;
//    calRho(points);
//    calDelta(points);
//    getCenter(points);      // get the centers of whole clusters
//    clusterize(points);     // assign every point to a cluster
//    
//    for (int i = 0; i < points.size(); ++i) {
//        cout << "rho: " << points[i].rho << " " << "delta: " << points[i].delta << endl;
//    }
//    
//    draw(points);
    
    vector<float> rgb;
    int column, row, feature_count = 0;
    char filepath[100];
    sprintf(filepath, "/Users/qiaotian/Desktop/layer1");
    if (load_feature_data(filepath, feature_count, column, row, rgb)) {
        IplImage *image = cvCreateImage(cvSize(WINDOW_WIDTH, WINDOW_HEIGHT), 8, 1);
        
    }
    
    cvWaitKey();
    return 0;
}

