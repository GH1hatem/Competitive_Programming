#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
#define vi vector<int>
#define int ll
#define all(x) x.begin(), x.end()
const int N = 2e7 + 100;
int lpd[N];

void sieve() {
  for (int i = 1; i < N; i++) {
    lpd[i] = i;
  }
  for (int i = 2; i * i < N; i++) {
    if (lpd[i] != i) continue;
    for (int j = i; j < N; j += i) {
      if (lpd[j] == j) lpd[j] = i;
    }
  }
}
bool primes(int x){
  return lpd[x] == x ; 
}

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
signed main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  sieve();

  int n;
  cin >> n;
  vector<int> a(n);
  int nb = -1;
  int c = 0;
  for (int i = 0; i < n; i++) {
    cin >> a[i];
    if (a[i] == 1) nb = i, c++;
  }
  vector<vector<int>> g(n);
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      if (a[i] == 1 || a[j] == 1) continue;
      int x = a[i] + a[j];
      if (primes(x)) {
        g[i].push_back(j);
        g[j].push_back(i);
      }
    }
  }
  vector<int> btoa(n, -1);
  int M = n - hopcroftKarp(g, btoa) / 2 - c;

  if (nb >= 0) {
    for (int i = 0; i < n; i++) {
      if (a[i] != 1) {
        int x = a[i] + 1;
        if (primes(x)) {
          g[i].push_back(nb);
          g[nb].push_back(i);
        }
      }
    }
  }

  int M1 = 0;
  vector<int> btob(n, -1);
  if (c > 0) M1 = n - hopcroftKarp(g, btob) / 2 - c + 1;
  cout << max(M1, M) << endl;
}