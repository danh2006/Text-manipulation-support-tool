#include <bits/stdc++.h>
using namespace std;
void debug() { cout << "]" << endl;} template<class H, class... T> void debug(H h, T... t) { cout << h; if(sizeof...(t)) cout << ", "; debug(t...); }
#define dbg(...) cout << "LINE(" << __LINE__ << ") : [" <<  #__VA_ARGS__ << "] = [", debug(__VA_ARGS__)

namespace Setting{
    vector<char> specialChar = {' ', '\n'};
    vector<string> action = {"\\remove", "\\replace", "\\insert", "\\find"};
    vector<string> attribute = {"\\ignore","\\step","\\repeat", "lower", 
                          "upper", "reverse", "type", "rate"};

    auto AddElement(string& str, const vector<char> condition, 
                    const char* breakChar){
        vector<string> save;
        string assign; 
        for(int i = 0; i < (int)str.size(); ++i){
            if(find(condition.begin(), condition.end(), str[i]) != condition.end()) continue;
            if(i + strlen(breakChar) < str.size()){
                string val = str.substr(i, strlen(breakChar));
                if(val == breakChar && assign != "") {
                    save.push_back(assign);
                    assign = "";
                    i += (int)val.size() - 1;
                    continue;
                }
            }
            assign += str[i];
        }
        if(assign != "") {
            save.push_back(assign);
        }
        return save;
    };

    auto Split(string& data, string type) {
        assert(type == "char" || type == "word" || type == "sentence");

        vector<string> save; 
        if(type == "char") {
            save.resize((int)data.size());
            for(int i = 0; i < (int)data.size(); ++i) {
                save[i] = data[i];
            }
        }else {
            auto token = (type == "word" ? ' ' : '.');
            save.resize(count(data.begin(), data.end(), token));
            string assign; 
            int count = 0;
            for(auto& value : data){
                if(value == token){
                    if(assign == "") continue;
                    save[count++] = assign;
                    assign = "";
                }else{
                    assign += value;
                } 
            }
            if(assign != ""){
                save.push_back(assign);
            }
        }
        return save;
    }
    
    class Info{

    public:

        vector<string> ignore; 
        vector<string> repeatString;
        vector<int> repeatTimes;
        vector<int> step;
        string type; 
        string action; 
        string str; 
        double rate;
        bool reverse;
        bool lower;
        bool upper;
        
        Info(){
            step.push_back(1);
            rate = 1;
            reverse = false;
            lower = false;
            upper = false;
            type = "char";
        }
        auto operator=(const Info& other){
            ignore = other.ignore;
            repeatString = other.repeatString;
            repeatTimes = other.repeatTimes;
            step = other.step;
            reverse = false;
            type = "char";
            lower = false;
            upper = false;
        }

    private:

        auto Bracket(string& data, int& end, int begin = -1) {
            vector<char> openBracket = {'{', '(', '['};
            vector<char> closeBracket = {'}', ')', ']'};

            for(int i = 0; i < (int)data.size(); ++i){
                auto id = find(openBracket.begin(), openBracket.end(), data[i]);
                if(id == openBracket.end()) continue;
                begin = i;
                break;
            }

            for(int i = 0; i < (int)data.size(); ++i){
                auto id = find(closeBracket.begin(), closeBracket.end(), data[i]);
                if(id == closeBracket.end()) continue;
                end = i;
                break;
            }

            {
                if(data[begin] == '{' || data[end] == '}'){
                    assert(data[begin] == '{' && data[end] == '}');
                }
            }

            auto subRange = string(data.begin() + begin + 1, data.begin() + end);
            if(data[begin] == '{'){ // Ignore whitespace
                auto element = AddElement(subRange, vector<char>{}, ",")   ;
                assert((int)element.size() > 0);
                return element;
            }else{ // [], [), (] ()
                auto element = AddElement(subRange, vector<char>{}, "...");
                assert((int)element.size() == 2);
                if(!isdigit(element[0][0])){
                    // cout << "element = ";
                    // cout << element[0] << " " << element[1] << "\n";
                    assert(element[0].size() == 1 && element[1].size() == 1);
                    element[0][0] += (data[begin] == '(');
                    element[1][0] -= (data[end] == ')');
                    if(element[0] > element[1]){
                        swap(element[0], element[1]);
                    }
                }else{
                    assert((int)element[0].size() > 0 && (int)element[1].size() > 0);
                    int64_t minRange = stoll(element[0]) + (data[begin] == '(');
                    int64_t maxRange = stoll(element[1]) - (data[end] == ')');
                    if(minRange > maxRange){
                        swap(minRange, maxRange);
                    }
                    element[0] = to_string(minRange);
                    element[1] = to_string(maxRange);
                }
                return element;
            }
        }		

        auto Step(string& data, int ptr = -1) {
            auto Range = Bracket(data, ptr); 
            vector<int> _step(Range.size());
            for(int i = 0; i < (int)_step.size(); ++i)
                _step[i] = stoi(Range[i]);
            return _step;
        }

