#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <random>

using namespace std;

void generate_arr(vector<int>& arr, int max){
    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<int> dist(0,max);
    for(int i=0; i<arr.size(); i++){
        arr[i] = dist(mt);
    }
}

bool test_sort(vector<int> arr)
{
    for (int i = 1; i < arr.size(); i++)
        if (arr[i] < arr[i - 1])
            return false;
    return true;
}

int max(vector<int> arr){
    int m = arr[0];
    for(int i=1;i<arr.size();i++){
        if(arr[i]>m)
            m=arr[i];
    }
    return m;
}

void print(vector<int> arr){
    for(int i=0; i<arr.size(); i++){
        cout<<arr[i]<<" ";
    }
}

void bubble_sort(vector<int>& arr){
    for(int i=0; i<arr.size()-1; i++){
        for(int j=0; j<arr.size()-i-1; j++)
            if(arr[j]>arr[j+1])
                swap(arr[j], arr[j+1]);
    }
}

void count_sort(vector<int>& arr){
    int m = max(arr), index=0;
    vector<int> count(m+1), out(arr.size());
    for(int i=0; i<=m; i++){
        count[i] = 0;
    }
    for(int i=0; i<arr.size(); i++){
        ++count[arr[i]];
    }
    for(int i=0; i<=m; i++){
        while(count[i]!=0){
            arr[index++]=i;
            count[i]--;
        }
    }
}

// merge sort

void intercls(vector<int>& arr, int st, int m, int dr){
    int len1 = m - st + 1, len2 = dr - m, i1=0, i2=0, i3=st;
    vector<int> S(len1), D(len2);
    for (int i = 0; i < len1; i++)
        S[i] = arr[st + i];
    for (int i = 0; i < len2; i++)
        D[i] = arr[m + 1 + i];
    while ((i1 < len1) && (i2 < len2)) {
        if (S[i1] <= D[i2]) {
            arr[i3] = S[i1];
            i1++;
        }
        else {
            arr[i3] = D[i2];
            i2++;
        }
        i3++;
    }
    while (i1 < len1) {
        arr[i3] = S[i1];
        i1++;
        i3++;
    }
    while (i2 < len2) {
        arr[i3] = D[i2];
        i2++;
        i3++;
    }
}

void merge_sort(vector<int>& arr, int st, int dr){
    int m;
    if(st<dr){
        m=st + (dr-st)/2;
        merge_sort(arr,st,m);
        merge_sort(arr,m+1,dr);
        intercls(arr, st, m, dr);
    }
}

//quick sort pivot ultimul element

int part_rightmost(vector<int>& arr, int mmic, int mmare){
    int rightindex = mmic - 1;
    int p = arr[mmare];
    for(int i = mmic; i<=mmare-1; i++){
        if(arr[i]<p){
            ++rightindex;
            swap(arr[rightindex], arr[i]);
        }
    }
    swap(arr[mmare],arr[rightindex+1]);
    return rightindex+1;
}

void quick_sort_rightmost(vector<int>& arr, int mmic, int mmare){
    if(mmic < mmare){
        int p = part_rightmost(arr, mmic, mmare);
        quick_sort_rightmost(arr,mmic,p-1);
        quick_sort_rightmost(arr,p+1,mmare);
    }
}

//quicksort cu mediana din 3

int part_3way(vector<int>& arr, int mmic, int mmare){
    int mmij=mmic+(mmare-mmic)/2;
    if(arr[mmare] < arr[mmic]){
        swap(arr[mmare], arr[mmic]);
    }
    if(arr[mmij] < arr[mmic]){
        swap(arr[mmij],arr[mmic]);
    }
    if(arr[mmare] < arr[mmij]){
        swap(arr[mmare],arr[mmij]);
    }
    int p = arr[mmare];
    int rightindex = mmic - 1;
    for(int i = mmic; i<=mmare-1; i++){
        if(arr[i]<p){
            ++rightindex;
            swap(arr[rightindex], arr[i]);
        }
    }
    swap(arr[mmare],arr[rightindex+1]);
    return rightindex+1;
}

