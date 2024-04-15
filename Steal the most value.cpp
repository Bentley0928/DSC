#include <bits/stdc++.h>
using namespace std;
int main(){
    int n,k;
    cin >> n >> k;
    vector<int> v,w;
    vector<vector<int>> dp(n+1, vector<int> (k+1, 0));
    for(int i=0; i<n; i++){
        int a,b;
        cin >> a >> b;
        v.push_back(a);
        w.push_back(b);
    }
     for (int i=1; i<=n; i++) {
        for (int j=0; j<=k; j++) {
            dp[i][j] = dp[i-1][j];
            if (w[i-1] <= j) {
                dp[i][j] = max(dp[i][j], dp[i-1][j-w[i-1]] + v[i-1]);
            }
        }
    }
    cout << dp[n][k] << endl;
    return 0;
}