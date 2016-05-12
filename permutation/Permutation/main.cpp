////
////  main.cpp
////  Permutation
////
////  Created by QiaoTian on 12/9/14.
////  Copyright (c) 2014 State Key Laboratory of Intelligent Technology and Systems. All rights reserved.
////
//
////
////  main.cpp
////  Permutation
////
////  Created by QiaoTian on 12/9/14.
////  Copyright (c) 2014 State Key Laboratory of Intelligent Technology and Systems. All rights reserved.
////
//
//#include <iostream>
//#include <vector>
//#include <string>
//
//using namespace std;
//
//int total = 0;
//struct Element{
//    int data;
//    int direction;
//};
//
//bool FindActive(Element array[],int length,vector<int>& vec)
//{
//    vec.clear();
//    for(int i = 0;i < length;i++)
//    {
//        if(array[i].direction == 0 && (i-1 >= 0)&& array[i].data > array[i-1].data )
//        {
//            vec.push_back(i);
//        }
//        else if (array[i].direction == 1 && (i+1 < length)&& array[i].data > array[i+1].data)
//        {
//            vec.push_back(i);
//        }
//    }
//    if (vec.size() != 0)
//    {
//        return true;
//    }
//    return false;
//}
//
//int FindBiggestActive(Element array[],int length,vector<int>& vec,int &biggestNum)
//{
//    if (vec.size()==0)
//    {
//        return -1;
//    }
//    int biggest = vec[0];
//    for (int i = 0; i < vec.size();i++)
//    {
//        if(array[vec[i]].data > array[biggest].data )
//        {
//            biggest = vec[i];
//        }
//    }
//    biggestNum = array[biggest].data ;
//    return biggest;
//}
//void SwapE( Element array[],int i ,int j)
//{
//    Element temp = {0,0};
//    temp = array[i];
//    array[i] = array[j];
//    array[j] = temp;
//}
//
//void  Swap(Element array[],int length,int biggest)
//{
//    int near;
//    if(array[biggest].direction == 0)
//    {
//        near = biggest - 1;
//    }
//    else{
//        near = biggest + 1;
//    }
//    if (near < 0 || near > length)
//    {
//        return ;
//    }
//    
//    SwapE(array,near,biggest);
//}
//
//void  ChangeDirection(Element array[],int length,int biggestNum)
//{
//    for (int i = 0 ; i < length ; i++)
//    {
//        if(array[i].data > biggestNum)
//        {
//            array[i].direction = !array[i].direction;
//        }
//    }
//}
//
//void InitElement(int data[],Element* elem,int length)
//{
//    for (int i = 0; i < length ; i++)
//    {
//        elem->data = data[i];
//        elem->direction = 0;
//        elem++;
//    }
//}
//
//void printElement(Element array[],int length)
//{
//    for (int i = 0 ; i < length ; i++)
//    {
//        cout << array[i].data << " ";
//        
//    }
//    cout << "number : " << ++total << endl;
//    cout << endl;
//}
//
//void swapA(int A[],int i, int j);
//int partition(int A[],int p,int r);
//void quicksort(int A[],int p,int r);
//
//bool  permutation(Element array[],int length,vector<int>& vec)
//{
//    if (!FindActive(array,length,vec))
//    {
//        return false;
//    }
//    int biggestNUM;
//    int biggest = FindBiggestActive(array,length,vec,biggestNUM);
//    Swap(array,length,biggest);
//    ChangeDirection(array,length,biggestNUM);
//    printElement(array,length);
//    return true;
//}
//
//void swapA(int A[],int i, int j)
//{
//    int temp = A[i];
//    A[i] = A[j];
//    A[j] = temp;
//}
//int partition(int A[],int p,int r)
//{
//    int x = A[r-1];
//    int i = p - 1;
//    
//    for(int j = p ; j < r-1; j++)
//        if(A[j]<=x){
//            i++;
//            swapA(A,i,j);
//        }
//    swapA(A,i+1,r-1);
//    return i+1;
//}
//void quicksort(int A[],int p,int r)
//{
//    if(p<r)
//    {
//        int q = partition(A,p,r);
//        quicksort(A,p,q-1);
//        quicksort(A,q+1,r);
//    }
//}
//
//
//int  main()
//{
//    //int data[] = {1,2,3,4,5};
//    string str;
//    cin >> str;
//    
//    long count = str.length();
//    int data[count];
//    for (int i =0; i<count; ++i) {
//        //data[i]=string[i]-48;
//        data[i] = str[i]-48;
//    }
//    
//    vector<int> vec;
//    int length = (int)sizeof(data)/sizeof(int);
//    quicksort(data,0,length);
//    
//    Element* array = new Element[length];
//    InitElement(data,array,length);
//    printElement(array,length);
//    
//    while(true)
//    {
//        if(!permutation(array,length,vec))
//            break;
//    }
//    delete [] array;
//    system("pause");
//    return 0;
//}

