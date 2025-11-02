# Report2
## Problem
Apply the A\* algorithm to solve the 8 puzzle problem.

## Analysis
The implementation addresses an extended version of the 8-puzzle problem, allowing for:
- Any 2D grid: The board is represented by `vector<vector<long long>>`, which can theoretically support rows with varying column counts, although the main function initializes a rectangular grid.
- Duplicate pieces: The heuristic functions, are designed to handle duplicate piece values.
- Special moving pieces: Some pieces are identified as "moving pieces" and their positions are tracked in `PosMovs`. The A\* algorithm explores moves for each of these identified moving pieces.

A\* Algorithm Core Implementation:
- Node Structure: Each state in the search space is represented by a `nodeT` struct, which includes the current board configuration (`board_cur`), the sequence of moves taken to reach this state (`procs`), the current positions of the moving pieces (`posMovs`), and the calculated `f_val` (total estimated cost).
- Open Set: A `priority_queue<nodeT>` (Que) serves as the open set, automatically ordering nodes by their `f_val` in ascending order (due to the overloaded `operator<()`), ensuring that the node with the lowest estimated total cost is always processed next.
- Closed Set: A `map<vector<ll_vec>, long long>` (vis) acts as the closed set. It stores visited board configurations along with the minimum g-cost (number of steps) required to reach them, preventing redundant computations and ensuring that the shortest path to any state is considered.
- Cost Function: The `f_val` for a node is calculated as `g_cost + h_cost`, where `g_cost` is the number of moves made (`procs.size()`) and `h_cost` is the heuristic estimate.

Heuristic Functions:
- `calBoardHVal_1()`: This function calculates the number of non-moving pieces that are not in their correct target positions. It iterates through the board and increments the heuristic value for each non-moving piece whose value does not match the corresponding piece in the target board.
- `calBoardHVal_2()`: This function calculates the sum of Manhattan distances for all non-moving pieces. For each non-moving piece on the current board, it searches the entire target board to find its closest matching target position (handling duplicate values by taking the minimum distance) and sums these distances. Both heuristics explicitly exclude the "moving pieces" (those in `PosMovs`) from their calculations.

Movement Generation:
The algorithm generates possible moves by iterating through each identified moving piece and attempting to move it according to `Movs`,provided the move is within the board boundaries (`isIdxValid()`).

