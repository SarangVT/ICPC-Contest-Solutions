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

struct Fenwick {
    int n;
    vector<int> bit;
    Fenwick(int n) : n(n), bit(n+1, 0) {}
    void add(int idx, int val) {
        for (; idx <= n; idx += idx & -idx)
            bit[idx] += val;
    }
    int sum(int idx) {
        int res = 0;
        for (; idx > 0; idx -= idx & -idx)
            res += bit[idx];
        return res;
    }
    int range_sum(int l, int r) {
        return sum(r) - sum(l-1);
    }
};

int idx = 0;
vi tin, tout, euler;

void dfsEuler(int v, vvi &tree) {
    tin[v] = ++idx;
    euler.push_back(v);
    for(int u : tree[v])
        dfsEuler(u, tree);
    tout[v] = idx;
}

Fenwick ageBIT(100005);

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, m; cin>>n>>m;
    int k = 20;
    vvi adj(n+1, vi(k, 0));
    vi ages(n+1);
    fl(i,1,n+1)
        cin>>ages[i]>>adj[i][0];
    fl(j, 1, k) 
        fl(i, 1, n+1)
            adj[i][j] = adj[adj[i][j-1]][j-1];

    vvi tree(n+1);
    fl(i,2,n+1)
        tree[adj[i][0]].push_back(i);

    tin.resize(n+1);
    tout.resize(n+1);
    euler.reserve(n);
    dfsEuler(1, tree);

    Fenwick bit(n);
    vector<vii> addParty(n+2), removeParty(n+2); 

    fl(i, 0, m) {
        int o, l, r; cin >>o>>l>>r;
        int cur = o;
        flb(j, k-1, 0) if(ages[adj[cur][j]] <= r) cur = adj[cur][j];
        int Si = cur;
        addParty[tin[Si]].push_back({l, 1});
        removeParty[tout[Si]+1].push_back({l, 1});
    }
    vi ans(n+1, 0);
    fl(t, 1, n+1) {
        int v = euler[t-1];
        for(auto &p : addParty[t]) ageBIT.add(p.ff, p.ss);
        for(auto &p : removeParty[t]) ageBIT.add(p.ff, -p.ss);
        ans[v] = ageBIT.range_sum(1, ages[v]);
    }
    fl(i, 1, n+1) cout<<ans[i]<<" ";
    cout<<"\n";
return 0;}