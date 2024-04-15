#include <bits/stdc++.h>
using namespace std;

int min_weight_for_value(int N, int K, vector<pair<int, int>>& items) {
    const int INF = INT_MAX / 2;
    int max_value = 0;
    for (const auto& item : items) {
        max_value += item.first;
    }
    
    vector<vector<int>> dp(N + 1, vector<int>(max_value + 1, INF));
    dp[0][0] = 0;

    for (int i = 1; i <= N; ++i) {
        int value = items[i - 1].first;
        int weight = items[i - 1].second;
        for (int j = 0; j <= max_value; ++j) {
            dp[i][j] = dp[i - 1][j];
            if (j >= value) {
                dp[i][j] = min(dp[i][j], dp[i - 1][j - value] + weight);
            }
        }
    }

    int min_weight = INF;
    for (int j = K; j <= max_value; ++j) {
        if (dp[N][j] < min_weight) {
            min_weight = dp[N][j];
        }
    }

    return min_weight == INF ? -1 : min_weight;
}

int main() {
    int N, K;
    cin >> N >> K;
    vector<pair<int, int>> items(N);
    for (int i = 0; i < N; ++i) {
        cin >> items[i].first >> items[i].second;
    }
    int result = min_weight_for_value(N, K, items);
    if (result == -1) {
        cout << "Impossible" << endl;
    } else {
        cout << result << endl;
    }
    return 0;
}
