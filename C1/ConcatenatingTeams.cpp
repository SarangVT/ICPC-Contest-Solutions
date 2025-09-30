#include <bits/stdc++.h>
using namespace std;
#define vi vector<int>
#define vvi vector<vector<int>>
#define ff first
#define ss second
#define fl(i,a,b) for(int i=a;i<b;++i)
#define flb(i,a,b) for(int i=a;i>=b;--i)
#define vii vector<pair<int,int>>
#define pii pair<int, int>
typedef long long ll;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int m, n; cin>>m>>n;
    vector<string> a(m), b(n);
    fl(i, 0, m) cin>>a[i];
    fl(i, 0, n) cin>>b[i];
    
    sort(a.begin(), a.end());
    sort(b.begin(), b.end());

    int idx = 0;
    
return 0;}