#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

int main(){
    vector<int>vec={77, 42, 89, 33, 56, 92, 25, 73, 68, 81, 37, 28, 48, 99, 52};
    vector<int>even;
    vector<int>odd;
    for(int i=0;i<vec.size();i++){
        if(vec[i]%2==0){
            even.push_back(vec[i]);
        }
        else odd.push_back(vec[i]);
    }
    sort(even.begin(),even.end());
    sort(odd.begin(),odd.end(),[](auto a, auto b){
        return a>b;
    });

    vec.clear();
    for(int i=0;i<odd.size();i++){
        vec.push_back(odd[i]);
    }
    for(int i=0;i<even.size();i++){
        vec.push_back(even[i]);
    }
    for(int i=0;i<vec.size();i++){
        cout<<vec[i]<<" ";
    }
    return 0;
}