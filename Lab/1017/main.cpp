#include <iostream>
#include <utility>
#include <vector>
#include <stack>

// #define K 2

using namespace std;

typedef pair<long long, long long> PLL;
typedef vector<PLL> procedure;

vector<procedure> solve(const long long n, const long long m, const long long k)
{
    vector<procedure> solutions;
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
                cout << "step " << (proc - (it->begin())) << ": " << endl;
                cout << "trans: " << (proc->first) << ", " << (proc->second) << "; " << "left: " << (left.first -= (proc->first)) << ", " << (left.second -= (proc->second)) << "; " << "right: " << (n - (left.first)) << ", " << (m - (left.second)) << endl;
            }
            cout << endl;
        }
    }
    return 0;
}
