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

    string s; cin>>s;
    int n = s.size();
    char L = 'S';
    int pL = 0, pR = 0;
    int gameWinL = 0, gameWinR = 0;
    int g = 0;
    fl(i, 0, n) {
        if(s[i]=='Q') {
            if(gameWinL>1) {
                cout<<gameWinL<<" (winner) - "<<gameWinR<<'\n';
            }
            else if(gameWinR>1) {
                cout<<gameWinL<<" - "<<gameWinR<<" (winner)"<<'\n';
            }
            else if(L=='S') {
                cout<<gameWinL<<" ("<<pL<<"*) - "<<gameWinR<<" ("<<pR<<")\n";
            }
            else {
                cout<<gameWinL<<" ("<<pL<<") - "<<gameWinR<<" ("<<pR<<"*)\n";
            }
            continue;
        }
        if(L==s[i]) {
            pL++;
            L = 'S';
        }
        else {
            pR++;
            L = 'R';
        }
        if(pR>4 && pR-pL>1 || pR>9) {
            gameWinR++;
            L = 'R';
            pL = 0;
            pR = 0;
        }
        else if(pL>4 && pL-pR>1 || pL>9) {
            gameWinL++;
            L = 'S';
            pL = 0;
            pR = 0;
        }
    }
return 0;}