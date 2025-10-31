#include <iostream>
#include <vector>
#include <utility>
#include <queue>
#include <set>

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

inline long long calBoardHVal_1(const vector<ll_vec> &Board_cur, const vector<ll_vec> &Board_tar)
{
    long long h_val = -1;
    if (Board_cur.size() == (Board_tar.size()))
    {
        h_val = 0;
        for (auto it1 = (Board_cur.begin()), it2 = (Board_tar.begin()); it2 < (Board_tar.end()); ++it1, ++it2)
            if (it1->size() == (it2->size()))
            {
                for (auto it3 = it1->begin(), it4 = it2->begin(); it3 < (it1->end()); ++it3, ++it4)
                    if ((*it3) != (*it4))
                        ++h_val;
            }
            else
                return -1;
    }
    return h_val;
}

inline long long calBoardHVal_2(const vector<ll_vec> &Board_cur, const vector<ll_vec> &Board_tar)
{
    auto h_val = -1;
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
        nodeT node = {Board_ini, {}, PosMovs, calBoardHVal_1(Board_ini, Board_tar)};
        // nodeT node = {Board_ini, {}, PosMovs, calBoardHVal_2(Board_ini, Board_tar)};
        set<vector<ll_vec>> vis = {Board_ini};
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
                        if (vis.insert(temp.board_cur).second)
                        {
                            if (temp.board_cur == Board_tar)
                                return temp.procs;
                            else
                                temp.f_val = static_cast<long long>((temp.procs).size()) + calBoardHVal_1(temp.board_cur, Board_tar), Que.push(temp);
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
