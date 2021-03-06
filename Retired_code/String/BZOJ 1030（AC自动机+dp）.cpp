#include <bits/stdc++.h>
using namespace std;
const int maxn = 500100;
const int mod = 10007;

// AC自动机有三个部分，分别是建树，获取失配指针和查询。
// Aho_Corasick_Automaton ：可以简单的理解为将KMP放在Trie树上

// https://www.lydsy.com/JudgeOnline/problem.php?id=1030

// 给定 n个模板串，求出至少包含1个模板串的长度为 m 的串的个数。
// 只可能包含英文大写字母A ~ Z

int End[maxn];
int ch[maxn][26];
int fail[maxn];
int sz;
void insert(string s)
{
  int now = 0; //字典树的当前指针
  for(int i = 0; i < (int)s.size();i++) {
    int c = s[i] - 'A';
    // //Trie树没有这个子节点 : 就构造出来
    if(!ch[now][c]) ch[now][c] = ++sz;
    now = ch[now][c]; //然后向下构造
  }
  End[now] = 1;; //标记单词结尾
}
void getfail() //构造fail指针
{
  queue<int>que;
  for(int i = 0; i < 26; i ++) {
    if(ch[0][i]) {
      que.push(ch[0][i]);
      fail[ch[0][i]] = 0;  //指向根节点
    }
  }
  while(!que.empty()) { // BFS求fail指针
    int u = que.front();
    que.pop();
    for(int i = 0; i < 26; i++) { //枚举所有子节点
      int v = ch[u][i];
      if(v) { //存在这个子节点
        //子节点的 fail指针指向当前节点的 fail指针所指向的节点的相同子节点
        fail[v] = ch[fail[u]][i];
        
        // 注意如果一个单词的后缀是一个可读单词(即fail指针指向可读单词的结尾节点)
        // 那么这个单词一定也是可读的
        End[v] |= End[ch[fail[u]][i]];
        que.push(v);
      }
      // 不存在这个子节点
      //  当前节点的这个子节点指向当前节点fail指针的这个子节点
      else ch[u][i] = ch[fail[u]][i];
    }
  }
}
int n,m;
int dp[123][10010];
// dp[i][j]表示串长为 i , 匹配到了 AC 自动机的节点 j，并且从来没有出现过完整单词的方案数
void solve()
{
  int ans = 0;
  dp[0][0] = 1;
  for(int i = 1; i <= m; i++) {
    for(int j = 0; j <= sz; j++) {
      for(int k = 0; k < 26; k++) {
        int v = ch[j][k];
        if(End[v] == 0) {
          dp[i][v] += dp[i - 1][j];
          dp[i][v] %= mod;
        }
      }
    }
  }
  for(int i = 0; i <= sz; i++) {
    ans += dp[m][i];
    ans %= mod;
  }
  int sum = 1;
  for(int i = 1; i <= m; i++) {
    sum = sum * 26 % mod;
    sum %= mod;
  }
  std::cout << (sum - ans + mod) % mod << '\n';
}
string s;
int main(int argc, char const *argv[]) {

  std::cin >> n >> m;
  for(int i = 1; i <= n; i++) {
    std::cin >> s;
    insert(s);
  }
  getfail();
  solve();
  return 0;
}
