#include <bits/stdc++.h>
using namespace std;
#define all(x) x.begin(), x.end()
#define rall(x) (x).rbegin(), (x).rend()
#define vi vector<int>
typedef long long ll;
typedef long double ld;
#define int ll

bool dfs(int a, int L, vector<vi>& g, vi& btoa, vi& A, vi& B) {
  if (A[a] != L) return 0;
  A[a] = -1;
  for (int b : g[a])
    if (B[b] == L + 1) {
      B[b] = 0;
      if (btoa[b] == -1 || dfs(btoa[b], L + 1, g, btoa, A, B))
        return btoa[b] = a, 1;
    }
  return 0;
}
int hopcroftKarp(vector<vi>& g, vi& btoa) {
  int res = 0;
  vi A(g.size()), B(btoa.size()), cur, next;
  for (;;) {
    fill(all(A), 0);
    fill(all(B), 0);
    cur.clear();
    for (int a : btoa)
      if (a != -1) A[a] = -1;
    for (int a = 0; a < g.size(); a++)
      if (A[a] == 0) cur.push_back(a);
    for (int lay = 1;; lay++) {
      bool islast = 0;
      next.clear();
      for (int a : cur)
        for (int b : g[a]) {
          if (btoa[b] == -1) {
            B[b] = lay;
            islast = 1;
          } else if (btoa[b] != a && !B[b]) {
            B[b] = lay;
            next.push_back(btoa[b]);
          }
        }
      if (islast) break;
      if (next.empty()) return res;
      for (int a : next) A[a] = lay;
      cur.swap(next);
    }
    for (int a = 0; a < g.size(); a++) res += dfs(a, 0, g, btoa, A, B);
  }
}
void solve() {
  int n, m;
  cin >> n >> m;
  int a[n][m];
  set<int> hi;
  ll sum = 0;
  int al = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      cin >> a[i][j];
      sum += a[i][j];
      if (a[i][j] != 0) al++;
      hi.insert(a[i][j]);
    }
  }
  map<int, vector<int>> mpr, mpc;
  for (int i = 0; i < n; i++) {
    int M = 0;
    for (int j = 0; j < m; j++) {
      M = max(M, a[i][j]);
    }
    mpr[M].push_back(i);
  }
  for (int i = 0; i < m; i++) {
    int M = 0;
    for (int j = 0; j < n; j++) {
      M = max(M, a[j][i]);
    }
    mpc[M].push_back(i);
  }
  int ans = 0;
  int filled = 0;
  for (auto e : hi) {
    if (e > 0) {
      vector<vector<int>> g(202);
      for (auto r : mpr[e]) {
        for (auto c : mpc[e]) {
          if (a[r][c] != 0LL) {
            g[r].push_back(c + 100);
            g[c + 100].push_back(r);
          }
        }
      }
      vi btoa(202, -1);
      int X = hopcroftKarp(g, btoa);
      ans += e * (mpr[e].size() + mpc[e].size() - X / 2);
      filled += mpr[e].size() + mpc[e].size() - X / 2;
    }
  }

  cout << sum - ans - (al - filled) << endl;
}
signed main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  cout.tie(0);
  int tt = 1;
  // cin >> tt;
  while (tt--) {
    solve();
  }
}