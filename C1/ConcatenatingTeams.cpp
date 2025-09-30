#include <bits/stdc++.h>
using namespace std;

struct Trie {
    vector<array<int,26>> nxt;
    vector<int> term;
    Trie(){ nxt.emplace_back(); nxt.back().fill(-1); term.push_back(-1); }
    void insert(const string &w, int id){
        int v = 0;
        for(char ch : w){
            int c = ch - 'a';
            if (nxt[v][c] == -1){
                nxt[v][c] = nxt.size();
                nxt.emplace_back();
                nxt.back().fill(-1);
                term.push_back(-1);
            }
            v = nxt[v][c];
        }
        term[v] = id;
    }
    int step(int v, char ch){
        return nxt[v][ch - 'a'];
    }
};

// --- suffix array (doubling + counting sort) on integer alphabet ---
vector<int> build_sa(const vector<int>& s){
    int n = s.size();
    vector<int> sa(n), rnk(n), tmp(n);
    for (int i = 0; i < n; ++i){ sa[i] = i; rnk[i] = s[i]; }
    for (int k = 1;; k <<= 1){
        int maxv = *max_element(rnk.begin(), rnk.end());
        int C = max(maxv + 2, n + 1);
        vector<int> sa2(n), cnt(C, 0);

        for (int i = 0; i < n; ++i){
            int key = (sa[i] + k < n ? rnk[sa[i] + k] + 1 : 0);
            cnt[key]++;
        }
        for (int i = 1; i < C; ++i) cnt[i] += cnt[i-1];
        for (int i = n-1; i >= 0; --i){
            int key = (sa[i] + k < n ? rnk[sa[i] + k] + 1 : 0);
            sa2[--cnt[key]] = sa[i];
        }

        fill(cnt.begin(), cnt.end(), 0);
        for (int i = 0; i < n; ++i){
            int key = rnk[sa2[i]] + 1;
            cnt[key]++;
        }
        for (int i = 1; i < C; ++i) cnt[i] += cnt[i-1];
        for (int i = n-1; i >= 0; --i){
            int key = rnk[sa2[i]] + 1;
            sa[--cnt[key]] = sa2[i];
        }

        tmp[sa[0]] = 0;
        for (int i = 1; i < n; ++i){
            int a = sa[i-1], b = sa[i];
            int a1 = rnk[a], a2 = (a + k < n ? rnk[a+k] : -1);
            int b1 = rnk[b], b2 = (b + k < n ? rnk[b+k] : -1);
            tmp[b] = tmp[a] + (a1 != b1 || a2 != b2);
        }
        rnk = tmp;
        if (rnk[sa[n-1]] == n-1) break;
    }
    return sa;
}

vector<int> build_lcp(const vector<int>& svec, const vector<int>& sa){
    int n = svec.size();
    vector<int> rank(n);
    for (int i = 0; i < n; ++i) rank[sa[i]] = i;
    vector<int> lcp(max(0, n-1));
    int h = 0;
    for (int i = 0; i < n; ++i){
        int r = rank[i];
        if (r == 0) continue;
        int j = sa[r-1];
        while (i + h < n && j + h < n && svec[i+h] == svec[j+h]) ++h;
        lcp[r-1] = h;
        if (h) --h;
    }
    return lcp;
}

struct RMQ {
    int n, LOG;
    vector<vector<int>> st;
    vector<int> lg;
    void build(const vector<int> &a){
        n = a.size();
        if (n == 0) return;
        lg.assign(n+1, 0);
        for (int i = 2; i <= n; ++i) lg[i] = lg[i>>1] + 1;
        LOG = lg[n] + 1;
        st.assign(LOG, vector<int>(n));
        for (int i = 0; i < n; ++i) st[0][i] = a[i];
        for (int k = 1; k < LOG; ++k){
            for (int i = 0; i + (1<<k) <= n; ++i){
                st[k][i] = min(st[k-1][i], st[k-1][i + (1<<(k-1))]);
            }
        }
    }
    inline int query(int l, int r){
        if (l > r) return INT_MAX;
        int k = lg[r - l + 1];
        return min(st[k][l], st[k][r - (1<<k) + 1]);
    }
};

struct Trip {
    int start;
    int len;
    bool isA;
    int sidx, lidx;
};

