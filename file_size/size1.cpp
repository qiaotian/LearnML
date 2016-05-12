//
//  main.cpp
//  calculate_file_size
//
//  Created by QiaoTian on 1/5/15.
//  Copyright (c) 2015 State Key Laboratory of Intelligent Technology and Systems. All rights reserved.
//

#include <iostream>
#include <pthread.h>
#include <sys/stat.h>
#include <stdio.h>

unsigned long get_file_size(const char *path)
{
    unsigned long filesize = -1;
    struct stat statbuff;
    if(stat(path, &statbuff) < 0){
        return filesize;
    }else{
        filesize = statbuff.st_size;
    }
    return filesize;
}

void writefile(const char *path) {
    //int i = 100;
    FILE *fp = fopen(path, "r");
    while (1) {
        fputc('a', fp);
    }
}

int main(int argc, const char * argv[]) {
    // insert code here...
    //pthread_t *newThread = pthread_create(<#pthread_t *#>, <#const pthread_attr_t *#>, <#void *(*)(void *)#>, <#void *#>)
    
    std::cout << "Hello, World!\n";
    return 0;
}
