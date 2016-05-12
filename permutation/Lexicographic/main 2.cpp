////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////Algorithm 1
//#include <stdio.h>
//#include <time.h>
//
//void print(const int *v, const int size)
//{
//    if (v != 0) {
//        for (int i = 0; i < size; i++) {
//            printf("%4d", v[i] );
//        }
//        printf("\n");
//    }
//}
//
//
//void visit(int *Value, int N, int k)
//{
//    static int level = -1;
//    level = level+1; Value[k] = level;
//    
//    //if (level == N)
//        //print(Value, N);
//        
//    //else
//        for (int i = 0; i < N; i++)
//            if (Value[i] == 0)
//                visit(Value, N, i);
//    
//    level = level-1; Value[k] = 0;
//}
//
//
//int main()
//{
//    clock_t start, end;
//    start = clock();
//    const int N = 12;
//    int Value[N];
//    for (int i = 0; i < N; i++) {
//        Value[i] = 0;
//    }
//    visit(Value, N, 0);
//    end = clock();
//    double duration = end - start;
//    printf("Duration: %lf\n", duration/CLOCKS_PER_SEC);
//}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////Algorithm 2
//
//#include <stdio.h>
//#include <time.h>
//
//void print(const int *v, const int size)
//{
//    if (v != 0) {
//        for (int i = 0; i < size; i++) {
//            printf("%4d", v[i] );
//        }
//        printf("\n");
//    }
//}
//
//
//void permute(int *v, const int start, const int n)
//{
////    if (start == n-1) {
////        print(v, n);
////    }
////    else {
//        for (int i = start; i < n; i++) {
//            int tmp = v[i];
//            
//            v[i] = v[start];
//            v[start] = tmp;
//            permute(v, start+1, n);
//            v[start] = v[i];
//            v[i] = tmp;
//        }
////    }
//}
//
//
//int main()
//{
//    clock_t start, end;
//    start = clock();
//    
//    const int N = 12;
//    int v[N];
//    for (int i = 0; i < N; i++) {
//        v[i] = 0;
//    }
//    
//    //int v[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
//    permute(v, 0, sizeof(v)/sizeof(int));
//    end = clock();
//    double duration = end - start;
//    printf("Duration: %lf s\n", duration/CLOCKS_PER_SEC);
//}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//#include <stdio.h>
//#include <time.h>
//
///**
// Read a number, N, from standard input and print the
// permutations.
// */
//
//void print(const int *v, const int size)
//{
//    if (v != 0) {
//        for (int i = 0; i < size; i++) {
//            printf("%4d", v[i] );
//        }
//        printf("\n");
//    }
//} // print
//
//
//void swap(int *v, const int i, const int j)
//{
//    int t;
//    t = v[i];
//    v[i] = v[j];
//    v[j] = t;
//}
//
//
//void rotateLeft(int *v, const int start, const int n)
//{
//    int tmp = v[start];
//    for (int i = start; i < n-1; i++) {
//        v[i] = v[i+1];
//    }
//    v[n-1] = tmp;
//} // rotateLeft
//
//
//void permute(int *v, const int start, const int n)
//{
//    //print(v, n);
//    if (start < n) {
//        int i, j;
//        for (i = n-2; i >= start; i--) {
//            for (j = i + 1; j < n; j++) {
//                swap(v, i, j);
//                permute(v, i+1, n);
//            } // for j
//            rotateLeft(v, i, n);
//        } // for i
//    }
//} // permute
//
//
//void init(int *v, int N)
//{
//    for (int i = 0; i < N; i++) {
//        v[i] = i+1;
//    }
//} // init
//
//
//int main()
//{
//    clock_t start, end;
//    start = clock();
//    char buf[80];
//    int N;
//    printf("Enter the number of elements: ");
//    fgets(buf, sizeof(buf), stdin );
//    sscanf(buf, "%d", &N);
//    
//    if (N > 0 && N <= 12) {
//        int *v = new int[N];
//        init(v, N);
//        permute(v, 0, N);
//        delete [] v;
//    }
//    
//    end = clock();
//    double duration = end - start;
//    printf("Duration: %lf s\n", duration/CLOCKS_PER_SEC);
//    
//    return 0;
//}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// multithread

