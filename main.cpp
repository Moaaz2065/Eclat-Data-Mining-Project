#include <bits/stdc++.h>

#define el '\n'
#define d(x) cout<<"value of "<<#x<<" is : "<<x<<endl
using namespace std;
vector<string> c1, c2;
map<string, set<string>> ItemsetAndTID, sorted_key;
bool seq = false, ok = false;
int min_sup, OG_size;
double min_c;

vector<string> splitString(const string &input, char delimiter) {
    vector<std::string> result;
    istringstream stream(input);
    string token;

    while (std::getline(stream, token, delimiter)) {
        result.push_back(token);
    }

    return result;
}

void convert() {
    map<string, set<string>> mp;
    for (int i = 1; i < c1.size(); i++) {
        string tt;
        for (auto &j: c2[i]) {
            if (j == ',') {
                string tmp = tt;
                sort(tmp.begin(), tmp.end());
                mp[tt].insert(to_string(i));
                sorted_key[tmp].insert(to_string(i));
                tt.clear();
                continue;
            }
            tt += j;
        }
        string tmp = tt;
        sort(tmp.begin(), tmp.end());
        sorted_key[tmp].insert(to_string(i));
        mp[tt].insert(to_string(i));
    }
    ItemsetAndTID = mp;
}

void read() {
    ifstream file;
    file.open("Horizontal_Format.csv");
    while (!file.eof()) {
        string temp;
        getline(file, temp);
        c1.emplace_back(temp.substr(0, temp.find(',')));
        string ss = temp.substr(temp.find(',') + 1);
        c2.emplace_back("");
        for (auto &i: ss) if (i != '"') c2.back() += i;


    }
    for (auto &i: c1[0]) i = tolower(i);
    if (c1[0] == "sid") seq = true;
    else if (c1[0] == "tid") convert();
    else {
        for (int i = 1; i < c1.size(); i++) {
            string tt;
            for (int j = 0; j < c2[i].size() + 1; j++) {
                if (c2[i][j] == ',' || j == c2[i].size()) {
                    ItemsetAndTID[c1[i]].insert(tt);
                    tt.clear();
                    continue;
                }
                tt += c2[i][j];
            }
        }
    }
    file.close();
}

void gen_association(int n, vector<string> &l) {
    set<string> st;

    for (string s: l) {
        vector<string> tempo = splitString(s, ',');
        sort(tempo.begin(), tempo.end());
        do {

            for (int i = 1; i < tempo.size(); i++) {
                string tmp = tempo[0];
                for (int j = 1; j < tempo.size(); j++) {
                    if (j == i)
                        tmp += "->";
                    else
                        tmp += ',';
                    tmp += tempo[j];
                }

                st.insert(tmp);
            }
        } while (next_permutation(tempo.begin(), tempo.end()));
    }

    for (auto &s: st)
        cout << s << ' ';
    cout << endl;
}

map<set<string>, set<string>> tkrar;
set<string> rules;

void get_rule(int idx, vector<string> &s, string tt, vector<bool> &vis, bool arrow) {
    if (!count(vis.begin(), vis.end(), false) && arrow) {
        string s1 = tt.substr(0, tt.find("->"));
        string s2 = tt.substr(tt.find("->") + 2);
        set<string> a1, a2;
        string temp;
        for (int i = 0; i < s1.size() + 1; i++) {
            if (s1[i] == ',' || i == s1.size()) {
                a1.insert(temp);
                temp.clear();
                continue;
            }
            temp += s1[i];
        }
        for (int i = 0; i < s2.size() + 1; i++) {
            if (s2[i] == ',' || i == s2.size()) {
                a2.insert(temp);
                temp.clear();
                continue;
            }
            temp += s2[i];
        }

        if (tkrar[a1] != a2) {
            tkrar[a1] = a2;
            rules.insert(tt);
        }
        return;
    }
    if (!count(vis.begin(), vis.end(), false)) return;
    //
    for (int i = 0; i < s.size(); i++) {
        if (vis[i]) continue;
        vis[i] = true;
        if (!arrow) get_rule(idx, s, tt + "->" + s[i], vis, true);
        get_rule(idx, s, tt + ',' + s[i], vis, arrow);
        vis[i] = false;
    }
}


