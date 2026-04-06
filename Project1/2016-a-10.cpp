#include <iostream>
#include <algorithm>
#include <vector>
#include <climits>
#include <cmath>
using namespace std;
typedef long long ll;
ll arr[100];
int N;

struct ratio//定义一个按照分数存储的结构体
{
	/*逐项讲清楚：
		•	Ratio(ll _a, ll _b)：构造函数头，接受两个参数 _a 和 _b。
		• : ：开始成员初始化列表，表示“下面要直接初始化成员”，不是在函数体里赋值。
		•	a(_a)：把成员 a 直接用 _a 初始化（调用 a 的构造器或对内置类型直接赋初值）。
		•	b(_b)：同理初始化成员 b。多个成员用逗号分隔。
		•{ }：构造函数体，初始化完成后执行；此例中为空，因为初始化已经完成。*/
	long long a;
	long long b;
	long long gcd(long long a, long long b)//这里实现求最大公约数的函数
	{
		if (b == 0) { return a; }
		return gcd(b, a % b);
	}
	//给一个构造器
	ratio(long long _a, long long _b) :a(_a), b(_b) {//这里的下划线只是为了区分成员变量和参数变量，名字可以一样的
		long long _gcd = gcd(a, b);//这里这两个不能同名
		a /= _gcd;
		b /= _gcd;//这里实现约分
		/*•	在函数体里赋值（先默认构造再赋值）：Ratio(ll _a, ll _b) { a = _a; b = _b; } —— 可能多一次构造 / 赋值，且对 const / 引用无效*/

	}
};

/*
 * extract(x, pow)
 * 含义：对正整数 x 做「开 (pow+1) 次方」——找一个整数底数 base，使得
 *       base^(pow+1) == x。
 *       - pow == 0 时，(pow+1)==1，任意 x 都满足 x^1==x，因此直接返回 x。
 *       - 若不存在这样的整数 base，返回 -1（说明 x 不是某个整数的 (pow+1) 次幂）。
 * 实现：在 [1, x] 上对底数二分；用「连乘并防溢出」比较 mid^(pow+1) 与 x。
 */
long long extract(long long x, int pow) {
	int k = pow + 1; // 实际要开的是「k 次方根」，即指数固定为 k
	if (x <= 0) {
		return -1;
	}
	if (k == 1) {
		return x;
	}
	if (x == 1) {
		return 1; // 1^k 恒为 1，底数取 1
	}

	ll lo = 1, hi = x;
	while (lo <= hi) {
		ll mid = lo + (hi - lo) / 2;
		ll r = 1;
		bool too_big = false;//控制看是否会过大
		for (int i = 0; i < k; i++) {
			// 判断 r * mid 是否会大于 x（乘的过程中一旦超过 x 就停止）
			if (mid != 0 && r > x / mid) {
				too_big = true;//如果过大就停止
				break;
			}
			r *= mid;
		}//这个循环就是找到离他最近的那个数并判断是否满足条件
		if (too_big) {
			hi = mid - 1;//如果过大就减小mid继续尝试，同时过大的话直接进行下一次循环了
			continue;
		}
		if (r == x) {
			return mid;
		}
		// 未触发 too_big 时连乘 k 次后必有 r <= x，故除相等外只会 r < x，mid 偏小
		lo = mid + 1;
	}
	return -1;
}

/*
 * getpow(xx, fx)
 * 含义：在正整数范围内求「以 fx 为底的离散对数」——找非负整数 p，使得
 *       fx^p == xx。
 *       - xx == 1 时，约定 p == 0（任何 fx>=1 都有 fx^0 == 1）。
 *       - fx == 1 时，只有 xx == 1 有解（p 取 0），否则无解。
 *       - 若 xx 不能写成 fx 的整数次幂，返回 -1。
 * 实现：不断用 xx 除以 fx，能除尽则指数加一，最后必须除到 1。
 */
