#include <bits/stdc++.h>
using namespace std;

struct StringHash {
static const uint64_t base = 1315423911ULL;
vector<unsigned long long> h, p;
StringHash(const string &s) {
int n = s.size();
h.resize(n+1,0);
p.resize(n+1,1);
for (int i=0;i<n;i++) {
h[i+1] = h[i]*base + (unsigned long long)(s[i]);
p[i+1] = p[i]*base;
}
}
unsigned long long get(int l,int r){ // substring [l,r)
return h[r] - h[l]*p[r-l];
}
};

int main(){
ios::sync_with_stdio(false);
cin.tie(nullptr);

int M,N;
cin>>M>>N;
vector<string> A(M),B(N);
for (int i=0;i<M;i++) cin>>A[i];
for (int i=0;i<N;i++) cin>>B[i];

unordered_set<string> setA(A.begin(),A.end());
unordered_set<string> setB(B.begin(),B.end());

// Maps S -> involved words
unordered_map<string, vector<string>> mapA, mapB;

// Generate triplets for A
for (auto &w : A){
    int L=w.size();
    for (int k=1;k<=L;k++){
        string pref=w.substr(0,k);
        if (pref!=w && setA.count(pref)){
            string suf=w.substr(k);
            mapA[suf].push_back(pref);
            mapA[suf].push_back(w);
        }
    }
}

// Generate triplets for B
for (auto &w : B){
    int L=w.size();
    for (int k=0;k<L;k++){
        string suf=w.substr(k);
        if (suf!=w && setB.count(suf)){
            string pref=w.substr(0,k);
            mapB[pref].push_back(suf);
            mapB[pref].push_back(w);
        }
    }
}

unordered_set<string> nonpecA, nonpecB;

for (auto &kv : mapA){
    auto it = mapB.find(kv.first);
    if (it==mapB.end()) continue;
    for (auto &nm: kv.second){
        if (setA.count(nm)) nonpecA.insert(nm);
    }
    for (auto &nm: it->second){
        if (setB.count(nm)) nonpecB.insert(nm);
    }
}

cout << (M - (int)nonpecA.size()) << " " << (N - (int)nonpecB.size()) << "\n";
}