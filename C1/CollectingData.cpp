#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll MOD = 1000000007LL;

static ll llgcd(ll a, ll b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b) { ll t = a % b; a = b; b = t; }
    return a;
}

struct LineKey {
    ll A, B, C;
    bool operator==(LineKey const &o) const {
        return A==o.A && B==o.B && C==o.C;
    }
};
struct LineKeyHash {
    size_t operator()(LineKey const &k) const noexcept {
        uint64_t x = (uint64_t)(k.A + 1000003LL);
        uint64_t y = (uint64_t)(k.B + 1000033LL);
        uint64_t z = (uint64_t)(k.C + 1000213LL);
        uint64_t h = x;
        h = (h * 1000003ULL) ^ (y + 0x9e3779b97f4a7c15ULL + (h<<6) + (h>>2));
        h ^= (z + 0x9e3779b97f4a7c15ULL + (h<<6) + (h>>2));
        return (size_t)h;
    }
};

LineKey make_line_key(ll x1, ll y1, ll x2, ll y2) {
    if (x1 == x2) return LineKey{1,0,-x1};   // vertical: x = const
    if (y1 == y2) return LineKey{0,1,-y1};   // horizontal: y = const
    ll A = y2 - y1;
    ll B = x1 - x2;
    ll C = x2 * y1 - x1 * y2;
    ll g = llgcd(llgcd(llabs(A), llabs(B)), llabs(C));
    if (g != 0) { A /= g; B /= g; C /= g; }
    if (A < 0 || (A==0 && B < 0) || (A==0 && B==0 && C < 0)) { A=-A; B=-B; C=-C; }
    return LineKey{A,B,C};
}