## Code
```cpp
#include <iostream>
#include <vector>
#include <utility>
#include <queue>
#include <map>
#include <algorithm>

#define N_MOV 4

using namespace std;

typedef vector<long long> ll_vec;
typedef pair<long long, long long> PLL;
typedef pair<short, long long> stepT;

struct nodeT
{
    vector<ll_vec> board_cur;
    vector<stepT> procs;
    vector<PLL> posMovs;
    long long f_val;
    bool operator<(const nodeT &other) const
    {
        return other.f_val < f_val;
    }
};

const PLL Movs[N_MOV] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

inline bool isIdxValid(const vector<ll_vec> &Board, const PLL &idx)
{
    return (idx.first >= 0) && (static_cast<size_t>(idx.first) < (Board.size())) && (idx.second >= 0) && (static_cast<size_t>(idx.second) < ((Board[idx.first].size())));
}

inline long long calBoardHVal_1(const vector<ll_vec> &Board_cur, const vector<ll_vec> &Board_tar, const vector<PLL> &PosMovs)
{
    long long h_val = -1;
    if (Board_cur.size() == (Board_tar.size()))
    {
        h_val = 0;
        for (auto it1 = (Board_cur.begin()), it2 = (Board_tar.begin()); it1 < (Board_cur.end()); ++it1, ++it2)
            if (it1->size() == (it2->size()))
            {
                for (auto it3 = (it1->begin()), it4 = (it2->begin()); it3 < (it1->end()); ++it3, ++it4)
                    if (((*it3) != (*it4)) && (find(PosMovs.begin(), PosMovs.end(), (PLL){it1 - (Board_cur.begin()), it3 - (it1->begin())}) == PosMovs.end()))
                        ++h_val;
            }
            else
                return -1;
    }
    return h_val;
}

inline long long calBoardHVal_2(const vector<ll_vec> &Board_cur, const vector<ll_vec> &Board_tar, const vector<PLL> &PosMovs)
{
    long long h_val = -1;
    if (Board_cur.size() == (Board_tar.size()))
    {
        h_val = 0;
        for (auto it1 = (Board_cur.begin()), it2 = (Board_tar.begin()); it1 < (Board_cur.end()); ++it1, ++it2)
            if (it1->size() == (it2->size()))
            {
                for (auto it3 = (it1->begin()); it3 < (it1->end()); ++it3)
                    if (find(PosMovs.begin(), PosMovs.end(), (PLL){it1 - (Board_cur.begin()), it3 - (it1->begin())}) == PosMovs.end())
                    {
                        long long temp = -1;
                        for (auto it4 = (Board_tar.begin()); it4 < (Board_tar.end()); ++it4)
                            for (auto it5 = (it4->begin()); it5 < (it4->end()); ++it5)
                                if ((*it3) == (*it5))
                                {
                                    long long dst = abs((it4 - (Board_tar.begin())) - (it1 - (Board_cur.begin()))) + abs((it5 - (it4->begin())) - (it3 - (it1->begin())));
                                    temp = ((temp < 0) || (temp > dst)) ? dst : temp;
                                }
                        if (temp < 0)
                            return -1;
                        else
                            h_val += temp;
                    }
            }
            else
                return -1;
    }
    return h_val;
}

inline vector<stepT> solve1_bfs(const vector<ll_vec> &Board_ini, const vector<ll_vec> &Board_tar, const vector<PLL> &PosMovs)
{
    if ((!(Board_ini.empty())) && (Board_ini.size() == (Board_tar.size())) && (!(PosMovs.empty())) && (Board_ini != Board_tar))
    {
        for (auto it1 = (Board_ini.begin()), it2 = (Board_tar.begin()); it1 < Board_ini.end(); ++it1, ++it2)
            if (it1->size() != (it2->size()))
                return {};
        priority_queue<nodeT> Que;
        // nodeT node = {Board_ini, {}, PosMovs, calBoardHVal_1(Board_ini, Board_tar, PosMovs)};
        nodeT node = {Board_ini, {}, PosMovs, calBoardHVal_2(Board_ini, Board_tar, PosMovs)};
        map<vector<ll_vec>, long long> vis = {{Board_ini, 0}};
        do
        {
            if (!(Que.empty()))
                node = Que.top(), Que.pop();
            short j = 0;
            for (long long i = 0; static_cast<size_t>(i) < (node.posMovs.size()); ++i, j = 0)
                for (; j < N_MOV; ++j)
                    if (isIdxValid(node.board_cur, node.posMovs[i]) && isIdxValid(node.board_cur, {node.posMovs[i].first + (Movs[j].first), node.posMovs[i].second + (Movs[j].second)}))
                    {
                        auto temp = node;
                        swap(temp.board_cur[temp.posMovs[i].first][temp.posMovs[i].second], (temp.board_cur[temp.posMovs[i].first + (Movs[j].first)][temp.posMovs[i].second + (Movs[j].second)])), temp.posMovs[i].first += (Movs[j].first), temp.posMovs[i].second += (Movs[j].second), temp.procs.emplace_back(j, i);
                        if ((vis.insert({temp.board_cur, temp.procs.size()}).second) || ((vis[temp.board_cur] > static_cast<long long>(temp.procs.size())) ? (vis[temp.board_cur] = static_cast<long long>(temp.procs.size())) : false))
                        {
                            if (temp.board_cur == Board_tar)
                                return temp.procs;
                            else
                                // temp.f_val = static_cast<long long>((temp.procs).size()) + calBoardHVal_1(temp.board_cur, Board_tar, temp.posMovs), Que.push(temp);
                                temp.f_val = static_cast<long long>((temp.procs).size()) + calBoardHVal_2(temp.board_cur, Board_tar, temp.posMovs), Que.push(temp);
                        }
                    }
        } while (!(Que.empty()));
    }
    return {};
}

int main()
{
    long long m = 0, n = 0;
    cin >> m >> n;
    vector<PLL> PosMovs;
    vector<ll_vec> Board_ini(m, ll_vec(n)), Board_tar(m, ll_vec(n));
    for (auto it1 = (Board_ini.begin()); it1 < (Board_ini.end()); ++it1)
        for (auto it2 = (it1->begin()); it2 < (it1->end()); ++it2)
            if ((cin >> (*it2)) && (!(*it2)))
                PosMovs.emplace_back(it1 - Board_ini.begin(), it2 - (it1->begin()));
    for (auto it1 = (Board_tar.begin()); it1 < (Board_tar.end()); ++it1)
        for (auto it2 = (it1->begin()); it2 < (it1->end()); ++it2)
            cin >> (*it2);
    auto Sol = solve1_bfs(Board_ini, Board_tar, PosMovs);
    for (auto it1 = (Sol.begin()); it1 < (Sol.end()); ++it1)
    {
        cout << "Step: " << it1 - (Sol.begin()) + 1 << endl;
        swap(Board_ini[PosMovs[it1->second].first][PosMovs[it1->second].second], Board_ini[PosMovs[it1->second].first + (Movs[it1->first].first)][PosMovs[it1->second].second + (Movs[it1->first].second)]), PosMovs[it1->second].first += (Movs[it1->first].first), PosMovs[it1->second].second += (Movs[it1->first].second);
        for (auto it2 = (Board_ini.begin()); it2 < (Board_ini.end()); ++it2)
        {
            for (auto it3 = (it2->begin()); it3 < (it2->end()); ++it3)
                cout << (*it3) << ' ';
            cout << endl;
        }
    }
    return 0;
}

```

### Complexity
- Time:
    - `calBoardHVal_1()`: $O(s! \times s)$
    - `calBoardHVal_2()`: $O(s! \times s^2)$
- Space: $O((s!)^2)$

Since `calBoardHVal_2()` provides more heuristic information than `calBoardHVal_1()` because it's always greater or equal to `calBoardHVal_1()`. So the A\* algorithm using it as heuristic function will eventually expands fewer or equal nodes and run faster, although its theorotical complexity is higher.

## Results
- `calBoardHVal_1()`
    ```text
    3 3
    2 8 3
    1 6 4
    7 0 5
    1 2 3
    8 0 4
    7 6 5
    Step: 1
    2 8 3 
    1 0 4 
    7 6 5 
    Step: 2
    2 0 3 
    1 8 4 
    7 6 5 
    Step: 3
    0 2 3 
    1 8 4 
    7 6 5 
    Step: 4
    1 2 3 
    0 8 4 
    7 6 5 
    Step: 5
    1 2 3 
    8 0 4 
    7 6 5
    ```
- `calBoardHVal_2()`
    ```text
    3 3
    2 8 3
    1 6 4
    7 0 5
    1 2 3
    8 0 4
    7 6 5
    Step: 1
    2 8 3 
    1 0 4 
    7 6 5 
    Step: 2
    2 0 3 
    1 8 4 
    7 6 5 
    Step: 3
    0 2 3 
    1 8 4 
    7 6 5 
    Step: 4
    1 2 3 
    0 8 4 
    7 6 5 
    Step: 5
    1 2 3 
    8 0 4 
    7 6 5
    ```
