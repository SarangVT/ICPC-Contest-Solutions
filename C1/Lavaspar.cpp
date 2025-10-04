#include <bits/stdc++.h>
using namespace std;
#define vi vector<int>
#define vvi vector<vector<int>>
#define ff first
#define ss second
#define fl(i,a,b) for(int i=a;i<b;++i)
#define flb(i,a,b) for(int i=a;i>=b;--i)
#define vii vector<pair<int,int>>
#define pii pair<int,int>
typedef long long ll;

bool matchChar(vi &first, vi &last, vi &word) {
    fl(i, 0, 26) if (first[i] - last[i] != word[i]) return 0;
    return 1;
}

int countSpecial(vector<vector<set<int>>> &count, int l, int c) {
    int ans = 0;
    fl(i, 0, l) fl(j, 0, c) if (count[i][j].size() > 1) ans++;
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int l, c; cin >> l >> c;
    vector<vector<char>> grid(l, vector<char>(c));
    vector<vector<set<int>>> count(l, vector<set<int>>(c));
    fl(i, 0, l) fl(j, 0, c) cin >> grid[i][j];

    int n; cin >> n;
    vvi words(n, vi(26));
    vector<string> word(n);
    fl(i, 0, n) {
        cin >> word[i];
        fl(j, 0, word[i].size()) words[i][word[i][j] - 'A']++;
    }

    fl(i, 0, l) {
        vvi pref(c+1, vi(26));
        fl(j, 1, c+1) {
            pref[j] = pref[j-1];
            pref[j][grid[i][j-1]-'A']++;
        }
        fl(k, 0, n) {
            int sz = word[k].size();
            fl(m, sz, c+1) {
                if (matchChar(pref[m], pref[m-sz], words[k])) {
                    fl(t, m-sz, m) count[i][t].insert(k);
                }
            }
        }
    }

    fl(i, 0, c) {
        vvi pref(l+1, vi(26));
        fl(j, 1, l+1) {
            pref[j] = pref[j-1];
            pref[j][grid[j-1][i]-'A']++;
        }
        fl(k, 0, n) {
            int sz = word[k].size();
            fl(m, sz, l+1) {
                if (matchChar(pref[m], pref[m-sz], words[k])) {
                    fl(t, m-sz, m) count[t][i].insert(k);
                }
            }
        }
    }

    fl(start, 0, l) {
        vector<pii> coords;
        int i = start, j = 0;
        while (i < l && j < c) {
            coords.push_back({i, j});
            i++; j++;
        }
        int len = coords.size();
        vvi pref(len+1, vi(26));
        for (int idx = 1; idx <= len; idx++) {
            pref[idx] = pref[idx-1];
            pref[idx][grid[coords[idx-1].ff][coords[idx-1].ss]-'A']++;
        }
        fl(k, 0, n) {
            int sz = word[k].size();
            fl(m, sz, len+1) {
                if (matchChar(pref[m], pref[m-sz], words[k])) {
                    fl(t, m-sz, m) {
                        auto [x,y] = coords[t];
                        count[x][y].insert(k);
                    }
                }
            }
        }
    }
    
    fl(start, 1, c) {
        vector<pii> coords;
        int i = 0, j = start;
        while (i < l && j < c) {
            coords.push_back({i, j});
            i++; j++;
        }
        int len = coords.size();
        vvi pref(len+1, vi(26));
        fl(idx, 1, len+1) {
            pref[idx] = pref[idx-1];
            pref[idx][grid[coords[idx-1].ff][coords[idx-1].ss]-'A']++;
        }
        fl(k, 0, n) {
            int sz = word[k].size();
            fl(m, sz, len+1) {
                if (matchChar(pref[m], pref[m-sz], words[k])) {
                    fl(t, m-sz, m) {
                        auto [x,y] = coords[t];
                        count[x][y].insert(k);
                    }
                }
            }
        }
    }

    fl(start, 0, l) {
        vector<pii> coords;
        int i = start, j = c-1;
        while (i < l && j >= 0) {
            coords.push_back({i, j});
            i++; j--;
        }
        int len = coords.size();
        vvi pref(len+1, vi(26));
        fl(idx, 1, len+1) {
            pref[idx] = pref[idx-1];
            pref[idx][grid[coords[idx-1].ff][coords[idx-1].ss]-'A']++;
        }
        fl(k, 0, n) {
            int sz = word[k].size();
            fl(m, sz, len+1) {
                if (matchChar(pref[m], pref[m-sz], words[k])) {
                    fl(t, m-sz, m) {
                        auto [x,y] = coords[t];
                        count[x][y].insert(k);
                    }
                }
            }
        }
    }
    flb(start, c-2, 0) {
        vector<pii> coords;
        int i = 0, j = start;
        while (i < l && j >= 0) {
            coords.push_back({i, j});
            i++; j--;
        }
        int len = coords.size();
        vvi pref(len+1, vi(26));
        fl(idx, 1, len+1) {
            pref[idx] = pref[idx-1];
            pref[idx][grid[coords[idx-1].ff][coords[idx-1].ss]-'A']++;
        }
        fl(k, 0, n) {
            int sz = word[k].size();
            fl(m, sz, len+1) {
                if (matchChar(pref[m], pref[m-sz], words[k])) {
                    fl(t, m-sz, m) {
                        auto [x,y] = coords[t];
                        count[x][y].insert(k);
                    }
                }
            }
        }
    }
    cout<<countSpecial(count, l, c);
return 0;}