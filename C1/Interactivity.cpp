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
const ll mod = 1e9+7;

void dfs(int node, vector<array<ll, 2>> &dp, vvi &adj) {
    if(adj[node].empty()) {
        dp[node][0] = dp[node][1] = 1;
        return;
    }
    int sz = adj[node].size();
    for(int i: adj[node]) dfs(i, dp, adj);
    vector<ll> pref(sz+1, 1), suf(sz+1, 1);
    fl(i, 0, sz) {
        pref[i+1] = (pref[i]*dp[adj[node][i]][0]) % mod;
        suf[sz-i-1] = (suf[sz-i]*dp[adj[node][sz-i-1]][0]) % mod;
    }
    dp[node][0] = pref[sz];
    fl(i, 1, sz+1) {
        ll temp = (pref[i-1]*suf[i])%mod;
        temp *= dp[adj[node][i-1]][1];
        temp %= mod;
        dp[node][1] += temp;
        dp[node][1] %= mod;
    }
    dp[node][0] += dp[node][1];
    dp[node][0]%=mod;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n; cin>>n;
    vvi adj(n);
    fl(i, 1, n) {
        int x; cin>>x; --x;
        adj[x].push_back(i);
    }
    vector<array<ll, 2>> dp(n);
    dfs(0, dp, adj);
    cout<<dp[0][0];
return 0;}