void gen_association2(int n, vector<string> &l) {
    rules.clear();
    for (string &s: l) {
        vector<string> temp = splitString(s, ',');

        for (int i = 0; i < temp.size(); i++) {
            string tt = temp[i];
            vector<bool> vis(temp.size(), false);
            vis[i] = true;
            get_rule(i, temp, tt, vis, false);
        }

    }
    cout << "ASSOCIATION RULES : \n";

    cout << el << el;
    //HERE...
    //E-> K
    for (auto &it: rules) {
        vector<string> v;
        string s1 = it.substr(0, it.find('-'));
        string s2 = it.substr(it.find('>') + 1);

        string tmp = s1 + "," + s2;
        sort(tmp.begin(), tmp.end());
        cout << "--------------------------\n";
        sort(s1.begin(), s1.end());
        sort(s2.begin(), s2.end());

        double c = (double)sorted_key[tmp].size() / (double) sorted_key[s1].size();
        if (c >= min_c)
            cout << "Strong Rule: " << it << " as Confidence = " << c << el;
        else
            cout << "Weak Rule : " << it << " as Confidence = " << c << el;

        double lift = ((double)sorted_key[tmp].size() / ((double) (sorted_key[s1].size() * sorted_key[s2].size()))) *OG_size;

        if(lift > 1.0) cout << "Dependent +ve Correlated Lift = " << lift << el;
        else if(lift < 1.0) cout << "Dependent -ve Correlated Lift = " << lift << el;
        else cout << "Independent Lift = " << lift << el;
    }
    cout << el;

}

void gen_c(int n) {
    vector<string> l;
    cout << "C" << n << " : " << el;
    cout << "--------------------------------\n";
    for (auto &it: ItemsetAndTID) {
        if(count(it.first.begin(), it.first.end(), ',') == n - 1) {
            cout << it.first << " : ";
            for (auto &j: it.second) {
                cout << j;
                if (j != *it.second.rbegin()) cout << ",";
            }
            cout << el;
            cout << "--------------------------------\n";
        }
        if (count(it.first.begin(), it.first.end(), ',') == n - 1 && it.second.size() >= min_sup)
            l.push_back(it.first);
    }
    cout << el;
    cout << "L" << n << el;
    cout << "--------------------------------\n";
    for (string &i: l) {
        cout << i << " : ";
        for(auto &j : ItemsetAndTID[i]) {
            cout << j;
            if(j != *ItemsetAndTID[i].rbegin()) cout << ",";
        }
        cout << el;
        cout << "--------------------------------\n";
    }
    cout << el << el;
    //generate c (n+1)
    for (int i = 0; i < l.size(); i++) {
        for (int j = i + 1; j < l.size(); j++) {
            set<string> s;
            string tt;
            for (char c: l[i]) {
                if (c == ',') {
                    s.insert(tt);
                    tt.clear();
                    continue;
                }
                tt += c;
            }
            s.insert(tt);
            tt.clear();
            for (char c: l[j]) {
                if (c == ',') {
                    s.insert(tt);
                    tt.clear();
                    continue;
                }
                tt += c;
            }
            s.insert(tt);
            if (s.size() == n + 1) {
                ok = 1;
                string tmp;
                for (string c: s)
                    tmp += c + ',';

                tmp.pop_back();
                //intersection
                string tmp2 = tmp;
                sort(tmp2.begin(), tmp2.end());
                for (auto &k: ItemsetAndTID[l[i]]) {
                    for (auto &ii: ItemsetAndTID[l[j]]) {
                        if (ii == k) {
                            ItemsetAndTID[tmp].insert(ii);
                            sorted_key[tmp2].insert(ii);
                        }
                    }
                }

            }
        }
    }
    if (n > 1)
        gen_association2(n, l);


}


int main() {
    cout << "Enter min support and min confidence : ";
    cin >> min_sup >> min_c;
    cout << el;
    read();
    if (seq) {
        cout << "ERROR: Eclat can't be used on sequenced itemsets!" << el;
        return 0;
    }
    cout << "Vertical Data Format :" << el;
    cout << "--------------------------------\n";
    set<string> sz;
    for (auto &i: ItemsetAndTID) {
        cout << i.first << " : ";
        string temp = i.first;
        sort(temp.begin(),temp.end());
        for (auto &j: i.second) {
            sorted_key[temp].insert(j);
            sz.insert(j);
            cout << j;
            if(j != *i.second.rbegin()) cout << ",";
        }
        cout << el;
        cout << "--------------------------------\n";
    }
    OG_size = sz.size();
    cout << el;
    int idx = 1;
    do {
        ok = false;
        gen_c(idx++);
    } while (ok);

    cout << "DONE!!!!\n";
    return 0;
}
