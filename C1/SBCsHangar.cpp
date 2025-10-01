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

ll nCk(int n, int k) {
    if(k > n) return 0;
    if(k == 0 || k == n) return 1;
    if(k > n-k) k = n-k;
    ll ans = 1;
    fl(i,1,k+1) {
        ans = ans * (n-k+i) / i;
    }
    return ans;
}

ll countWaysLessThanW(vector<ll>& w, ll W, int k) {
    int n = w.size();
    ll ans = 0;
    int bx = k;
    flb(i, n-1, 0) {
        if(w[i] > W) continue;
        if(bx == 1) {
            ans += (i+1);
            break;
        }
        ans += nCk(i, bx);
        W -= w[i];
        bx--;
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, k; cin>>n>>k;
    vector<ll> w(n);
    fl(i,0,n) cin >> w[i];
    sort(w.begin(), w.end());
    ll a, b; cin>>a>>b;
    cout<<countWaysLessThanW(w, b, k)-countWaysLessThanW(w, a-1, k)<<"\n";
return 0;}