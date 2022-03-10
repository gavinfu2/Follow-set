#include <bits/stdc++.h>
using namespace std;

struct Rule
{
    char nonTerminal;
    string production;

    public:
        Rule(char nonTerminal, string production)
        {
            this->nonTerminal = nonTerminal;
            this->production = production;
        }
};
struct FirstSet
{
    char nonTerminal;
    string set;

    public:
        FirstSet(char nonTerminal, string set)
        {
            this->nonTerminal = nonTerminal;
            this->set = set;
        }
};
struct FollowSet
{
    char lhs;
    string rhs;

    public:
        FollowSet(char lhs, string rhs)
        {
            this->lhs = lhs;
            this->rhs = rhs;
        }
};
unsigned int firstSearch(char, vector<FirstSet>);
unsigned int followSearch(char, vector<FollowSet>);
string exist(string s,string rhs){
    string tmp="";
    bool found;
    for (unsigned int i=0; i<s.size();i++){
        found = true;
        for (unsigned int j=0;j<rhs.size();j++){
            if(rhs[j]==s[i]){
                found = false;
                break;
            }
        }
        if(found)
            tmp += s[i];
    }
    return tmp;
}
bool upper(char c){
    if (c>=65 && c<=90)
        return true;
    else
        return false;
}

int main()
{
    const int maxLineLength = 500;
    vector<Rule> rules;
    vector<char> store_lhs;
    char input[maxLineLength];
    string order;
    cin.getline(input, maxLineLength);
    while (strcmp(input, "END_OF_GRAMMAR"))
    {
        char nonTerminal = input[0];
        string production;
        for (unsigned int i = 2; i < strlen(input); i++)
        {
            if (input[i] != '|')
            {
                production += input[i];
            }
            else
            {
                rules.push_back(Rule(nonTerminal, production));
                production = "";
            }
        }
        rules.push_back(Rule(nonTerminal, production));
        cin.getline(input, maxLineLength);
    }
    vector<FirstSet> firstSets;
    vector<FollowSet> followSets;
    cin.getline(input, maxLineLength);
    while (strcmp(input, "END_OF_FIRST_SET"))
    {
        char nonTerminal = input[0];
        string production;
        for (int i = 2; i < strlen(input); i++)
        {
            production += input[i];
        }
        firstSets.push_back(FirstSet(nonTerminal, production));
        order += nonTerminal;
        cin.getline(input, maxLineLength);
    }
    sort(order.begin(), order.end(), less<char>());

    followSets.push_back(FollowSet('S',"$"));
    bool change,allEmpty;
    string temp,check,word,ii;
    int step=0;
    int Bpos,beforeChange,afterChange;
    int first_pos,follow_pos,follow_pos2,erase_pos;
    do{
        change = false;
        for (unsigned int l=0;l<followSets.size();l++){
            check += followSets[l].rhs;
        }
        beforeChange = check.length();
        check.clear();
        for (auto r : rules)
        {
            for (auto p : r.production)
            {
                allEmpty = true;
                Bpos = r.production.find(p);
                if(!upper(p)){
                    continue;
                }
                for (unsigned int i = Bpos + 1; i < r.production.size(); i++)
                {
                    allEmpty = false;
                    if(!upper(r.production[i])){
                        follow_pos = followSearch(r.production[Bpos], followSets);
                        if(follow_pos!=-1){
                            word = r.production[i];
                            ii = exist(word, followSets[follow_pos].rhs);
                            followSets[follow_pos].rhs += ii;
                            ii.clear();
                        }
                        else{
                            word = r.production[i];
                            followSets.push_back(FollowSet(r.production[Bpos],word));
                        }                      
                        continue;
                    }
                    first_pos = firstSearch(r.production[i], firstSets);
                    follow_pos = followSearch(r.production[Bpos], followSets);
                    erase_pos = firstSets[first_pos].set.find(';');
                    if (erase_pos>=0)
                    {
                        allEmpty = true;
                        temp = firstSets[first_pos].set;
                        temp.erase(erase_pos,erase_pos+1);
                    }
                    if (follow_pos!=-1)
                    {
                        ii = exist(temp, followSets[follow_pos].rhs);
                        followSets[follow_pos].rhs += ii;
                        ii.clear();
                    }
                    else
                        followSets.push_back(FollowSet(r.production[Bpos],temp));   
                }
                if(allEmpty){
                    follow_pos = followSearch(r.production[Bpos], followSets);
                    if(follow_pos!=-1){
                        follow_pos2 = followSearch(r.nonTerminal, followSets);
                        ii = exist(followSets[follow_pos2].rhs, followSets[follow_pos].rhs);
                        followSets[follow_pos].rhs += ii;
                        ii.clear();
                    }
                    else{
                        follow_pos2 = followSearch(r.nonTerminal, followSets);
                        followSets.push_back(FollowSet(r.production[Bpos],followSets[follow_pos2].rhs));
                    }
                }
            }
        }
        for (unsigned int o=0;o<followSets.size();o++){
            temp.clear();
            for (unsigned int oo=0;oo<followSets[o].rhs.size();oo++)
            {
                if(temp.find(followSets[o].rhs[oo])==string::npos){
                    temp += followSets[o].rhs[oo];
                }
            }
            check += temp;
        }
        afterChange = check.length();
        check.clear();
        temp.clear();
        if(beforeChange!=afterChange){
            change = true;
        }
        step++;
    }while (change);
    string output;
    for(auto i : order){
        follow_pos = followSearch(i, followSets);
        sort(followSets[follow_pos].rhs.begin(), followSets[follow_pos].rhs.end(), less<char>());
        cout << followSets[follow_pos].lhs << " " << followSets[follow_pos].rhs << endl;
    }
}
unsigned int firstSearch(char c,vector<FirstSet> set){
    for (unsigned int i=0;i<set.size();i++){
        if(set[i].nonTerminal==c)
            return i;
    }
}
unsigned int followSearch(char c,vector<FollowSet> set){
    for (unsigned int i=0;i<set.size();i++){
        if(set[i].lhs==c)
            return i;
        else
            return -1;
    }
}

