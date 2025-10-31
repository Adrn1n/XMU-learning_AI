# Report1
## Problem
Missionaries and cannibals problem.

## Analysis
Specify the problem, the constraint also applies on the boat. And when the boat arrives, all passengers must get off simultaneously.

Use `((leftMis_num, leftCan_num), (rightMis_num, rightCan_num), isBoatLeft)` to represent the state. It's meaningless to have 2 identical states in a single solution.

Depth-First Search (DFS) algorithm is used to find solutions for a variation of the Missionaries and Cannibals problem.
1. Search Strategy: A stack is used to perform a DFS, exploring possible moves from the current bank to the other
2. Move Generation: Nested loops iterate through all valid combinations of missionaries and cannibals that can be transported by the boat to the other side.
3. Constraints: Before making a move, checks if the number of cannibals is less than the missionaries on the departure and arrival banks and also the boat, preventing invalid states (cannibals outnumbering missionaries when missionaries are present).
4. Visited States: A set stores visited states to avoid redundant computations and infinite loops.
5. Goal: A solution is found when all individuals have successfully moved from the initial left bank to the right bank.
6. Solution Storage: All valid sequences of moves (procedures) that lead to the goal state are collected.

## Code
```cpp
#include <iostream>
#include <utility>
#include <vector>
#include <stack>
#include <set>

// #define K 2

using namespace std;

typedef pair<long long, long long> PLL;
typedef vector<PLL> procedure;

struct state
{
    PLL l_state, r_state;
    bool isL;
    bool operator<(const state &b) const
    {
        if (l_state < (b.l_state))
            return true;
        else if (l_state > (b.l_state))
            return false;
        else
        {
            if (r_state < (b.r_state))
                return true;
            else if (r_state > (b.r_state))
                return false;
            else
                return b.isL && (!isL);
        }
    }
};

struct stk_node
{
    state st;
    PLL move;
};

vector<procedure> solve(const long long n, const long long m, const long long k)
{
    vector<procedure> solutions;
    if ((m >= 0) && (n >= m) && (k > 0))
    {
        stack<stk_node> stk;
        stk_node node = {{{n, m}, {0, 0}, true}, {0, 1}};
        stk.push(node);
        procedure proc = {};
        set<state> vis = {{{n, m}, {0, 0}, true}};
        while (!(stk.empty()))
        {
            node = stk.top();
            bool flag = true, isL = node.st.isL;
            auto &st_1 = isL ? node.st.l_state : node.st.r_state, &st_2 = isL ? node.st.r_state : node.st.l_state;
            for (auto &i = (stk.top().move.first), &j = (stk.top().move.second); (i <= k) && (i <= (st_1.first)); j = 0, ++i)
            {
                for (; flag && (j <= (st_1.second)) && ((!i) || (j <= i)) && ((i + j) <= k); ++j)
                {
                    if ((st_1.first - i) && ((st_1.first - i) < (st_1.second - j)))
                        continue;
                    else if ((st_2.first + i) && ((st_2.first + i) < (st_2.second + j)))
                        break;
                    else
                    {
                        st_1.first -= i, st_1.second -= j, st_2.first += i, st_2.second += j, node.st.isL = !(node.st.isL);
                        if ((!(node.st.l_state.first)) && (!(node.st.l_state.second)))
                        {
                            if (isL)
                                proc.emplace_back(i, j);
                            else
                                proc.emplace_back(-i, -j);
                            solutions.push_back(proc), proc.pop_back();
                        }
                        else
                        {
                            if (vis.find(node.st) == vis.end())
                            {
                                if (isL)
                                    proc.emplace_back(i, j);
                                else
                                    proc.emplace_back(-i, -j);
                                vis.insert(node.st), flag = false;
                            }
                        }
                        if (flag)
                            st_1.first += i, st_1.second += j, st_2.first -= i, st_2.second -= j, node.st.isL = !(node.st.isL);
                    }
                }
                if (!flag)
                {
                    node.move.first = 0, node.move.second = 1, stk.push(node);
                    break;
                }
            }
            if (flag)
            {
                vis.erase(stk.top().st), stk.pop();
                if (!(proc.empty()))
                    proc.pop_back();
            }
        }
    }
    return solutions;
}

int main()
{
    long long n = 0, m = 0, k = 0;
    cin >> n >> m >> k;
    auto res = solve(n, m, k);
    // long long n = 0, m = 0;
    // cin >> n >> m;
    // auto res = solve(n, m, K);
    if (res.empty())
        cout << "No solutions!" << endl;
    else
    {
        cout << "Solution count: " << res.size() << endl
             << endl;
        for (auto it = res.begin(); it < res.end(); ++it)
        {
            cout << "Solution " << (it - res.begin()) << ": " << endl;
            PLL left = {n, m};
            for (auto proc = (it->begin()); proc < (it->end()); ++proc)
            {
                cout << "step " << (proc - (it->begin())) << ": \t" << "trans: " << (proc->first) << ", " << (proc->second) << ";\t";
                cout << "l: " << (left.first -= (proc->first)) << ", " << (left.second -= (proc->second)) << "; ";
                cout << "r: " << (n - (left.first)) << ", " << (m - (left.second)) << endl;
            }
            cout << endl;
        }
    }
    return 0;
}

```

