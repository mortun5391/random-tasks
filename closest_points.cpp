#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using ll = long long;

using namespace std;

vector<pair<int, int>> v;

double get_dist(int a, int b) {
    double dx = abs(v[a].first - v[b].first);
    double dy = abs(v[a].second - v[b].second);
    return sqrt(dx * dx + dy * dy);
}

tuple<double, pair<int, int>, vector<int>> f(int l, int r) {
    pair<int, int> points;
    double res_dist = 1e18;
    if (r - l + 1 <= 3) {
        for (int i = l; i <= r; i++) {
            for (int j = i + 1; j <= r; j++) {
                if (get_dist(i, j) < res_dist) {
                    res_dist = get_dist(i, j);
                    points = {i, j};
                }
            }
        }
        vector<int> b;
        for (int i = l; i <= r; i++) {
            b.push_back(i);
        }
        sort(b.begin(), b.end(), [](int a, int b) {
            return v[a].second < v[b].second;
        });
        return make_tuple(res_dist, points, b);
    }
    int mid = (l + r) / 2;
    auto [d1, p1, b1] = f(l, mid);
    auto [d2, p2, b2] = f(mid + 1, r);
    vector<int> b(b1.size() + b2.size());
    merge(b1.begin(), b1.end(), b2.begin(), b2.end(), b.begin(), [](int a, int b) {
        return v[a].second < v[b].second;
    });
    int midx = v[mid].first;
    double d = min(d1, d2);
    vector<int> c;
    for (auto x : b) {
        if (abs(midx - v[x].first) <= d) {
            c.push_back(x);
        }
    }
    if (d1 < d2) {
        res_dist = d1;
        points = p1;
    } else {
        res_dist = d2;
        points = p2;
    }
    for (int i = 0; i < c.size(); i++) {
        int j = i - 1;
        while (j >= 0 && v[c[j]].second >= v[c[i]].second - d) {
            if (get_dist(c[i], c[j]) < res_dist) {
                res_dist = get_dist(c[i], c[j]);
                points = {c[i], c[j]};
            }
            j--;
        }
    }
    return make_tuple(res_dist, points, b);
}

int main() {
    int n;
    cin >> n;
    v.resize(n);
    for (auto& x : v) {
        cin >> x.first >> x.second;
    }
    sort(v.begin(), v.end());
    auto [d, points, b] = f(0, n - 1);
    cout << fixed << setprecision(6) << d << endl;
    cout << v[points.first].first << ' ' << v[points.first].second << endl;
    cout << v[points.second].first << ' ' << v[points.second].second << endl;
    return 0;
}
