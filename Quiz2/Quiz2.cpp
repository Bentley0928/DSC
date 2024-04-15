//
//  main.cpp
//  Quiz2.cpp
//
//  Created by Bentley on 2024/4/15.
//

#include <bits/stdc++.h>
using namespace std;
struct Pokemon {
    int id;
    int lvl;
};
vector < list<Pokemon> > pok(11);
void insert(int int1, Pokemon idx){
    pok[int1].push_back(idx);
}
void removeid(int int1, int int2){
    auto it = pok[int1].begin();
    while(it != pok[int1].end()){
        auto todel = it;
        bool rem = 0;
        if(it->id == int2){
            rem = 1;
        }
        ++it;
        if(rem){
            pok[int1].erase(todel);
        }
    }
}
void Rotate(int int1, int int2){
    int dotimes = int2;
    while (dotimes--) {
        Pokemon tomove = *prev(pok[int1].end());
        pok[int1].erase(prev(pok[int1].end()));
        pok[int1].insert(pok[int1].begin(), tomove);
    }
}
void Reorder(int int1){
    list<Pokemon> oddline, evenline;
    auto it = pok[int1].begin();
    while (1) {
        oddline.push_back(*it);
        ++it;
        if(it == pok[int1].end()) break;
        ++it;
        if(it == pok[int1].end()) break;
    }
    it = next(pok[int1].begin());
    while (1) {
        evenline.push_back(*it);
        ++it;
        if(it == pok[int1].end()) break;
        ++it;
        if(it == pok[int1].end()) break;
    }
    pok[int1].clear();
    for(auto i:oddline){
        pok[int1].push_back(i);
    }
    for(auto i:evenline){
        pok[int1].push_back(i);
    }
}
void Reverse(int int1, int int2, int int3){
    list <Pokemon> tmp;
    auto it = pok[int1].begin();
    for(int i=0;i<int2;i++){
        tmp.push_back(*it);
        it++;
    }
    vector<Pokemon> rev;
    for(int i=int2;i<=int3;i++){
        rev.push_back(*it);
        it++;
    }
    reverse(rev.begin(), rev.end());
    for(auto i:rev){
        tmp.push_back(i);
    }
    for(; it != pok[int1].end();it++){
        //cout << "tthaha\n";
        tmp.push_back(*it);
    }
    pok[int1].clear();
    pok[int1]=tmp;
}
void merging(int int1, int int2){
    auto it = pok[int2].begin();//to comp
    auto it2 = pok[int1].begin();//aim
    while(it != pok[int2].end()){
        if(it2 == pok[int1].end()){
            pok[int1].insert(it2, *it);
            it++;
        }
        else if(it->lvl < it2->lvl){
            pok[int1].insert(it2, *it);
            it++;
            continue;
        }
        else if(it->lvl == it2->lvl){
            if(it->id < it2->id){
                pok[int1].insert(it2, *it);
                it++;
                continue;
            }
            else{
                it2++;
                continue;
            }
        }
        else{
            it2++;
            continue;
        }
    }
    pok[int2].clear();
}
void display(int n){
    for(int i=0;i<n;i++){
        cout << "List " << i << "\n";
        if(pok[i].empty()){
            cout << "Empty" << "\n";
            continue;
        }
        for(auto a:pok[i]){
            cout << "ID: " << a.id << " Level: " << a.lvl << "\n";
        }
    }
}
int main() {
    int n,x;
    cin >> n >> x;
    while(x--){
        string s;
        cin >> s;
        if(s == "Insert"){
            int int1, int2, int3;
            cin >> int1 >> int2 >> int3;
            Pokemon idx;
            idx.id = int2;
            idx.lvl = int3;
            insert(int1, idx);
        }
        else if (s == "Remove"){
            int int1,int2;
            cin >> int1 >> int2;
            removeid(int1, int2);
        }
        else if (s == "Rotate"){
            int int1, int2;
            cin >> int1 >> int2;
            Rotate(int1, int2);
        }
        else if (s == "Reorder"){
            int int1;
            cin >> int1;
            Reorder(int1);
        }
        else if (s == "Reverse"){
            int int1, int2, int3;
            cin >> int1 >> int2 >> int3;
            Reverse(int1, int2, int3);
        }
        else if(s == "MergeListsPreserveOrder"){
            int int1, int2;
            cin >> int1 >> int2;
            merging(int1, int2);
        }
    }
    display(n);
    return 0;
}
