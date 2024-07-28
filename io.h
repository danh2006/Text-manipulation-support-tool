#define file(a) freopen(a".INP", "r", stdin), freopen(a".OUT", "w", stdout)
#define support(os, ...) os << "LINE(" << __LINE__ << ") : [" << #__VA_ARGS__ << "] =";

bool space = false;

#define printVec(out, x) \
   FOR(i, 0, (int)x.size() - 1){ \
      out << x[i] << (space ? " " : ""); \
   } \
   out << "\n"; \

template<class T>
ostream& operator << (ostream& out, const vector<T> x){
   printVec(out, x);
   return out;
}

ostream& operator << (ostream& out, const Info info){ // for debug
   support(out, "ignore");
   printVec(out, info.ignore);
   support(out, "repeatString");
   printVec(out, info.repeatString);
   support(out, "repeatTimes");
   printVec(out, info.repeatTimes);
   support(out, "step");
   printVec(out, info.step);
   support(out, "type");
   out << info.type << "}\n";
   support(out, "action");
   out << info.action << "}\n";
   support(out, "str");
   out << info.str << "}\n";
   return out;
}

template<class... T> void read(T&&... t) { 
   ((cin >> t), ...); 
}
template<class... T> void print(T&&... t) { 
   ((cout << t << " "), ...); 
}


