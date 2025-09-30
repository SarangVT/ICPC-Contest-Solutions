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

    int t; cin >> t;
    fl(p,0,t) {
        int redC, blackC, strip, total;
        cin>>redC>>blackC>>strip>>total;
        vi red(redC), black(blackC + 2);
        fl(i, 0, redC) cin>>red[i];
        fl(i, 0, blackC) cin>>black[i];
        sort(red.begin(), red.end());
        sort(black.begin(), black.end());
        black.insert(black.begin(), 0);
        black.push_back(total + 1);
        vi str;
        bool pos = 1;
        int idxRed = 0;
        for(int i=1; i<black.size(); i++) {
            int l = black[i-1]+1;
            int r = black[i]-1;
            vi temp;
            while(idxRed < redC && red[idxRed] >= l && red[idxRed] <= r) {
                temp.push_back(red[idxRed]);
                idxRed++;
            }
            if(temp.empty()) continue;
            int j = 0;
            while(j < temp.size()) {
                int left = temp[j];
                int right = left + strip - 1;
                if(right > r) {
                    int extra = right - r;
                    int sz = str.size();
                    for(int k=sz-1; k>=0 && extra>0; k--) {
                        int canMove = (k==0 ? 0 : str[k] - (str[k-1] + strip));
                        int move = min(canMove, extra);
                        str[k] -= move;
                        extra -= move;
                    }
                    if(extra > 0) {
                        pos = false;
                        break;
                    }
                    right = left + strip - 1 - extra;
                }
                str.push_back(left);
                while(j < temp.size() && temp[j] <= left + strip - 1) j++;
            }
            if(!pos) break;
        }
        if(!pos) cout<<-1<<'\n';
        else {
            cout << str.size() << '\n';
            for(int x : str) cout << x << " ";
            cout <<'\n';
        }
    }
return 0;}