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
    cin.tie(nullptr);
    int n; double a, b;
    cin>>n>>a>>b;
    int k = (int)(b - a + 1);
    vector<double> dp(n+1, 0.0);
    vector<double> pref(n+1, 0.0);
    dp[0] = 0.0;
    pref[0] = 0.0;
    fl(i, 1, n+1) {
        int m = min((int) b, i - 1);
        if (m < a) dp[i] = 1.0;
        else {
            int sm = i - m;
            int bg = i - max(1, (int)a);
            //bg -- sm == i-a -- x-b
            double sum = 0.0;
            if(sm <= bg) sum = pref[bg] - ((sm >= 1) ? pref[sm - 1] : 0.0);

            if(a==0) dp[i] = ((k+sum) /(k -1.0));
            else dp[i] = 1.0 + sum/k;
        }
        pref[i] = pref[i-1] + dp[i];
    }
    cout.setf(std::ios::fixed); cout<<setprecision(6)<<dp[n]<<"\n";
return 0;}