long long getpow(long long xx, long long fx) {
	// 目标：找非负整数 p，使 fx^p == xx；找不到返回 -1（离散对数 / 反复整除判幂）
	if (xx <= 0 || fx <= 0) {
		return -1; // 题目里是正整数，非法输入直接无解
	}
	if (fx == 1) {
		// 1 的任何次幂都是 1，只有 xx==1 时才有 p（约定 p=0，因 1^0=1）
		return xx == 1 ? 0 : -1; // xx 不是 1 却用底数 1，永远凑不出来
	}
	ll p = 0;   // 指数计数：统计「从 xx 里一共拆出了多少个 fx」
	ll t = xx;  // 工作变量：从 xx 开始不断除以 fx，看能否除到 1
	while (t > 1) {
		// 若 xx 真是 fx 的整数次幂，每一步都必须能整除，否则不是纯幂
		if (t % fx != 0) {
			return -1; // 中途除不尽，说明 xx 不能写成 fx^p
		}
		t /= fx; // 再拆出一个因子 fx，等价于指数里多乘一个 fx
		p++;     // 每成功除一次，指数 p 加 1
	}
	return p; // 除到 t==1，说明 xx = fx*fx*...（p 个），即 fx^p == xx
}

// 在类型定义之后再声明容器
vector<ratio> ratios; // 存储那个分数

int main2()
{//这里处理输入
	cin >> N;//输入数字个数
	for (int i = 0; i < N; i++)
	{
		cin >> arr[i];
	}
//给这些数排序
	sort(arr, arr + N);

	// 接下来将相邻两项的比（后项/前项）存成最简分数；N==2 也走同一套逻辑，便于统一「本原公比」的尝试
	for (int j = 0; j < N - 1; j++) {
		if (arr[j] != arr[j + 1]) {// 相邻不相等才存，避免除零或重复
			ratios.push_back(ratio(arr[j + 1], arr[j]));// 升序：相邻级公比 = 后项/前项
		}
	}
	// 所有数相同：没有相邻差，公比视为 1/1
	if (ratios.empty()) {
		cout << "1/1" << endl;
		return 0;
	}

	// 对第一个比值的分子分母同时开 (pow+1) 次方根，得到候选本原公比。
	// pow 从大到小：先尽量「开方到底」，第一次全匹配即为本原公比（避免从小到大先命中 25/4 再错过 5/2）。
	for (int pow = 39; pow >= 0; pow--) {// 2^40 > 10^12，开方次数到此足够
		ratio ra0 = ratios[0];//现在我先把第一个比值给拿出来
		ll x = ra0.a;//分子
		ll y = ra0.b;//分母
		//然后是循环对他们开方
		ll fx = extract(x, pow);
		ll fy = extract(y, pow);
		if (fx == -1 || fy == -1) continue;//如果开不出来就继续尝试下一个pow
		//到这里找出来了所有可能的开方数
		bool all_match = true;

		for (int i = 0; i < ratios.size(); i++)//遍历接下来的数
		{
			ll xx = ratios[i].a;//分子
			ll yy = ratios[i].b;//分母
			ll px = getpow(xx, fx);// 以 fx 为底，xx 是几次幂
			ll py = getpow(yy, fy);
			// 1 的任何非负整数次幂都是 1：底为 1 且该侧真为 1 时，getpow 的指数不唯一，与另一侧对齐
			if (fx == 1 && xx == 1) {
				px = py;
			} else if (fy == 1 && yy == 1) {
				py = px;
			}

			if (px == -1 || py == -1 || px != py)// 两侧都能表成幂且指数相同
			{
				all_match = false;//如果不满足条件就不是答案
			}

		}
		if (all_match)
		{
			//这个如果返回的是true的时候，就说明这个pow是正确的了
			//这里我还要给他越分一下
			ratio ans = ratio(fx, fy);//这里就是它的最后答案
			cout << ans.a << "/" << ans.b << endl;//输出答案（分数形式）
			break;// pow 从大到小，首次合法即最细本原公比，只输出一行
		}
	}
}