#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <algorithm>
#include<iostream>
using namespace std;

typedef struct T{
    int n;
    int i;
} T_T;
#define swap(a, b) {t = *(a); *(a) = *(b); *(b) = t;}

int next_perm(int *b, int *e) {
    int t, *p, *q;
    for (p = e - 1; p >= b && *p > *(p + 1); p--);
    if (p < b)
        return 0;
    for (q = e; q > p && *q < *p; q--);
    swap(q, p);
    for (q = e, p++; p < q; q--, p++)
        swap(p, q);
    return 1;
}

void* thread(void *arg) {
    //long long t = *(long long*)(arg);
    //int i = t;
    //int n = t >> 32;
    T_T* ttp = (T_T*)arg;
    int i = ttp->i;
    int n = ttp->n;
    int k, a[20];
    a[1] = i;
    for (int j = 1, k = 2; j <= n; ++j)
        if (j != i)
            a[k++] = j;
    long long count = 1;
    while (next_perm(a + 2, a + n)){
        /*for(int i = 0;i < n;i++){
         cout << a[i+1] << "\t";
         }
         cout << endl;*/
        count++;
    }
    return (void*)count;
}

long long serial(int n) {
    if (n == 1)
        return 1;
    int a[20];
    for (int i = 0; i < n; ++i)
        a[i] = i + 1;
    long long count = 1;
    while (next_perm(a, a + n - 1))
        //	while (next_permutation(a, a + n - 1))
        count++;
    return count;
}

long long parallel(int n) {
    
    
    if (n == 1)
        return 1;
    pthread_t pids[20];
    long long tot = 0, count;
    for (int i = 1; i <= n; ++i) {
        //long long t = n;
        //t <<= 32;
        //t += i;
        T_T tt;
        tt.n = n;
        tt.i = i;
        pthread_create(pids + i, NULL, thread, (void*)&tt);
    }
    for (int k = 1; k <= n; ++k) {
        pthread_join(pids[k], (void**)(&count));
        tot += count;
    }
    return tot;
}


long long stdlib(int n) {
    int a[20];
    for (int i = 0; i < n; ++i)
        a[i] = i + 1;
    long long tot = 1;
    while (next_permutation(a, a + n))
    {
        //for (int i = 0; i < n; ++i)
        //	printf("%d ",a[i]);
        //printf("\n");
        tot++;
    }
    return tot;
}

int main() {
    int n;
    scanf("%d", &n);
    //struct timeval t1, t2;
    clock_t start, end;
    
    start = clock();
    
    long long tot;
    
    //gettimeofday(&t1, NULL);
    //tot = serial(n);
    //gettimeofday(&t2, NULL);
    //	printf("serial:   n=%lld\ttime=%.4f\n", tot, (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) / 1000000.0);
    
    tot = parallel(n);
    //gettimeofday(&t1, NULL);
    //	printf("parallel: n=%lld\ttime=%.4f\n", tot, (t1.tv_sec - t2.tv_sec) + (t1.tv_usec - t2.tv_usec) / 1000000.0);
    
    
    //tot = stdlib(n);
    //gettimeofday(&t2, NULL);
    //	printf("stdlib:   n=%lld\ttime=%.4f\n", tot, (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) / 1000000.0);
    
    
    end = clock();
    
    cout<<"Run time: "<<(double)(end - start) / CLOCKS_PER_SEC<<"S"<<endl;
    
    return 0;
}