        auto Ignore(string& data, int ptr = -1){
            auto Range = Bracket(data, ptr);
            return Range;
        }

        auto Repeat(string& data){
            int times = 1, ptr = -1;
            auto Range = Bracket(data, ptr);
            if(ptr + 1 != (int)data.size()){
                auto lenNum = (int)data.size() - ptr - 2;
                times = stoi(data.substr(ptr + 1, lenNum));
            }
            return pair<decltype(Range), vector<int>>
                    {Range, vector<int>(Range.size(), times)};
        }

    public:
        void filter(vector<string>& _str, vector<vector<string>>& _ignore, 
                    vector<vector<string>>& _repeatString){ 
                        
            _str = Split(str, type);

            for(int i = 0; i < (int)ignore.size(); ++i){
                _ignore[i] = Split(ignore[i], type);
            }   

            for(int i = 0; i < (int)repeatString.size(); ++i){
                _repeatString[i] = Split(repeatString[i], type);
            }   
        }

       template<class H, class Func>
        void select(string& name, H Range, bool flag, Func&& func){ 
            // xem xem la phai chon ham nao 
            if(name == "\\step"){
                func(step, Step(Range));
            }else if(name == "\\ignore"){
                func(ignore, Ignore(Range));
            }else if(name == "\\repeat"){
                auto [_repeatString, _repeatTimes] = Repeat(Range); 
                func(repeatString, _repeatString);
                func(repeatTimes, _repeatTimes);
            }else if(name == "rate"){
                auto id = find(Range.begin(), Range.end(), '%') - Range.begin();
                rate = stod(Range.substr(0, id - 1));
                if(id != Range.size())
                    rate /= 100;
                assert(rate >= 0 && rate <= 1);
            }else if(name == "reverse"){
                reverse = flag;
            }else if(name == "lower"){
                lower = flag;
            }else if(name == "upper"){
                upper = flag;
            }else if(name == "char" || name == "word" || name == "sentence"){
                type = name;
            }
        }
        
        void reset(){
            ignore.clear();
            repeatString.clear();
            repeatTimes.clear();
            step.clear();
            step.push_back(1);
            rate = 100;
            reverse = false;
            type = "char";
            lower = false;
            upper = false;
        }

        void update(string& data){
            auto len = find(data.begin(), data.end(), '{') - data.begin();
            auto name = data.substr(0, len);
            auto Range = string(data.begin() + (int)name.size() - 1, data.end());
            if(name == "\\step"){ 
                auto [_repeatString, _repeatTimes] = Repeat(Range);
                for(int i = 0; i < (int)_repeatString.size(); ++i){
                    auto id = find(repeatString.begin(), repeatString.end(), _repeatString[i]);
                    if(id != repeatString.end()){
                        int pos = id - repeatString.begin();
                        repeatTimes[pos] = _repeatTimes[i];  
                    }else{
                        repeatString.push_back(_repeatString[i]);
                        repeatTimes.push_back(_repeatTimes[i]);
                    }
                }
                return;
            }
            select(name, Range, true, [&](auto& source, auto _range){
                for(auto& val : _range)
                    source.push_back(val);
            });  
        }

        void erase(string& data){ // erase command
            auto id = find(data.begin(), data.end(), '{');
            if(id == data.end()){
                select(data, string(""), false, [&](auto& source, auto _range){
                    source.clear();
                });
                return;
            }
            auto len = id - data.begin();
            auto name = data.substr(0, len);
            auto Range = string(data.begin() + name.size() - 1, data.end() - 1);
            select(name, Range, false, [&]<typename T>(T& source, auto _range){
                T now;
                for(auto& val : source){
                    if(find(_range.begin(), _range.end(), val) != _range.end()){
                        now.push_back(val);
                    }
                }
                source = now;
            });                  
        }

