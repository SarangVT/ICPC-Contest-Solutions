#include <bits/stdc++.h>
using namespace std;
#define vi vector<int>
#define vl vector<long long>
#define vvi vector<vector<int>>
#define ff first
#define ss second
#define fl(i,a,b) for(int i=a;i<b;++i)
#define flb(i,a,b) for(int i=a;i>=b;--i)
#define vii vector<pair<int,int>>
#define pii pair<int, int>
typedef long long ll;
const ll mod = 1e9+7;

vl kmp_prefix(const vl& p) {
    int n = p.size();
    vl pi(n);
    fl(i, 1, n) {
        int j = pi[i-1];
        while(j > 0 && p[i]!=p[j]) j = pi[j-1];
        if(p[i]==p[j]) ++j;
        pi[i] = j;
    }
    return pi;
}

vi kmp_find(const vl& s, const vl& p) {
    if(p.empty() || s.size() < p.size()) return {};
    vl pi = kmp_prefix(p);
    vi res;
    int j = 0;
    fl(i, 0, s.size()) {
        while(j > 0 && s[i]!=p[j]) j = pi[j-1];
        if(s[i]==p[j]) ++j;
        if(j == p.size()) {
            res.push_back(i - p.size() + 1);
            j = pi[j-1];
        }
    }
    return res;
}

struct Mat { ll a[2][2]; };

Mat mul(const Mat &x, const Mat &y){
    Mat r;
    fl(i, 0, 2) fl(j, 0, 2) {
        ll sum = 0;
        fl(k, 0, 2) {
            sum += x.a[i][k] * y.a[k][j];
            sum %= mod;
        }
        r.a[i][j] = sum % mod;
    }
    return r;
}

Mat matPow(Mat base, ll exp){
    Mat res;
    res.a[0][0]=1; res.a[0][1]=0; res.a[1][0]=0; res.a[1][1]=1;
    while(exp){
        if(exp&1) res = mul(res, base);
        base = mul(base, base);
        exp >>= 1;
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n; ll m, k;
    cin>>n>>m>>k;
    vl a(n), b(n);
    fl(i, 0, n) cin>>a[i];
    fl(i, 0, n) cin>>b[i];
    vl da(n), db(n);

    fl(i, 0, n) {
        int ni = (i+1) % n;
        da[i] = ((a[ni] - a[i])+m) % m;
        db[i] = ((b[i] - b[ni]) + m) % m;
    }

    vl text = da;
    text.insert(text.end(), da.begin(), da.end());

    vi occ = kmp_find(text, db);
    int cnt = 0;
    bool zero_match = 0;
    for(int pos : occ) {
        if(pos < n) {
            ++cnt;
            if(pos == 0) zero_match = 1;
        } else break;
    }

    if(cnt == 0) {
        cout<<0<<"\n";
        return 0;
    }

    Mat t;
    t.a[0][0] = 0; t.a[0][1] = (n-1) % mod;
    t.a[1][0] = 1; t.a[1][1] = (n-2) % mod;
    Mat p = matPow(t, k);
    ll w0 = p.a[0][0] % mod;
    ll w1 = p.a[1][0] % mod;

    ll cnt0 = zero_match ? 1 : 0;
    ll cnt1 = cnt - cnt0;

    ll ans = ((w0 * cnt0) % mod + (w1 * cnt1) % mod) % mod;
    cout<<ans<<"\n";
return 0;}