// pair-hash for unordered_map key
struct PairHash {
    size_t operator()(const pair<unsigned long long, unsigned long long>&p) const noexcept {
        return (size_t)(p.first ^ (p.second + 0x9e3779b97f4a7c15ULL + (p.first<<6) + (p.first>>2)));
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int M, N;
    if (!(cin >> M >> N)) return 0;
    vector<string> A(M), B(N);
    for (int i = 0; i < M; ++i) cin >> A[i];
    for (int j = 0; j < N; ++j) cin >> B[j];

    // Build global integer array Gvec with separators
    vector<int> posA(M), posB(N);
    vector<int> Gvec;
    Gvec.reserve( (int)1e6 + M + N + 5 );
    auto pushA = [&](const string &w, int idx){
        posA[idx] = (int)Gvec.size();
        for (char c : w) Gvec.push_back((int)(c - 'a') + 3);
        Gvec.push_back(1);
    };
    auto pushB = [&](const string &w, int idx){
        posB[idx] = (int)Gvec.size();
        for (char c : w) Gvec.push_back((int)(c - 'a') + 3);
        Gvec.push_back(2);
    };
    for (int i = 0; i < M; ++i) pushA(A[i], i);
    for (int j = 0; j < N; ++j) pushB(B[j], j);

    // Tries
    Trie trieA;
    for (int i = 0; i < M; ++i) trieA.insert(A[i], i);
    Trie trieBr;
    for (int j = 0; j < N; ++j){
        string r = B[j];
        reverse(r.begin(), r.end());
        trieBr.insert(r, j);
    }

    // collect triplets
    vector<Trip> subs;
    subs.reserve(Gvec.size()/4 + 10);

    for (int v = 0; v < M; ++v){
        const string &w = A[v];
        int node = 0;
        for (int p = 0; p < (int)w.size(); ++p){
            node = trieA.step(node, w[p]);
            if (node == -1) break;
            int t = trieA.term[node];
            if (t != -1 && t != v){
                int prefix_len = p + 1;
                int s_len = (int)w.size() - prefix_len;
                if (s_len > 0){
                    int s_start = posA[v] + prefix_len;
                    subs.push_back({s_start, s_len, true, t, v});
                }
            }
        }
    }

    for (int idx = 0; idx < N; ++idx){
        const string &w = B[idx];
        int node = 0;
        for (int p = 0; p < (int)w.size(); ++p){
            char ch = w[(int)w.size() - 1 - p];
            node = trieBr.step(node, ch);
            if (node == -1) break;
            int t = trieBr.term[node];
            if (t != -1 && t != idx){
                int suffix_len = p + 1;
                int s_len = (int)w.size() - suffix_len;
                if (s_len > 0){
                    int s_start = posB[idx];
                    subs.push_back({s_start, s_len, false, t, idx});
                }
            }
        }
    }

    if (subs.empty()){
        cout << M << " " << N << "\n";
        return 0;
    }

    // Build SA + LCP + RMQ
    auto sa = build_sa(Gvec);
    auto lcp = build_lcp(Gvec, sa);
    int Gn = (int)Gvec.size();
    vector<int> rankPos(Gn);
    for (int i = 0; i < Gn; ++i) rankPos[sa[i]] = i;
    RMQ rmq; rmq.build(lcp);

    auto lcp_between = [&](int p, int q)->int{
        if (p == q) return Gn - p;
        int rp = rankPos[p], rq = rankPos[q];
        if (rp > rq) swap(rp, rq);
        return rmq.query(rp, rq - 1);
    };

    // Precompute two 64-bit rolling hashes (no mod, rely on natural uint64 overflow)
    const unsigned long long B1 = 1315423911ULL; // distinct bases
    const unsigned long long B2 = 11400714819323198485ULL; // different big base
    int S = Gn;
    vector<unsigned long long> pre1(S+1,0), pre2(S+1,0), pw1(S+1,1), pw2(S+1,1);
    for (int i = 0; i < S; ++i){
        pre1[i+1] = pre1[i] * B1 + (unsigned long long)(Gvec[i] + 1);
        pre2[i+1] = pre2[i] * B2 + (unsigned long long)(Gvec[i] + 1);
        pw1[i+1] = pw1[i] * B1;
        pw2[i+1] = pw2[i] * B2;
    }
    auto get_hash = [&](int start, int len)->pair<unsigned long long, unsigned long long>{
        unsigned long long h1 = pre1[start+len] - pre1[start] * pw1[len];
        unsigned long long h2 = pre2[start+len] - pre2[start] * pw2[len];
        return {h1, h2};
    };

    // Bucket triplets by hash
    unordered_map<pair<unsigned long long,unsigned long long>, vector<int>, PairHash> buckets;
    buckets.reserve(subs.size()*2 + 10);
    for (int i = 0; i < (int)subs.size(); ++i){
        auto &t = subs[i];
        auto h = get_hash(t.start, t.len);
        buckets[h].push_back(i);
    }

    vector<char> nonA(M, 0), nonB(N, 0);

    // Process each bucket: sort indices by SA rank of start (cheap), then group contiguous equal substrings
    for (auto &it : buckets){
        auto &vec = it.second;
        if (vec.empty()) continue;
        sort(vec.begin(), vec.end(), [&](int a, int b){
            if (rankPos[subs[a].start] != rankPos[subs[b].start]) return rankPos[subs[a].start] < rankPos[subs[b].start];
            return subs[a].len < subs[b].len;
        });

        // linear scan per bucket: group equal substrings (use lcp_between to verify)
        for (int i = 0, K = (int)vec.size(); i < K; ){
            int j = i + 1;
            int idx0 = vec[i];
            int st = subs[idx0].start;
            int ln = subs[idx0].len;
            // expand while same length and LCP >= ln
            while (j < K && subs[vec[j]].len == ln && lcp_between(st, subs[vec[j]].start) >= ln) ++j;

            bool hasA=false, hasB=false;
            for (int t = i; t < j; ++t){
                if (subs[vec[t]].isA) hasA = true; else hasB = true;
            }
            if (hasA && hasB){
                for (int t = i; t < j; ++t){
                    auto &tr = subs[vec[t]];
                    if (tr.isA){
                        nonA[tr.sidx] = 1;
                        nonA[tr.lidx] = 1;
                    } else {
                        nonB[tr.sidx] = 1;
                        nonB[tr.lidx] = 1;
                    }
                }
            }
            i = j;
        }
    }

    int cntA=0, cntB=0;
    for (int i = 0; i < M; ++i) if (!nonA[i]) ++cntA;
    for (int j = 0; j < N; ++j) if (!nonB[j]) ++cntB;
    cout << cntA << " " << cntB << "\n";
    return 0;
}