//#include <stdio.h>
//#include <time.h>
//#include <pthread.h>
//
///**
// Read a number, N, from standard input and print the
// permutations.
// */
//
//void print(const int *v, const int size)
//{
//    if (v != 0) {
//        for (int i = 0; i < size; i++) {
//            printf("%4d", v[i] );
//        }
//        printf("\n");
//    }
//} // print
//
//void swap(int *v, const int i, const int j)
//{
//    int t;
//    t = v[i];
//    v[i] = v[j];
//    v[j] = t;
//}
//
//void rotateLeft(int *v, const int start, const int n)
//{
//    int tmp = v[start];
//    for (int i = start; i < n-1; i++) {
//        v[i] = v[i+1];
//    }
//    v[n-1] = tmp;
//} // rotateLeft
//
//
//
//struct mypara
//{
//    int *v;
//    int *i;
//    int *j;
//    int n;
//};
//
//void permute(int *v, const int start, const int n);
//
//
//void *sub_permute(void *arg){
//    struct mypara *para = (struct mypara *)arg;
//    
//    int *v = para->v;
//    int *i = para->i;
//    int *j = para->j;
//    int n = para->n;
//    
//    for (*j = *i + 1; *j < n; (*j)++) {
//        swap(v, *i, *j);
//        permute(v, *(i)+1, n);
//    } // for j
//    rotateLeft(v, *i, n);
//    
//    return (void *)(0);
//}
//
//void permute(int *v, const int start, const int n)
//{
//    print(v, n);
//    if (start < n) {
//        int i, j;
//        
//        if (start == 0) {
//            pthread_t *threadids = new pthread_t[n-2-start+1];
//            for (i = n-2; i >= start; i--) {
//                struct mypara para;
//                para.v = v;
//                para.n = n;
//                para.i = &i;
//                para.j = &j;
//                pthread_create(&threadids[i], NULL, sub_permute, &para);
//            }
//            
//        } else {
//            for (i = n-2; i >= start; i--) {
//                
//                for (j = i + 1; j < n; j++) {
//                    swap(v, i, j);
//                    permute(v, i+1, n);
//                } // for j
//                rotateLeft(v, i, n);
//            } // for i
//        }
//    }
//} // permute
//
//
//void init(int *v, int N)
//{
//    for (int i = 0; i < N; i++) {
//        v[i] = i+1;
//    }
//} // init
//
//int main()
//{
//    clock_t start, end;
//    start = clock();
//    
//    char buf[80];
//    int N;
//    printf("Enter the number of elements: ");
//    fgets(buf, sizeof(buf), stdin );
//    sscanf(buf, "%d", &N);
//
//    
//    if (N > 0 && N <= 13) {
//        
//        int *v = new int[N];
//        init(v, N);
//        permute(v, 0, N);
//        
//        delete [] v;
//    }
//
//    end = clock();
//    double duration = end - start;
//    printf("Duration: %lf s\n", duration/CLOCKS_PER_SEC);
//
//    return 0;
//}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//#include <stdio.h>
//#include <time.h>
//#include <pthread.h>
//#include <iostream>
//
//using namespace std;
//
//struct mypara
//{
//    int *v;
//    int n;
//};
//
//void init(int *v, int N, int omit)
//{
//    int step = 0;
//    for (int i = 0; i <= N; i++) {
//        if(i+1 == omit) {
//            step ++;
//            continue;
//        }
//        v[i-step] = i+1;
//    }
//    
////    for (int i = 0; i < N; ++i) {
////        cout << v[i] << " ";
////        if (i == N-1) {
////            cout << "\n";
////        }
////    }
//}
//
//void print(const int *v, const int size)
//{
//    if (v != 0) {
//        for (int i = 0; i < size; i++) {
//            printf("%4d", v[i] );
//        }
//        printf("\n");
//    }
//}
//
//void swap(int *v, const int i, const int j)
//{
//    int t;
//    t = v[i];
//    v[i] = v[j];
//    v[j] = t;
//}
//
//void rotateLeft(int *v, const int start, const int n)
//{
//    int tmp = v[start];
//    for (int i = start; i < n-1; i++) {
//        v[i] = v[i+1];
//    }
//    v[n-1] = tmp;
//}
//
//void permute(int *v, const int start, const int n)
//{
//    //print(v, n);
//    if (start < n) {
//        int i, j;
//        for (i = n-2; i >= start; i--) {
//            for (j = i + 1; j < n; j++) {
//                swap(v, i, j);
//                permute(v, i+1, n);
//            }
//            rotateLeft(v, i, n);
//        }
//    }
//}
//
//void *excute_permute (void *arg) {
//    struct mypara *para = (struct mypara *)arg;
//    int *arr = para->v;
//    int n = para->n;
//    
//    clock_t start, end;
//    start = clock();
//    permute(arr, 0, n);
//    
//    end = clock();
//    double duration = end - start;
//    printf("Thread Duration: %lf s\n", duration/CLOCKS_PER_SEC);
//    
//    return (void *)0;
//}
//
//int main()
//{
//    clock_t start, end;
//    start = clock();
//    
//    char buf[80];
//    int N;
//    printf("Enter the number of elements: ");
//    fgets(buf, sizeof(buf), stdin );
//    sscanf(buf, "%d", &N);
//    
//    
//    if (N > 0 && N <= 13) {
//        // Divide the initial array into N-1 subarray
//        // Execute the process into N-1 threads
//        pthread_t *threads = new pthread_t[N];
//        void *retval;
//        for (int i = 0; i < N; ++i) {
//            // Generate the new array
//            int *v = new int[N-1];
//            init(v, N-1, i+1);
//            
//            // Assemble parameter for function
//            struct mypara para;
//            para.v = v;
//            para.n = N-1;
//            // Start a thread to generate permutations
//            pthread_create(&threads[i], NULL, excute_permute, &para);
//        }
//        for (int i = 0; i < N; ++i) {
//            pthread_join(threads[i], &retval);
//        }
//    }
//    
//    end = 0;
//    end = clock();
//    double duration = end - start;
//    printf("Duration: %lf s\n", duration/CLOCKS_PER_SEC);
//    return 0;
//}

