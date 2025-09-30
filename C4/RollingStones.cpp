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
    fl(p,0,t) {
        int n, k; cin>>n>>k;
        vi a(n);
        map<int, int> freq;
        fl(i, 0, n) {
            cin>>a[i];
            freq[a[i]]++;
        }
        int ans = 0;
        fl(i, 0, k) {
            if(freq[i]==0) {
                ans++;
            }
        }
        if(ans<freq[k]) ans = freq[k];
        cout<<ans<<'\n';
    }
return 0;}