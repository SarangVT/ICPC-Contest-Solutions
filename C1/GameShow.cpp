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

    int t; cin>>t;
    vi b(t);
    fl(p,0,t) {
        cin>>b[p];
    }
    vector<ll> pref(t);
    pref[0] = b[0]+100;
    ll maxN = max((ll)100, pref[0]);
    fl(i, 1, t) {
        pref[i] = pref[i-1]+b[i];
        maxN = max(maxN, pref[i]);
    }
    cout<<maxN;
return 0;}