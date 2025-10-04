#include <bits/stdc++.h>
using namespace std;
#define vi vector<int>
#define vvi vector<vi>
#define ff first
#define ss second
#define fl(i,a,b) for(int i=a;i<b;++i)
#define flb(i,a,b) for(int i=a;i>=b;--i)
#define vii vector<pair<int,int>>
#define pii pair<int, int>
typedef long long ll;

bool gaussXor(vvi& A, vi& b) {
    int m = A.size();
    int n = A[0].size();
    int r = 0;

    for(int c = 0; c < n && r < m; ++c) {
        int pivot = r;
        while (pivot < n && A[pivot][c]==0) pivot++;
        if (pivot==n) continue;

        swap(A[pivot], A[r]);
        swap(b[pivot], b[r]);

        fl(i, 0, m) {
            if (i != r && A[i][c]) {
                fl(j, c, n) A[i][j] ^= A[r][j];
                b[i] ^= b[r];
            }
        }
        r++;
    }
    fl(i, 0, m) {
        bool allZero = 1;
        fl(j, 0, n) if(A[i][j]) {
            allZero = 0;
            break;
        }
        if(allZero && b[i]) return 0;
    }
    return 1;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int p, f;
    cin>>p>>f;
    vvi friends(p);

    fl(i, 0, f) {
        int a, b;
        cin>>a>>b;
        --a; --b;
        friends[a].push_back(b);
        friends[b].push_back(a);
    }
    vvi a(p, vi(p));
    vi b(p);
    fl(i, 0, p) {
        int deg = friends[i].size();
        if (deg%2==1) {
            a[i][i] = 1;
            for(int f : friends[i]) a[i][f] = 1;
            b[i] = 0;
        } else {
            for(int f : friends[i]) a[i][f] = 1;
            b[i] = 1;
        }
    }
    bool solEx = gaussXor(a, b);
    cout<<(solEx ? 'Y' : 'N')<<"\n";
return 0;}