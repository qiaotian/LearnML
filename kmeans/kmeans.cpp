#include <iostream>
#include <opencv/cv.h>
#include <cvaux.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <algorithm>
#include <fstream>
#include <math.h>

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000
#define DATA_MAX 40
#define WINDOW_RHO  0.08
#define k 3


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
    
};

input_source datatype;

///存放元组的属性信息
struct Tuple{
    float attr1;
    float attr2;
    int cluster;
};

vector<Tuple> clusters[k];

///计算两个元组间的欧几里距离
float getDistXY(Tuple t1, Tuple t2)
{
    return sqrt((t1.attr1 - t2.attr1) * (t1.attr1 - t2.attr1) + (t1.attr2 - t2.attr2) * (t1.attr2 - t2.attr2));
}

///根据质心，决定当前元组属于哪个簇
int clusterOfTuple(Tuple means[],Tuple tuple){
    float dist=getDistXY(means[0],tuple);
    float tmp;
    int label=0;//标示属于哪一个簇
    for(int i=1; i<k; i++){
        tmp=getDistXY(means[i],tuple);
        if(tmp<dist) {dist=tmp;label=i;}
    }
    return label;
}
///获得给定簇集的平方误差
float getVar(vector<Tuple> clusters[],Tuple means[]){
    float var = 0;
    for (int i = 0; i < k; i++)
    {
        vector<Tuple> t = clusters[i];
        for (int j = 0; j< t.size(); j++)
        {
            var += getDistXY(t[j],means[i]);
        }
    }
    //cout<<"sum:"<<sum<<endl;
    return var;
    
}
///获得当前簇的均值（质心）
Tuple getMeans(vector<Tuple> cluster){
    
    int num = (int)cluster.size();
    double meansX = 0, meansY = 0;
    Tuple t;
    for (int i = 0; i < num; i++)
    {
        meansX += cluster[i].attr1;
        meansY += cluster[i].attr2;
    }
    t.attr1 = meansX / num;
    t.attr2 = meansY / num;
    return t;
}

void KMeans(vector<Tuple> tuples){
    Tuple means[k];
    int i=0;
    ///默认一开始将前K个元组的值作为k个簇的质心（均值）
    for(i=0;i<k;i++){
        means[i].attr1=tuples[i].attr1;
        means[i].attr2=tuples[i].attr2;
    }
    int lable=0;
    ///根据默认的质心给簇赋值
    for(i=0;i!=tuples.size();++i){
        lable=clusterOfTuple(means,tuples[i]);
        clusters[lable].push_back(tuples[i]);
    }
    ///输出刚开始的簇
    for(lable=0;lable<3;lable++){
        cout<<"第"<<lable+1<<"个簇："<<endl;
        vector<Tuple> t = clusters[lable];
        for (i = 0; i< t.size(); i++)
        {
            cout<<"("<<t[i].attr1<<","<<t[i].attr2<<")"<<"   ";
        }
        cout<<endl;
    }
    float oldVar=-1;
    float newVar=getVar(clusters,means);
    while(abs(newVar - oldVar) >= 1) //当新旧函数值相差不到1即准则函数值不发生明显变化时，算法终止
    {
        
        for (i = 0; i < k; i++) //更新每个簇的中心点
        {
            means[i] = getMeans(clusters[i]);
            //cout<<"means["<<i<<"]:"<<means[i].attr1<<"  "<<means[i].attr2<<endl;
        }
        oldVar = newVar;
        newVar = getVar(clusters,means); //计算新的准则函数值
        for (i = 0; i < k; i++) //清空每个簇
        {
            clusters[i].clear();
        }
        ///根据新的质心获得新的簇
        for(i=0;i!=tuples.size();++i){
            lable=clusterOfTuple(means,tuples[i]);
            clusters[lable].push_back(tuples[i]);
        }
        ///输出当前的簇
        for(lable=0;lable<3;lable++){
            cout<<"第"<<lable+1<<"个簇："<<endl;
            vector<Tuple> t = clusters[lable];
            for (i = 0; i< t.size(); i++)
            {
                cout<<"("<<t[i].attr1<<","<<t[i].attr2<<")"<<"   ";
            }
            cout<<endl;
        }
    }
}

