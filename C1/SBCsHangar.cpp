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

    int n, k; cin >> n >> k;
    vector<ll> a(n);
    fl(i,0,n) cin >> a[i];
    sort(a.begin(), a.end());
    ll c, d; cin >> c >> d;

    int half1 = n/2, half2 = n - half1;
    vector<vector<ll>> lS(k+1), rS(k+1);

    fl(state,0,(1<<half1)) {
        int taken = 0;
        ll sum = 0;
        fl(i,0,half1) if(state & (1<<i)) {
            taken++;
            sum += a[i];
        }
        if(taken <= k) {
            lS[taken].push_back(sum);
        }
    }

    fl(state,0,(1<<half2)) {
        int taken = 0;
        ll sum = 0;
        fl(i,0,half2) if(state & (1<<i)) {
            taken++;
            sum += a[half1+i];
        }
        if(taken <= k) {
            rS[taken].push_back(sum);
        }
    }

    fl(i,0,k+1) {
        sort(lS[i].begin(), lS[i].end());
        sort(rS[i].begin(), rS[i].end());
    }

    ll ans = 0;
    fl(leftSize,0,k+1) {
        int rightSize = k - leftSize;
        if(rightSize < 0 || rightSize > k) continue;
        for(ll sumLeft : lS[leftSize]) {
            ll lo = c - sumLeft, hi = d - sumLeft;
            auto &vec = rS[rightSize];
            auto it1 = lower_bound(vec.begin(), vec.end(), lo);
            auto it2 = upper_bound(vec.begin(), vec.end(), hi);
            ans += (it2 - it1);
        }
    }

    cout<<ans<<"\n";
return 0;}