//#include <iostream>
//#include <vector>
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
//    //printElement(array,length);
//    return true;
//}
//
//void swapA(int A[],int i, int j)
//{
//    int temp = A[i];
//    A[i] = A[j];
//    A[j] = temp;
//}
//
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
//
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
//int  main()
//{
//    clock_t start, end;
//    start = clock();
//    
//    int data[] = {1,2,3,4,5,6,7,8,9,10,11,12};
//    vector<int> vec;
//    int length = sizeof(data)/sizeof(int);
//    quicksort(data,0,length);
//    
//    Element* array = new Element[length];
//    InitElement(data,array,length);
//    //printElement(array,length);
//    
//    while(true)
//    {
//        if(!permutation(array,length,vec))
//            break;
//    }
//    delete [] array;
//    
//    end = clock();
//    double duration = end - start;
//    printf("Duration: %lf s\n", duration/CLOCKS_PER_SEC);
//    system("pause");
//    return 0;
//}

//// permutations of any string inputted by user //
//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
//#define display(X) printf( "\n%s", X );
//static int counter = 1 ;
///**
// * This function is used to remove the trailing new line which is normally present at the end of the string accepted from the user
// * using fgets( ). This function is very important since without this the newline character at the end of the input will be considered
// * while drawing out permutations of the string which we dont want.
// *
// * @author ~s.o.s~
// * @param input C style string from which the trailing newline has to be removed.
// */
//void remove_newline( char* input )
//{
//    char* p = 0 ;
//    if( p = strrchr( input, '\n' ) )
//        *p = '\0' ;
//}
///**
// * This function is used to swap the two characters in the string array under consideration.
// *
// * @author ~s.o.s~
// * @param a - character pointer pointing at the character to be swapped.
// * @param b - character pointer pointing at the character to be swapped with.
// */
//void swapPlaces (char* a, char* b)
//{
//    char temp = *a;
//    *a = *b;
//    *b = temp;
//}
///**
// * The algorithm used for permutations of the string is an adaptation of the "Countdown Quickperm algorithm"
// * by Mr. Phillip Paul Fuchs. The credit for this algo goes to him.
// *
// * @author ~s.o.s~
// * @param input which is the old C style string holding the string which has to be permuted
// */
//void wordPermutation (const char* input)
//{
//    int string_length = strlen( input ) ;
//    if ( string_length == 0)		// guard against no input
//        return ;
//    int* p = (int*) malloc( string_length + 1 ) ;
//    // start of naked block meant for initializing array P
//    {
//        int j ;
//        for( j = 0; j <= string_length; ++j )
//            p[j] = j ;
//    }
//    // end of naked block
//    char* tempBuffer = (char*) malloc( string_length + 1 ); // dont affect the original string, create temporary string.
//    strcpy (tempBuffer, input);
//    //printf( "\n%s", tempBuffer ) ;
//    /// core algorithm begins
//    int i = 1, j = 0;
//    while(i < string_length)
//    {
//        p[i]--;
//        j = i % 2 * p[i];
//        swapPlaces( &tempBuffer[i], &tempBuffer[j] ) ;
//        counter++ ;
//        //display(tempBuffer);
//        i = 1;
//        while (!p[i])
//        {
//            p[i] = i;
//            i++;
//        }
//    }
//    /// core algorithm ends
//    free( tempBuffer ) ;
//    free( p ) ;
//    //printf( "\n\nThe number of permutations is %d\n\n", counter ) ;
//}
//int main ( )
//{
//    char buffer[BUFSIZ] = {'\0'} ;
//    printf ("\nEnter the string whose permutation u want: ");
//    fgets (buffer, BUFSIZ, stdin);
//    remove_newline (buffer);
//    wordPermutation (buffer);
//    return 0;
//}

