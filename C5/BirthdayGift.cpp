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
        string s; cin>>s;
        vi count(3);
        fl(i, 0, s.size()) {
            if(s[i]=='2') count[2]++;
            else if(i%2==1 && (s[i]=='1' || s[i]=='0')) count[s[i]-'0']++;
            else if(i%2==0 && (s[i]=='1' || s[i]=='0')) count[1 ^ (s[i]-'0')]++;
        }
        int diff = abs(count[0]-count[1]);
        if(count[2]>diff) {
            cout<<(count[2]-diff)%2;
        }
        else {
            cout<<(diff-count[2]);
        }
        cout<<'\n';
    }
return 0;}