#pragma once
#include <iostream>
#include <concepts>
#include <numeric>
#include <cmath>

template<typename I> requires std::integral<I>
class rational {
	I num_;
	I den_;
public:

	rational()
		: num_(I{0}),den_(I{1})
	{}
	
	rational(const I& n, const I& d)
		: num_(n),den_(d)
	{}
	
	I num() const{return num_;}
	I den() const{return den_;}

	rational& operator+=(const rational& other) {
        num_ = (num_ * other.den_) + (other.num_ * den_);
        den_ *= other.den_;
        return *this;
    }
	
	rational operator+(const rational& other) const{
		rational ret = *this;
		ret += other;
		return ret;
	}

	rational& operator-=(const rational& other) {
        num_ = (num_ * other.den_) - (other.num_ * den_);
        den_ *= other.den_;
        return *this;
    }
	
	rational operator-(const rational& other) const{
		rational ret = *this;
		ret -= other;
		return ret;
	}

	rational& operator*=(const rational& other) {
		num_ *= other.num_;
		den_ *= other.den_;
		return *this;
	}

	rational operator*(const rational& other) const {
		rational ret = *this;
		ret *= other;
		return ret;
	}
    
    rational& operator/=(const rational& other) {
		num_ *= other.den_;
		den_ *= other.num_;
		return *this;
	}

	rational operator/(const rational& other) const {
		rational ret = *this;
		ret /= other;
		return ret;
	}	
};

template<typename I> requires std::integral<I>
std::ostream&
operator<<(std::ostream& os, const rational<I>& r)
{
	I m = std::gcd(r.num(),r.den());
	if(r.num()==0 || r.den()==0){
		if(r.den()!=0) {
			os << r.num();
		}
		else if(r.num()<0) {
			os << "-Inf";
		}
		else if(r.num()>0) {
			os << "+Inf";
		}
		else {
			os << "NaN";
		}
	}
	else if(r.num()!=0 && r.den()>0) {
		if(r.den()/m == 1) {
			os << r.num()/m;
		}
		else{
			os << r.num()/m << "/" << r.den()/m;
		}
	}
	else if(r.num()!=0 && r.den()<0) {
		if(r.den()/m == -1) {
			os << -r.num()/m;
		}
		else{
			os << -r.num()/m << "/" << -r.den()/m;
		}
	}
	return os;
}