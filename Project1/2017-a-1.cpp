#include<iostream>
#include <vector>
#include <iostream>
#include <cstring>
#include <string>
using namespace std;
string str[10];
int vis[10][10]; // 用于记录本路径上是否已访问
int cnt = 0;     // 记录有多少个起点能走出迷宫

// 统一使用 (x,y) 作为坐标，用 str 存迷宫，用 vis 记录当前 DFS 路径上的访问
bool dfs(int x, int y, int vis[10][10])
{
    if (x < 0 || x > 9 || y < 0 || y > 9)
        return true;
    if (vis[x][y] == 1)
        return false;

    vis[x][y] = 1; // 标记为已访问
    switch (str[x][y]) {
    case 'U':
        return dfs(x - 1, y, vis);
    case 'D':
        return dfs(x + 1, y, vis);
    case 'L':
        return dfs(x, y - 1, vis);
    case 'R':
        return dfs(x, y + 1, vis);
    default:
        return false;
    }
}


int main()
{
	// 初始化迷宫（10 行字符串）
    str[0] = "UDDLUULRUL";
    str[1] = "UURLLLRRRU";
    str[2] = "RRUURLDLRD";
    str[3] = "RUDDDDUUUU";
    str[4] = "URUDLLRRUU";
    str[5] = "DURLRLDLRL";
    str[6] = "ULLURLLRDU";
    str[7] = "RDLULLRDDD";
    str[8] = "UUDDUDUDLL";
    str[9] = "ULRDLUURRR";

	// 以每个格子为起点尝试走出边界
	for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++)
		{
			memset(vis, 0, sizeof(vis)); // 每次新起点前清空 vis，避免上一次搜索污染
			bool flag = dfs(i, j, vis);
			if (flag)
			{
				cnt++;
			}
		}
	}
	cout << cnt << endl;

}