/*
 *    FILENAME:    05_Perms.CPP (formally Permutations.CPP)
 *       TITLE:    Iterative Examples of Linear Array Permutations
 *                 Using C++ Without Recursive Processes
 *      AUTHOR:    Mr. Phillip P. Fuchs
 *        DATE:    1991 Spring Project...
 *      NOTICE:    Copyright 1991-2002, Phillip Paul Fuchs
 *
 *   INTENT:   To gain a familiarity with linear array permutations without the
 *             use of recursion.  In particular, this program will apply several
 *             "example" functions to demonstrate various iterative brute-force
 *             procedures to compute all unique combinations of any linear array
 *             type or of any character string.
 *
 *             Unlike lexicographic (alphabetic) ordering which tends to nest
 *             three iterative loops and can produce more than two indices for
 *             each reversal; most examples presented below use a single nested
 *             loop to create only two indexes that insures a unique permutation.
 *             All "example" functions given below are self-contained and
 *             independent from other examples presented in this program. Each
 *             example will compute new i & j index values such that a unique
 *             swap can occur.
 *
 *             For all head permutations, the index variable i is determined by
 *             the array p[N+1], which is one integer size larger than the
 *             target array size.  Every element of the array p[] is initialized
 *             to the corresponding value of its index.  For each iteration, the
 *             index variable i is initialized to 1 and a new index value for i
 *             is determined by the first non-zero value sequentially found in
 *             p[i].  In addition, if the value found at p[i] is zero, then p[i]
 *             is reset to the corresponding value of its index.  The value
 *             found at p[i] is reduced by one and the value for j is calculated
 *             using the following fact: IF i is odd then j = p[i] otherwise
 *             j = 0.  At this point, a unique swap within the target array may
 *             occur only if the value of i is less than the total number of
 *             elements (N) in the target array.  This process is repeated until
 *             the value of i equals the size of the target array (when i equals
 *             N) and all permutations are complete.  Please note in every case,
 *             the index value of i will always be greater than the index value
 *             of j for all head permutations.
 *
 *             For all tail permutations, the index variable i is again determined
 *             by the array p[N], which is one integer size larger than the target
 *             array size.  In most cases, the values for i and j are simply
 *             adjusted with respect to the index ceiling of the target array.
 *             In addition, example_04() reverses array p[] and consequently its
 *             size is the same as the number of elements found in the target
 *             array.  Other than these facts, the procedure is identical to the
 *             head permutations mentioned above.
 *
 *             As a final example (example_05), the META class calculates all
 *             indices required for the previous four examples.  The constructor
 *             for Meta will initialize all indices.  The Meta class uses the
 *             transpose() function to compute new index values i & j for head
 *             permutations and q & r for tail permutations.  The value of i will
 *             always be greater than the value of j and the value of r will
 *             always be greater than the value of q.  All permutations will be
 *             complete when i exactly equals the number of elements in the
 *             target array.
 *
 *             For all permutations the p[] array controls every index value.
 *
 *             Compiled without syntax errors and warnings using Borland C++
 *             Version 5.02 in a Microsoft Windows 98 operating system environment.
 *
 *             Please remove comments to display the target array(s)...
 *
 */

#include <stdio.h>
#include <time.h>
#include <pthread.h>

int N;

/******************************************************************************/
/******************************************************************************/


void display(unsigned int *a)
{
    for(unsigned int x = 0; x < N; x++)
        printf("%d ",a[x]);
    printf("\n");
}


void permute(void)
{
    const unsigned int ax = N - 1;   // constant index ceiling (a[N] length)
    unsigned int a[N], p[N];         // target array and index control array
    register unsigned int i, j, tmp; // index variables and tmp for swap
    
    for(i = 0; i < N; i++)   // initialize arrays; a[N] can be any type
    {
        a[i] = N - i;   // a[i] value is arbitrary and not revealed
        p[i] = N - i;
    }
    i = ax;   // setup first swap point to be ax
    while (i > 0)
    {
        p[i]--;             // decrease index "weight" for i by one
        i--;                // i must be less then j (i < j)
        j = ax;             // reset j to ax
        do                  // reverse target array from i to j
        {
            tmp = a[j];      // swap(a[i], a[j])
            a[j] = a[i];
            a[i] = tmp;
            j--;             // decrement j by 1
            i++;             // increment i by 1
        } while (j > i);
        display(a);       // remove comment to display target array a[]
        i = ax;             // reset index i to ax (assumed)
        while (!p[i])       // while (p[i] == 0)
        {
            p[i] = N - i;    // reset p[i] zero value
            i--;             // set new index value for i (decrease by one)
        }
    }
}

int main(void)
{
    clock_t start_time, end_time;
    start_time = clock();
    
    scanf("%d", &N);
    freopen("output.txt", "w", stdout);
    permute();
    end_time = clock();
    printf("duration is %f\n", (end_time - start_time)*1.0/CLOCKS_PER_SEC);
    
    return 0;
}