// Count ways for a non-vertical/non-horizontal line determined by two points (x1,y1),(x2,y2).
// uniq_vals: sorted unique values; cnt_map: frequency map; idx_map: value->index
ll count_for_line_general(
    const vector<int> &uniq_vals,
    const unordered_map<int,int> &cnt_map,
    const unordered_map<int,int> &idx_map,
    ll x1, ll y1, ll x2, ll y2
) {
    int M = (int)uniq_vals.size();
    vector<int> next_idx(M, -1);
    vector<int> indeg(M,0), outdeg(M,0);

    ll dx = x2 - x1;
    ll dy = y2 - y1;
    if (dx == 0) return 0;

    // Build mapping x -> y (if integer and present)
    for (int i = 0; i < M; ++i) {
        ll x = uniq_vals[i];
        ll numer = (x - x1) * dy;
        if (numer % dx == 0) {
            ll y = y1 + numer / dx;
            auto it = cnt_map.find((int)y);
            if (it != cnt_map.end()) {
                int j = idx_map.at((int)y);
                next_idx[i] = j;
                outdeg[i] = 1;
                indeg[j] += 1;
                // two different x mapping to same y -> impossible
                if (indeg[j] > 1) return 0;
            } else next_idx[i] = -1;
        } else next_idx[i] = -1;
    }

    // If some value is initially isolated (cannot be used as x nor y) -> impossible
    for (int i = 0; i < M; ++i) if (indeg[i] == 0 && outdeg[i] == 0) return 0;

    // counts
    vector<ll> orig_cnt(M), cnt_res(M);
    for (int i=0;i<M;++i) { orig_cnt[i] = cnt_map.at(uniq_vals[i]); cnt_res[i] = orig_cnt[i]; }

    const ll NEG = LLONG_MIN/4;
    vector<ll> f_edge(M, NEG);
    vector<char> processed(M, 0);

    // reverse mapping (previous node) where exists
    vector<int> prev_idx(M, -1);
    for (int i=0;i<M;++i) if (next_idx[i] != -1) prev_idx[next_idx[i]] = i;

    deque<int> fq, bq;
    for (int i=0;i<M;++i) {
        if (indeg[i]==0 && outdeg[i]==1) fq.push_back(i);
        if (outdeg[i]==0 && indeg[i]==1) bq.push_back(i);
    }

    // Forward/backward elimination
    while (!fq.empty() || !bq.empty()) {
        while (!fq.empty()) {
            int u = fq.front(); fq.pop_front();
            if (processed[u]) continue;
            // important: only process if condition still holds; otherwise skip (do NOT mark processed)
            if (!(indeg[u]==0 && outdeg[u]==1)) continue;
            int v = next_idx[u];
            ll f = cnt_res[u];
            if (f < 0) return 0;
            f_edge[u] = f;
            cnt_res[v] -= f;
            if (cnt_res[v] < 0) return 0;
            indeg[v] -= 1;
            outdeg[u] = 0;
            processed[u] = 1;
            if (indeg[v]==0 && outdeg[v]==1) fq.push_back(v);
            if (outdeg[v]==0 && indeg[v]==1) bq.push_back(v);
        }

        while (!bq.empty()) {
            int u = bq.front(); bq.pop_front();
            if (processed[u]) continue;
            // important: only process if condition still holds; otherwise skip
            if (!(indeg[u]==1 && outdeg[u]==0)) continue;
            int p = prev_idx[u];
            if (p == -1) return 0;
            // sink u: f_p + f_u = orig_cnt[u]; after subtractions cnt_res[u] == remaining f_u
            ll fprev = orig_cnt[u] - cnt_res[u];
            if (fprev < 0) return 0;
            if (f_edge[p] != NEG && f_edge[p] != fprev) return 0;
            f_edge[p] = fprev;
            cnt_res[p] -= fprev;
            if (cnt_res[p] < 0) return 0;
            outdeg[p] -= 1;
            indeg[u] = 0;
            processed[u] = 1;
            if (indeg[p]==0 && outdeg[p]==1) fq.push_back(p);
            if (outdeg[p]==0 && indeg[p]==1) bq.push_back(p);
        }
    }

    // AFTER elimination: previously we rejected any unprocessed node that is isolated.
    // That was incorrect: an unprocessed node that is isolated is fine *if* its remaining count is 0
    // (it was fully satisfied by earlier flows). Reject only if residual is nonzero.
    for (int i=0;i<M;++i) {
        if (!processed[i] && indeg[i]==0 && outdeg[i]==0) {
            if (cnt_res[i] != 0) return 0;
        }
    }

    // Remaining unprocessed nodes must form cycles (each indeg==1 and outdeg==1).
    vector<char> visited(M,0);
    ll totalWays = 1;

    for (int i=0;i<M;++i) {
        if (processed[i] || visited[i]) continue;
        // Skip nodes that are isolated (were satisfied) — we already ensured cnt_res==0 above.
        if (indeg[i]==0 && outdeg[i]==0) continue;
        if (!(indeg[i]==1 && outdeg[i]==1)) return 0;

        // collect cycle
        vector<int> cyc;
        int u = i;
        while (!visited[u]) {
            visited[u] = 1;
            cyc.push_back(u);
            u = next_idx[u];
        }

        int L = (int)cyc.size();
        vector<ll> alt(L,0);
        for (int j=1;j<L;++j) alt[j] = cnt_res[cyc[j]] - alt[j-1];

        if (L % 2 == 1) {
            // odd cycle -> unique integer solution
            ll val = cnt_res[cyc[0]] - alt[L-1];
            if ((val & 1LL) != 0) return 0;
            ll t = val / 2;
            for (int j=0;j<L;++j) {
                ll fj = ((j%2==0) ? (t + alt[j]) : (-t + alt[j]));
                if (fj < 0) return 0;
                int node = cyc[j];
                if (f_edge[node] != NEG && f_edge[node] != fj) return 0;
                f_edge[node] = fj;
            }
            // exactly one way
        } else {
            // even cycle -> integer interval of solutions
            if (cnt_res[cyc[0]] != alt[L-1]) return 0;
            ll lower = LLONG_MIN/4, upper = LLONG_MAX/4;
            for (int j=0;j<L;++j) {
                if (j%2==0) lower = max(lower, -alt[j]);
                else upper = min(upper, alt[j]);
            }
            if (upper < lower) return 0;
            ll ways = upper - lower + 1;
            totalWays = (totalWays * (ways % MOD)) % MOD;
        }
    }

    return totalWays % MOD;
}