void quick_sort_3way(vector<int>& arr, int mmic, int mmare){
    if(mmic < mmare){
        int p = part_3way(arr, mmic, mmare);
        quick_sort_3way(arr,mmic,p-1);
        quick_sort_3way(arr,p+1,mmare);
    }
}

void radix_sort_LSD(vector<int>& arr){
    int m = max(arr), e=1;
    vector<int> out(arr.size());
    while(m/e>0){
        vector<int> count(10);
        for(int i=0; i<arr.size(); i++){
            ++count[(arr[i]/e)%10];
        }
        for(int i=1; i<10; i++){
            count[i] += count[i-1];
        }
        for(int i=arr.size()-1; i>= 0; i--){
            out[--count[(arr[i]/e) %10]] = arr[i];
        }
        for(int i=0; i<arr.size(); i++){
            arr[i] = out[i];
        }
        e *= 10;
    }
}

void radix_sort_bitwise(vector<int>& arr){
    int m = max(arr), b = 0;
    vector<int> out(arr.size());
    while((m>>b) > 0){
        vector<int> count(2);
        for(int i=0; i<arr.size(); i++){
            ++count[arr[i]>>b & 1];
        }
        count[1]+=count[0];
        for(int i=arr.size()-1; i>=0; i--){
            out[--count[(arr[i]>>b) & 1]] = arr[i];
        }
        for(int i=0; i<arr.size();i++){
            arr[i]=out[i];
        }
        b++;
    }
}

int main(){
    ofstream fout("rezultate_csort.txt");
    ifstream f1("test1.txt");
    ifstream f2("test2.txt");
    ifstream f3("test3.txt");
    ifstream f4("test4.txt");
    ifstream f5("test5.txt");
    vector<int> test1(1000),test2(1000000),test3(100000000);
    chrono::duration<double> time;
    fout<<"Sortarea din algorithm:\n";

    fout<<"n=1000 max=1000\n";
    for(int i=0; i<test1.size(); i++){
        f1>>test1[i];
    }
    auto start = chrono::high_resolution_clock::now();
    sort(test1.begin(),test1.end());
    auto stop = chrono::high_resolution_clock::now();
    time = stop - start;
    if(test_sort(test1)){
        fout<<time.count()<<"s\n";
    }
    else fout<<"failed sort\n";

    fout<<"n=1000000 max=1000000\n";
    for(int i=0; i<test2.size(); i++){
        f2>>test2[i];
    }
    start = chrono::high_resolution_clock::now();
    sort(test2.begin(),test2.end());
    stop = chrono::high_resolution_clock::now();
    time = stop - start;
    if(test_sort(test2)){
        fout<<time.count()<<"s\n";
    }
    else fout<<"failed sort\n";

    fout<<"n=100000000 max=1000\n";
    for(int i=0; i<test3.size(); i++){
        f3>>test3[i];
    }
    start = chrono::high_resolution_clock::now();
    sort(test3.begin(),test3.end());
    stop = chrono::high_resolution_clock::now();
    time = stop - start;
    if(test_sort(test3)){
        fout<<time.count()<<"s\n";
    }
    else fout<<"failed sort\n";

    fout<<"n=100000000 max=1000000\n";
    for(int i=0; i<test3.size(); i++){
        f4>>test3[i];
    }
    start = chrono::high_resolution_clock::now();
    sort(test3.begin(),test3.end());
    stop = chrono::high_resolution_clock::now();
    time = stop - start;
    if(test_sort(test3)){
        fout<<time.count()<<"s\n";
    }
    else fout<<"failed sort\n";

    fout<<"n=1000 max=1000000000\n";
    for(int i=0; i<test1.size(); i++){
        f5>>test1[i];
    }
    start = chrono::high_resolution_clock::now();
    sort(test1.begin(),test1.end());
    stop = chrono::high_resolution_clock::now();
    time = stop - start;
    if(test_sort(test1)){
        fout<<time.count()<<"s\n";
    }
    else fout<<"failed sort\n";


    return 0;
}