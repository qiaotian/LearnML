//
//  main.cpp
//  FileSize
//
//  Created by QiaoTian on 1/7/15.
//  Copyright (c) 2015 State Key Laboratory of Intelligent Technology and Systems. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include "time.h"

using namespace std;

int main()
{
    clock_t start, end;

    
    FILE* fp = NULL;
    long long newPos = 0;

    
    
    fp = fopen("/Users/qiaotian/Desktop/The.Interview.2014.WEB-DL.XviD.MP3-RARBG/The.Interview.2014.WEB-DL.XviD.MP3-RARBG.avi", "rb");
    if (fp == NULL)
    {
        cout << "can't open file" << endl;
    }
    
    //fseek(fp, 0, SEEK_END); //定位到文件末
    //newPos = (long long)ftell(fp); //文件长度
    //start = time(NULL);
    while (1) {
        start = clock();
        fflush(fp);
        
        //prePos = newPos;
        fgetpos(fp, &newPos);
        printf("old pos is %lld\n", newPos);
        fseek(fp, 0, SEEK_END);
        printf("old pos is %lld\n", newPos);
        fgetpos(fp, &newPos);

        newPos = (long long)ftell(fp);
        
        end = clock();
        //printf("new position is %lld\n", newPos);

        //printf("time=%f\n", difftime(end,start));
    }
    

    

    
    //end = time(NULL);

    
    //cout << "file len = " << nFileLen << endl;
    return 0;
}