void draw(vector<Tuple> *clusterArray){
    IplImage *image = cvCreateImage(cvSize(WINDOW_WIDTH, WINDOW_HEIGHT), 8, 3);
    cvLine(image, cvPoint(0, WINDOW_HEIGHT/2), cvPoint(WINDOW_WIDTH, WINDOW_HEIGHT/2), CV_RGB(255, 255, 255));
    cvLine(image, cvPoint(WINDOW_WIDTH/2, 0), cvPoint(WINDOW_WIDTH/2, WINDOW_HEIGHT), CV_RGB(255, 255, 255));
    //int count = (int)clusters.size();
    
    /* draw center */
    if (datatype == spiral) {
        /* draw original points */
        for (int i = 0; i<k; ++i) {
            int size = (int)clusters[i].size();
            for(int j = 0; j<size; ++j){
                cvCircle(image, cvPoint(clusterArray[i][j].attr1/WINDOW_RHO, WINDOW_HEIGHT/2 - clusterArray[i][j].attr2/WINDOW_RHO), 3, CV_RGB(255, 255, 255));
            }
        }
        
        for (int i = 0; i<k; ++i) {
            int size = (int)clusters[i].size();
            for(int j = 0; j<size; ++j){
                if (i == -1) {
                    cvCircle(image, cvPoint(clusterArray[i][j].attr1/WINDOW_RHO, WINDOW_HEIGHT - clusterArray[i][j].attr2/WINDOW_RHO), 3, CV_RGB(255, 255, 255));
                }
                else if (i == 0){
                    cvCircle(image, cvPoint(clusterArray[i][j].attr1/WINDOW_RHO, WINDOW_HEIGHT - clusterArray[i][j].attr2/WINDOW_RHO), 3, CV_RGB(255, 0, 0));
                }
                else if (i == 1) {
                    cvCircle(image, cvPoint(clusterArray[i][j].attr1/WINDOW_RHO, WINDOW_HEIGHT - clusterArray[i][j].attr2/WINDOW_RHO), 3, CV_RGB(0, 255, 0));
                }
                else if (i == 2) {
                    cvCircle(image, cvPoint(clusterArray[i][j].attr1/WINDOW_RHO, WINDOW_HEIGHT - clusterArray[i][j].attr2/WINDOW_RHO), 3, CV_RGB(0, 0, 255));
                }
                else if (i == 3) {
                    cvCircle(image, cvPoint(clusterArray[i][j].attr1/WINDOW_RHO, WINDOW_HEIGHT - clusterArray[i][j].attr2/WINDOW_RHO), 3, CV_RGB(255, 255, 0));
                }
                else if (i == 4) {
                    cvCircle(image, cvPoint(clusterArray[i][j].attr1/WINDOW_RHO, WINDOW_HEIGHT - clusterArray[i][j].attr2/WINDOW_RHO), 3, CV_RGB(255, 0, 255));
                }
                else {
                    cvCircle(image, cvPoint(clusterArray[i][j].attr1/WINDOW_RHO, WINDOW_HEIGHT - clusterArray[i][j].attr2/WINDOW_RHO), 3, CV_RGB(0, 255, 255));
                }
            }
        }
    }
    else if (datatype == flame) {
        /* draw original points */
        for (int i = 0; i<k; ++i) {
            int size = (int)clusters[i].size();
            for(int j = 0; j<size; ++j){
                cvCircle(image, cvPoint(clusterArray[i][j].attr1*1.5/WINDOW_RHO+100, WINDOW_HEIGHT/2 - clusterArray[i][j].attr2*1.5/WINDOW_RHO+150), 3, CV_RGB(255, 255, 255));
            }
        }
        for (int i = 0; i<k; ++i) {
            int size = (int)clusters[i].size();
            for(int j = 0; j<size; ++j){
                if (i == -1) {
                    cvCircle(image, cvPoint(clusterArray[i][j].attr1*1.5/WINDOW_RHO+100, WINDOW_HEIGHT - clusterArray[i][j].attr2*1.5/WINDOW_RHO+150), 3, CV_RGB(255, 255, 255));
                }
                else if (i == 0){
                    cvCircle(image, cvPoint(clusterArray[i][j].attr1*1.5/WINDOW_RHO+100, WINDOW_HEIGHT - clusterArray[i][j].attr2*1.5/WINDOW_RHO+150), 3, CV_RGB(255, 0, 0));
                }
                else if (i == 1) {
                    cvCircle(image, cvPoint(clusterArray[i][j].attr1*1.5/WINDOW_RHO+100, WINDOW_HEIGHT - clusterArray[i][j].attr2*1.5/WINDOW_RHO+150), 3, CV_RGB(0, 255, 0));
                }
                else if (i == 2) {
                    cvCircle(image, cvPoint(clusterArray[i][j].attr1*1.5/WINDOW_RHO+100, WINDOW_HEIGHT - clusterArray[i][j].attr2*1.5/WINDOW_RHO+150), 3, CV_RGB(0, 0, 255));
                }
                else if (i == 3) {
                    cvCircle(image, cvPoint(clusterArray[i][j].attr1*1.5/WINDOW_RHO+100, WINDOW_HEIGHT - clusterArray[i][j].attr2*1.5/WINDOW_RHO+150), 3, CV_RGB(255, 255, 0));
                } else {
                    cvCircle(image, cvPoint(clusterArray[i][j].attr1*1.5/WINDOW_RHO+100, WINDOW_HEIGHT - clusterArray[i][j].attr2*1.5/WINDOW_RHO+150), 3, CV_RGB(0, 255, 255));
                }
            }
            
            //            if (clusterArray[i][j].id == 2) {
            //                cvCircle(image, cvPoint(clusterArray[i][j].attr1*1.5/WINDOW_RHO+100, WINDOW_HEIGHT - clusterArray[i][j].attr2*1.5/WINDOW_RHO+150), 10, CV_RGB(0, 255, 255));
            //            }
            
        }
        //        for(int i = 0; i<centers.size(); ++i){
        //            //cvCircle(image, cvPoint(centers[i].rho*20+WINDOW_WIDTH/2, WINDOW_HEIGHT/2 - centers[i].delta*20), 10, CV_RGB(0, 255, 0));
        //            cvCircle(image, cvPoint(centers[i].attr1*1.5/WINDOW_RHO+100, WINDOW_HEIGHT - centers[i].attr2*1.5/WINDOW_RHO+150), 10, CV_RGB(255, 255, 0));
        //        }
        
    }
    else if (datatype == compound){
        /* draw original points */
        for (int i = 0; i<k; ++i) {
            int size = (int)clusters[i].size();
            for(int j = 0; j<size; ++j){
                cvCircle(image, cvPoint(clusterArray[i][j].attr1/WINDOW_RHO-60, WINDOW_HEIGHT/2 - clusterArray[i][j].attr2/WINDOW_RHO-100), 3, CV_RGB(255, 255, 255));
            }
        }
        //        /* draw rho & delta */
        //        for (int i = 0; i<k ; ++i) {
        //            cvCircle(image, cvPoint(clusterArray[i][j].rho*15+WINDOW_WIDTH/2, WINDOW_HEIGHT/2 - clusterArray[i][j].delta*15), 3, CV_RGB(0, 255, 0));
        //        }
        for (int i = 0; i<k; ++i) {
            int size = (int)clusters[i].size();
            for(int j = 0; j<size; ++j){
                if (i == -1) {
                    cvCircle(image, cvPoint(clusterArray[i][j].attr1/WINDOW_RHO-60, WINDOW_HEIGHT - clusterArray[i][j].attr2/WINDOW_RHO-100), 3, CV_RGB(255, 255, 255));
                }
                else if (i == 0){
                    cvCircle(image, cvPoint(clusterArray[i][j].attr1/WINDOW_RHO-60, WINDOW_HEIGHT - clusterArray[i][j].attr2/WINDOW_RHO-100), 3, CV_RGB(255, 0, 0));
                }
                else if (i == 1) {
                    cvCircle(image, cvPoint(clusterArray[i][j].attr1/WINDOW_RHO-60, WINDOW_HEIGHT - clusterArray[i][j].attr2/WINDOW_RHO-100), 3, CV_RGB(0, 255, 0));
                }
                else if (i == 2) {
                    cvCircle(image, cvPoint(clusterArray[i][j].attr1/WINDOW_RHO-60, WINDOW_HEIGHT - clusterArray[i][j].attr2/WINDOW_RHO-100), 3, CV_RGB(0, 0, 255));
                }
                else if (i == 3) {
                    cvCircle(image, cvPoint(clusterArray[i][j].attr1/WINDOW_RHO-60, WINDOW_HEIGHT - clusterArray[i][j].attr2/WINDOW_RHO-100), 3, CV_RGB(255, 255, 0));
                } else {
                    cvCircle(image, cvPoint(clusterArray[i][j].attr1/WINDOW_RHO-60, WINDOW_HEIGHT - clusterArray[i][j].attr2/WINDOW_RHO-100), 3, CV_RGB(0, 255, 255));
                }
            }
        }
        //        /* draw center */
        //        for(int i = 0; i<centers.size(); ++i){
        //            cvCircle(image, cvPoint(centers[i].attr1/WINDOW_RHO-60, WINDOW_HEIGHT - centers[i].attr2/WINDOW_RHO-100), 10, CV_RGB(150, 255, 45));
        //        }
        
    }
    else if (datatype == aggregation){
        /* draw original points */
        for (int i = 0; i<k; ++i) {
            int size = (int)clusters[i].size();
            for(int j = 0; j<size; ++j){
                cvCircle(image, cvPoint(clusterArray[i][j].attr1/WINDOW_RHO, WINDOW_HEIGHT/2 - clusterArray[i][j].attr2/WINDOW_RHO-80), 3, CV_RGB(255, 255, 255));
            }
        }
        //        /* draw rho & delta */
        //        for (int i = 0; i<k ; ++i) {
        //            cvCircle(image, cvPoint(clusterArray[i][j].rho*10+WINDOW_WIDTH/2, WINDOW_HEIGHT/2 - clusterArray[i][j].delta*30), 3, CV_RGB(0, 255, 0));
        //        }
        for (int i = 0; i<k; ++i) {
            int size = (int)clusters[i].size();
            for(int j = 0; j<size; ++j){
                if (i == -1) {
                    cvCircle(image, cvPoint(clusterArray[i][j].attr1/WINDOW_RHO, WINDOW_HEIGHT - clusterArray[i][j].attr2/WINDOW_RHO-80), 3, CV_RGB(255, 255, 255));
                }
                else if (i == 0){
                    cvCircle(image, cvPoint(clusterArray[i][j].attr1/WINDOW_RHO, WINDOW_HEIGHT - clusterArray[i][j].attr2/WINDOW_RHO-80), 3, CV_RGB(255, 0, 0));
                }
                else if (i == 1) {
                    cvCircle(image, cvPoint(clusterArray[i][j].attr1/WINDOW_RHO, WINDOW_HEIGHT - clusterArray[i][j].attr2/WINDOW_RHO-80), 3, CV_RGB(0, 255, 0));
                }
                else if (i == 2) {
                    cvCircle(image, cvPoint(clusterArray[i][j].attr1/WINDOW_RHO, WINDOW_HEIGHT - clusterArray[i][j].attr2/WINDOW_RHO-80), 3, CV_RGB(0, 0, 255));
                }
                else if (i == 3) {
                    cvCircle(image, cvPoint(clusterArray[i][j].attr1/WINDOW_RHO, WINDOW_HEIGHT - clusterArray[i][j].attr2/WINDOW_RHO-80), 3, CV_RGB(255, 255, 0));
                }
                else if (i == 4) {
                    cvCircle(image, cvPoint(clusterArray[i][j].attr1/WINDOW_RHO, WINDOW_HEIGHT - clusterArray[i][j].attr2/WINDOW_RHO-80), 3, CV_RGB(0, 255, 255));
                }
                else if (i == 5) {
                    cvCircle(image, cvPoint(clusterArray[i][j].attr1/WINDOW_RHO, WINDOW_HEIGHT - clusterArray[i][j].attr2/WINDOW_RHO-80), 3, CV_RGB(135, 135, 15));
                }
                else if (i == 6) {
                    cvCircle(image, cvPoint(clusterArray[i][j].attr1/WINDOW_RHO, WINDOW_HEIGHT - clusterArray[i][j].attr2/WINDOW_RHO-80), 3, CV_RGB(255, 72, 77));
                }
                else if (i == 7) {
                    cvCircle(image, cvPoint(clusterArray[i][j].attr1/WINDOW_RHO, WINDOW_HEIGHT - clusterArray[i][j].attr2/WINDOW_RHO-80), 3, CV_RGB(201, 94, 131));
                }
                else if (i == 8) {
                    cvCircle(image, cvPoint(clusterArray[i][j].attr1/WINDOW_RHO, WINDOW_HEIGHT - clusterArray[i][j].attr2/WINDOW_RHO-80), 3, CV_RGB(251, 178, 14));
                }
            }
        }
        //        /* draw center */
        //        for(int i = 0; i<centers.size(); ++i){
        //            cvCircle(image, cvPoint(centers[i].attr1/WINDOW_RHO, WINDOW_HEIGHT - centers[i].attr2/WINDOW_RHO-80), 10, CV_RGB(150, 255, 45));
        //            cvCircle(image, cvPoint(centers[i].rho*10+WINDOW_WIDTH/2, WINDOW_HEIGHT/2 - centers[i].delta*30), 10, CV_RGB(255, 0, 0));
        //        }
        
    }
    
    cvShowImage("window", image);
}

int main(){
    datatype = spiral;
    
    int count=0;
    vector<Tuple> tuples;
    Tuple tuple;
    char filename[256];
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
    FILE *fp = fopen(filename, "r");
    if (fp) {
        while(fscanf(fp, "%f %f %d", &tuple.attr1, &tuple.attr2, &tuple.cluster) != EOF) {
            tuples.push_back(tuple);
            ++count;
        }
    }
    else {
        cout << "open file error!" << endl;
    }
    //int k;
    //cout<<"请输入期望的簇的个数："
    //cin>>k;
    //cout<<endl;
    
    ///输出文件中的元组信息
    for(vector<Tuple>::size_type ix=0;ix!=tuples.size();++ix)
        cout<<"("<<tuples[ix].attr1<<","<<tuples[ix].attr2<<")"<<"    ";
    cout<<endl;
    KMeans(tuples);
    draw(clusters);
    cvWaitKey();
    return 0;
}
