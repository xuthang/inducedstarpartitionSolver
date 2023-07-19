#include<iostream>
#include<vector>
#include<cassert>
typedef long long int ll;
#define FOR(prom,a,b) for ( ll prom = (a); prom < (ll)(b); ++prom )
#define deb(x) std::cerr<<#x<<" "<<x<<std::endl;
template<typename T> std::ostream& operator<<(std::ostream& out, const std::vector<T> & v){for(const auto & x : v){out << x << ' ';} return out;}
template<typename T> std::ostream& operator<<(std::ostream& out, const std::set<T> & v){for(const auto & x : v){out << x << ' ';} return out;}
template<typename T> std::istream& operator>>(std::istream& in, std::vector<T>&v){for(auto&x:v){in>>x;}return in;}
