#include <iostream>       // 标准输入输出（cout）
#include <queue>          // 队列 queue，存放 BFS 待扩展状态
#include <string>         // 字符串 string，用 9 个字符表示盘面
#include <unordered_set>  // 哈希集合，记录已出现过的盘面，避免重复扩展

using namespace std;    // 使用 std 命名空间，省去 std:: 前缀

// BFS 队列里的一个结点：当前盘面 + 从起点走到这的步数
struct Node {
    string board; // 9 位字符串，'0' 为空位，'1'～'8' 为蚱蜢
    int level;    // 已跳跃次数
};

// 起点：下标 0 为空位 '0'，下标 1～8 依次为蚱蜢 1～8（与教程状态编码一致）
const string kStart = "012345678";
// 终点：空位仍在下标 0，顺时针顺序变为 8～1（逆时针与起点对调）
const string kTarget = "087654321";

// 判断：蚱蜢在环上位置 g，空位在 e，能否按规则一步跳到空位
bool canJump(const string& s, int g, int e)
{
    // 同一格、蚱蜢位置却是空、或落点不是空，则不能跳
    if (g == e || s[g] == '0' || s[e] != '0')
        return false;
    // d1：从 g 沿下标增大方向绕环走到 e 的步数（0～8）
    int d1 = (e - g + 9) % 9;
    // d2：从 e 沿同一方向走到 g 的步数（与 d1 配套，用于「逆时针隔一跳」）
    int d2 = (g - e + 9) % 9;
    // d1 为 1 或 8 表示 g 与 e 相邻（短弧一步），允许「挪到相邻空位」
    if (d1 == 1 || d1 == 8)//这里就相当于考虑了d1=1和d2=1的情况
        return true;
    // 顺时针方向隔一格到 e：中间格 (g+1)%9 必须是蚱蜢，才是「越过一只」
    if (d1 == 2 && s[(g + 1) % 9] != '0')
        return true;
    // 逆时针方向隔一格到 e：中间格 (g-1+9)%9 必须是蚱蜢
    if (d2 == 2 && s[(g - 1 + 9) % 9] != '0')
        return true;
    // 其余情况不符合相邻或隔一跳
    return false;
}
//总之这个函数就是看你能不能从g跳到e
int main()
{
    queue<Node> q;                // BFS 队列：每个元素是一个 Node（盘面 + 步数）
    unordered_set<string> vis;    // 已访问盘面集合，用于判重

    q.push(Node{ kStart, 0 });    // 初始状态入队，步数为 0
    vis.insert(kStart);           // 标记起点已访问

    while (!q.empty()) {          // 队列非空则继续搜
        Node cur = q.front();     // 取出队首结点
        q.pop();
        string s = cur.board;     // 当前盘面
        int level = cur.level;    // 从起点到当前盘面的跳跃次数
//先把当前节点的状态取出来
        if (s == kTarget) {       // 已到达目标排列
            cout << level << endl; // 输出最少步数
            return 0;              // 结束程序
        }

        int e = static_cast<int>(s.find('0')); // 当前空位在环上的下标 e先找到空位E
        for (int g = 0; g < 9; ++g) {          // 枚举每只蚱蜢可能在的位置 g
            if (!canJump(s, g, e))             // 不能从 g 一步跳到空位 e 则跳过
                continue;//跳不过去的话这种状态就不管了
            string nxt = s;                    // 复制当前盘面
            swap(nxt[g], nxt[e]);              // 蚱蜢进空位：交换两格字符，得到新盘面
            if (vis.insert(nxt).second)        // 若 nxt 是全新状态（插入成功）
                q.push(Node{ nxt, level + 1 }); // 新状态入队，步数 +1
        }
    }

    return 0;                     // 理论上必可达目标，此处兜底
}