### Complexity
Time: $O(k^{4 m n})$
Space: $O(m n)$

## Results
Take `n=3, m=3, k=2` as an example:
```txt
3 3 2
Solution count: 4

Solution 0: 
step 0:         trans: 0, 2;    l: 3, 1; r: 0, 2
step 1:         trans: 0, -1;   l: 3, 2; r: 0, 1
step 2:         trans: 0, 2;    l: 3, 0; r: 0, 3
step 3:         trans: 0, -1;   l: 3, 1; r: 0, 2
step 4:         trans: 2, 0;    l: 1, 1; r: 2, 2
step 5:         trans: -1, -1;  l: 2, 2; r: 1, 1
step 6:         trans: 2, 0;    l: 0, 2; r: 3, 1
step 7:         trans: 0, -1;   l: 0, 3; r: 3, 0
step 8:         trans: 0, 2;    l: 0, 1; r: 3, 2
step 9:         trans: 0, -1;   l: 0, 2; r: 3, 1
step 10:        trans: 0, 2;    l: 0, 0; r: 3, 3

Solution 1: 
step 0:         trans: 0, 2;    l: 3, 1; r: 0, 2
step 1:         trans: 0, -1;   l: 3, 2; r: 0, 1
step 2:         trans: 0, 2;    l: 3, 0; r: 0, 3
step 3:         trans: 0, -1;   l: 3, 1; r: 0, 2
step 4:         trans: 2, 0;    l: 1, 1; r: 2, 2
step 5:         trans: -1, -1;  l: 2, 2; r: 1, 1
step 6:         trans: 2, 0;    l: 0, 2; r: 3, 1
step 7:         trans: 0, -1;   l: 0, 3; r: 3, 0
step 8:         trans: 0, 2;    l: 0, 1; r: 3, 2
step 9:         trans: -1, 0;   l: 1, 1; r: 2, 2
step 10:        trans: 1, 1;    l: 0, 0; r: 3, 3

Solution 2: 
step 0:         trans: 1, 1;    l: 2, 2; r: 1, 1
step 1:         trans: -1, 0;   l: 3, 2; r: 0, 1
step 2:         trans: 0, 2;    l: 3, 0; r: 0, 3
step 3:         trans: 0, -1;   l: 3, 1; r: 0, 2
step 4:         trans: 2, 0;    l: 1, 1; r: 2, 2
step 5:         trans: -1, -1;  l: 2, 2; r: 1, 1
step 6:         trans: 2, 0;    l: 0, 2; r: 3, 1
step 7:         trans: 0, -1;   l: 0, 3; r: 3, 0
step 8:         trans: 0, 2;    l: 0, 1; r: 3, 2
step 9:         trans: 0, -1;   l: 0, 2; r: 3, 1
step 10:        trans: 0, 2;    l: 0, 0; r: 3, 3

Solution 3: 
step 0:         trans: 1, 1;    l: 2, 2; r: 1, 1
step 1:         trans: -1, 0;   l: 3, 2; r: 0, 1
step 2:         trans: 0, 2;    l: 3, 0; r: 0, 3
step 3:         trans: 0, -1;   l: 3, 1; r: 0, 2
step 4:         trans: 2, 0;    l: 1, 1; r: 2, 2
step 5:         trans: -1, -1;  l: 2, 2; r: 1, 1
step 6:         trans: 2, 0;    l: 0, 2; r: 3, 1
step 7:         trans: 0, -1;   l: 0, 3; r: 3, 0
step 8:         trans: 0, 2;    l: 0, 1; r: 3, 2
step 9:         trans: -1, 0;   l: 1, 1; r: 2, 2
step 10:        trans: 1, 1;    l: 0, 0; r: 3, 3

```