// Vertical / horizontal counting helper: valid only when some value appears >= N/2 times.
inline ll count_for_vertical_or_horizontal(int c, int N, const unordered_map<int,int> &cnt_map) {
    auto it = cnt_map.find(c);
    if (it == cnt_map.end()) return 0;
    if (it->second >= N/2) return 1;
    return 0;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    if (!(cin >> N)) return 0;
    vector<int> vals(N);
    for (int i = 0; i < N; ++i) cin >> vals[i];
    sort(vals.begin(), vals.end());

    // trivial: all equal -> 1 way
    if (vals.front() == vals.back()) {
        cout << 1 << '\n';
        return 0;
    }

    unordered_map<int,int> cnt_map;
    for (int v : vals) cnt_map[v]++;

    vector<int> uniq_vals;
    uniq_vals.reserve(cnt_map.size());
    for (auto &kv : cnt_map) uniq_vals.push_back(kv.first);
    sort(uniq_vals.begin(), uniq_vals.end());

    int MM = N / 2;
    if (uniq_vals.size() == 2) {
        int c1 = cnt_map[uniq_vals[0]];
        int c2 = cnt_map[uniq_vals[1]];
        if (c1 == MM && c2 == MM) {
            ll ways = MM + 1LL;
            if (MM % 2 == 0) ways += 1LL;
            cout << (ways % MOD) << '\n';
            return 0;
        }
    }

    if (uniq_vals.size() < 2) {
        cout << 0 << '\n';
        return 0;
    }

    // choose v1 as smallest unique; iterate v2 over every unique > v1 (safe)
    int v1 = uniq_vals[0];
    int v2 = uniq_vals[1];

    unordered_map<int,int> idx_map;
    for (int i = 0; i < (int)uniq_vals.size(); ++i) idx_map[uniq_vals[i]] = i;

    // deduplicate candidate lines
    unordered_set<LineKey, LineKeyHash> seen;
    ll answer = 0;

    // Add vertical/horizontal possibilities (if applicable)
    for (int v : uniq_vals) {
        if (cnt_map[v] >= N/2) {
            LineKey vk{1,0,-(ll)v};
            if (!seen.count(vk)) { seen.insert(vk); answer = (answer + count_for_vertical_or_horizontal(v,N,cnt_map)) % MOD; }
            LineKey hk{0,1,-(ll)v};
            if (!seen.count(hk)) { seen.insert(hk); answer = (answer + count_for_vertical_or_horizontal(v,N,cnt_map)) % MOD; }
        }
    }

    // generate candidate lines: for each unique v2 > v1, try four orientations and all a,b from the full multiset
    // But optimized to only the second smallest v2
    for (int pos1 = 0; pos1 < 2; ++pos1) {
        for (int pos2 = 0; pos2 < 2; ++pos2) {
            for (int ai = 0; ai < N; ++ai) {
                int a_val = vals[ai];
                for (int bi = 0; bi < N; ++bi) {
                    int b_val = vals[bi];
                    ll x1 = (pos1==0 ? v1 : a_val);
                    ll y1 = (pos1==0 ? a_val : v1);
                    ll x2 = (pos2==0 ? v2 : b_val);
                    ll y2 = (pos2==0 ? b_val : v2);
                    if (x1 == x2 && y1 == y2) continue;
                    LineKey key = make_line_key(x1,y1,x2,y2);
                    if (seen.count(key)) continue;
                    seen.insert(key);
                    // skip vertical/horizontal (already counted)
                    if (key.A == 1 && key.B == 0) continue;
                    if (key.A == 0 && key.B == 1) continue;
                    ll ways = count_for_line_general(uniq_vals, cnt_map, idx_map, x1, y1, x2, y2);
                    if (ways) answer = (answer + ways) % MOD;
                }
            }
        }
    }

    // Special handling for cases where v1 and v2 are in the same point
    for (int orient = 0; orient < 2; ++orient) {
        ll fx = (orient == 0 ? v1 : v2);
        ll fy = (orient == 0 ? v2 : v1);
        for (int ci = 0; ci < N; ++ci) {
            int c_val = vals[ci];
            for (int di = 0; di < N; ++di) {
                int d_val = vals[di];
                if (c_val == fx && d_val == fy) continue;
                LineKey key = make_line_key(fx, fy, c_val, d_val);
                if (seen.count(key)) continue;
                seen.insert(key);
                if (key.A == 1 && key.B == 0) continue;
                if (key.A == 0 && key.B == 1) continue;
                ll ways = count_for_line_general(uniq_vals, cnt_map, idx_map, fx, fy, c_val, d_val);
                answer = (answer + ways) % MOD;
            }
        }
    }

    cout << (answer % MOD) << '\n';
    return 0;
}