        auto search(vector<string>& data, vector<string>& _str, 
                    vector<vector<string>>& _ignore, 
                    vector<vector<string>>& _repeatString){

            int idStep = 0, len = (int)str.size();
            vector<pair<double, int>> getPos;
            vector<bool> markPos(data.size(), true);
            vector<int> save;

            for(int i = 0; i < (int)data.size(); i += step[idStep]){
                auto vitural_id_step = idStep;
                int count = 0;
                for(int j = i, cnt_len = 0; j < (int)data.size() && cnt_len < len; j += step[vitural_id_step++]){
                    // auto is_repeat = find_if(repeat.begin(), repeat.end(), [&](auto& val){
                    //     return val !=                                                                                                                       
                    // });
                    if(data[j] == _str[cnt_len]){
                        count++;
                    }

                    if(vitural_id_step == (int)step.size()) vitural_id_step = 0;
                    cnt_len++;
                }

                double probability = ((double)count) / len;
                if(probability >= rate) {
                    getPos.emplace_back(probability, i);
                }
                idStep++;
                if(idStep == (int)step.size()){
                    idStep = 0;
                }
            }
            sort(getPos.rbegin(), getPos.rend());
            // check 
            for(auto&[_, pos] : getPos){
                if(markPos[pos] == false) continue;
                fill(markPos.begin() + max(0, pos - len + 1), markPos.begin() + pos, false);
                save.push_back(pos);
            }
            sort(save.begin(), save.end());
            return save;
        }
        void replace(str2){
            auto Point = find(data, _str, _ignore, _repeatString);
            for(int i = 0; i < Point.size(); ++i){
                data[Point[i].first] = str2;
                fill(data.begin() + Point[i].first + 1, data.begin() + Point[i].second, + 1 "");
            }
        }
        void remove(vector<string>& data, vector<string>& _str, 
                    vector<vector<string>>& _ignore, 
                    vector<vector<string>>& _repeatString){ 
            auto Point = find(data, _str, _ignore, _repeatString);
            for(int i = 0; i < Point.size(); ++i){
                fill(data.begin() + Point[i].first, data.begin() + Point[i].second + 1, "");
            }
        }
        void insert(str2){
            auto Point = find(data, _str, _ignore, _repeatString);
            for(int i = 0; i < Point.size(); ++i){
                fill(data.begin() + Point[i].first, data.begin() + Point[i].second,  "");
                data[Point[i].second] += str2;
            }
        }
    };

    // struct Mark{
    //     string from;
    //     string to; 

    //     void replace(string& data){

    //     }

    //     void parse(string& data){

    //     }

    // };
    vector<Info> info;
    // vector<Mark> mark;
} 
using namespace Setting;

int countTimes;

signed main() {
    ios::sync_with_stdio(false); cin.tie(0); //srand(time(NULL));

       auto Setting = [&](){
        vector<string> log;
        string setting;
        info.resize(1);
        while(getline(cin, setting)){
            if(setting == "\\end{setting}") break;
            if(setting == "\\reset"){
                info.back().reset();
            }else{
                auto pos1 = find(setting.begin(), setting.end(), '{') - setting.begin();
                auto name = setting.substr(0, pos1);
                if(find(attribute.begin(), attribute.end(), name) != attribute.end()){
                    info.back().update(setting);
                }else if(find(action.begin(), action.end(), name) != action.end()){
                    info.resize(info.size() + 1, info.back());
                    for(auto& val : log){
                        info.back().erase(val);
                    }
                    log.clear();
                    info.back().str = setting.substr(pos1 + 1, setting.size() - pos1 - 2);
                }else if(name == "\\erase"){
                    info.back().erase(setting);
                }else{
                    auto id = find(setting.begin(), setting.end(), '<');
                    if(id != setting.end()){
                        auto pos2 = id - setting.begin();
                        auto str = setting.substr(pos2 + 1, pos1 - pos2 - 2);
                        auto Range = AddElement(str, specialChar, ",");
                        for(auto& val : Range){
                            info.back().update(val); // val is string
                            log.push_back(val);
                        }
                        cout << log;
                        info.back().str = setting.substr(pos1 + 1, setting.size() - pos1 - 2);
                        // auto subRange = setting.substr(0, id) + setting.substr 
                    }
                }
            }
        }   
        // add minipage package(du kien)
    };

    auto Process = [&](string& data){
        if(info[countTimes].reverse == true){
            reverse(data.begin(), data.end());
        }
        if(info[countTimes].lower == true){
            transform(data.begin(), data.end(), data.begin(), ::tolower);
        }
        if(info[countTimes].upper == true){
            transform(data.begin(), data.end(), data.begin(), ::toupper);
        }
        sort(info[countTimes].ignore.begin(), info[countTimes].ignore.end(), [&](auto& u, auto& v){
            return u.size() > v.size();
        });
        vector<vector<string>> repeatString, ignore;
        vector<string> str;
        #define ADD str, ignore, repeatString

        info[countTimes].filter(ADD);
        auto Range = Split(data, string("char"));
        auto f = info[countTimes].search(Range, ADD);
        for(auto&x : f){
            cout << x << " ";
        }
/*
        if(info[countTimes].action == "replace"){
            info[countTimes].replace(Range, ADD);
        }else if(info[countTimes].action == "insert"){
            info[countTimes].insert(Range, ADD);
        }else{
            info[countTimes].remove(Range, ADD);
        }
*/
        vector<string> change(Range.size());
        data = "";
        for(int i = 0; i < (int)change.size(); ++i){
            data += change[i];
        }
        countTimes++;
    };

    // auto Get = [&](){
    //     string input; cin >> input; 
    //     string data;
    //     while(getline(cin, input)){
    //         data += input;
    //     }
    //     while(countTimes < (int)info.size()) Process(data);
    //     // cout << data << "\n";
    // };

    Setting();
    Get();
    return 0;
}


