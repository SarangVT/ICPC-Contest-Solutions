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

vvi a(11, vi(11, 0));
bool check(int d, int l, int r, int c) {
    if(d==0) {
        if(c+l-1>10) return 0;
        fl(i, c, c+l) {
            if(a[r][i]==1) return 0;
            else a[r][i] = 1;
        } 
    }
    else {
        if(r+l-1>10) return 0;
        fl(i, r, r+l) {
            if(a[i][c]==1) return 0;
            else a[i][c] = 1;
        }
    }
    return 1;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n; cin>>n;
    bool ans = 1;
    fl(i, 1, n+1) {
        int d, l, r, c; cin>>d>>l>>r>>c;
        bool valid = check(d, l, r, c);
        if(!valid) ans = 0;
    }
    cout<<(ans ? "Y" : "N");
return 0;}