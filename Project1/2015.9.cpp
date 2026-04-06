#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>

using namespace std;

int dfs(char *s, int left, int right, int cnt);

char s[10000];

int main1(int argc, const char *argv[]) {
    if (!(cin >> s)) {
        return 0;
    }
    int len = static_cast<int>(strlen(s));
    int ans = dfs(s, 0, len - 1, 0);
    cout << ans << '\n';
    return 0;
}

// 两端不等：在左侧或右侧补一个字符使对称，取最少补字符次数
int dfs(char *s, int left, int right, int cnt) {
   if (left >= right)
        return cnt;
    if (*(s + left) != *(s + right)) {
        return min(dfs(s, left + 1, right, cnt + 1), dfs(s, left, right - 1, cnt + 1));
    }
    return dfs(s, left + 1, right - 1, cnt);
}
