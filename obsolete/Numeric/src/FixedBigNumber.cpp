#include <GradSoft/FixedBigNumber.h>
#include <iomanip>
#include <climits>
#include <cstdlib>

#ifdef HAVE_NAMESPACES
using namespace std;
namespace GradSoft {
#endif

FixedBigNumber& FixedBigNumber::operator-=(unsigned long y)
{
    if (sign_ == false) {
        sign_ = true;
        operator+=(y);
        sign_ = false;
        return *this;
    }
    if (this->less(y)) {
        sign_ = false;
        data_[0] = y - data_[0];
        return *this;
    }
    if (y <= data_[0]) {
        data_[0] -= y;
    } else {
        short i;
        for (i = 1; (i < dim_) && (data_[i] == 0); ++i) { }
        if (i < dim_) data_[i] -= 1;
        for(--i; i > 0; --i) data_[i] += ULONG_MAX;
        data_[0] += (ULONG_MAX - y);
    }
    return *this;
}


FixedBigNumber& FixedBigNumber::operator-=(const FixedBigNumber& y)
{
    if (sign_ != y.sign_) {
        sign_ = !sign_;
        operator+=(y);
        sign_ = !sign_;
        return *this;
    }
    if (this->less(y)) {
        FixedBigNumber tmp = -(y - *this);
        sign_ = tmp.sign_;
        short min = (dim_ < y.dim_) ? dim_ : y.dim_;
        short k;
        for (k = 0; k < min; ++k) data_[k] = tmp.data_[k];
        for (; k < dim_; ++k) data_[k] = 0;
        return *this;
    }
    short min = (dim_ < y.dim_) ? dim_ : y.dim_;
    for (short j = 0; j < min; ++j) {
        if (y.data_[j] <= data_[j]) {
            data_[j] -= y.data_[j];
        } else {
            short i;
            for (i = j + 1; (i < dim_) && (data_[i] == 0); ++i) { }
            if (i < dim_) data_[i] -= 1;
            for(--i; i > j; --i) data_[i] += ULONG_MAX;
            data_[j] += (ULONG_MAX - y.data_[j]) + 1;
        }
    }
    return *this;
}


FixedBigNumber operator-(FixedBigNumber x, unsigned long y)
{
    x -= y;
    return x;
}

FixedBigNumber operator-(unsigned long x, const FixedBigNumber& y)
{
    FixedBigNumber z(x, y.dimention());
    z -= y;
    return z;
}

FixedBigNumber operator-(FixedBigNumber x, const FixedBigNumber& y)
{
    x -= y;
    return x;
}


FixedBigNumber& FixedBigNumber::operator+=(unsigned long y)
{
    if (sign_ == false) {
        sign_ = true;
        operator-=(y);
        sign_ = !sign_;
        return *this;
    }
    data_[0] += y;
    short i = 1;
    unsigned long carry = 0UL;
    if (((ULONG_MAX - data_[0]) < y) && dim_ > 1) {
        carry = (ULONG_MAX == data_[1]) ? 1UL : 0UL;
        if (dim_ > 1) data_[1]++;
    }
    for (i = 2; (i < dim_) && (carry > 0); i++) {
        carry = (ULONG_MAX == data_[i]) ? 1UL : 0UL;
        data_[i] = 1;
    }
    return *this;
}

FixedBigNumber& FixedBigNumber::operator+=(const FixedBigNumber& x) {
    if (x.sign_ != sign_) {
        sign_ = !sign_;
        operator-=(x);
        sign_ = !sign_;
        return *this;
    }
    unsigned long carry = 0UL;
    unsigned long overflow;
    short i;
    short min = (dim_ < x.dim_) ? dim_ : x.dim_;
    for (i = 0; i < (min - 1); ++i) {
        overflow = ((ULONG_MAX - data_[i]) < x.data_[i]) ? 1UL : 0UL;
        data_[i] += x.data_[i] + carry;
        carry = overflow;
    }
    data_[i] += x.data_[i] + carry;
    return *this;
}

FixedBigNumber operator+(FixedBigNumber x, unsigned long y)
{
    x += y;
    return x;
}

FixedBigNumber operator+(unsigned long x, const FixedBigNumber& y)
{
    FixedBigNumber z(x, y.dimention());
    z += y;
    return z;
}

FixedBigNumber operator+(FixedBigNumber x, const FixedBigNumber& y)
{
    x += y;
    return x;
}

FixedBigNumber& FixedBigNumber::operator>>= (short sh)
{
    short j;
    short mv = sh/(sizeof(unsigned long)*8);
    if (mv > 0) {
        short dst;
        for (j = 0; j < dim_; ++j) {
            dst = j - mv;
            if (dst >= 0) data_[dst] = data_[j];
            data_[j] = 0;
        }
    }
    sh %= sizeof(unsigned long)*8;
    if (sh > 0) {
        data_[0] >>= sh;
        for (j = 1; j < dim_; ++j) {
            data_[j-1] |= data_[j] << (sizeof(unsigned long)*8 - sh);
            data_[j] >>= sh;
        }
    }
    return *this;
}

FixedBigNumber operator>> (FixedBigNumber x, short sh)
{
    return x >>= sh;
}

FixedBigNumber& FixedBigNumber::operator<<= (short sh) {
    short j;
    short mv = sh/(sizeof(unsigned long)*8);
    if (mv > 0) {
        short dst;
        for (j = dim_ - 1; j >= 0; --j) {
            dst = j + mv;
            if (dst < dim_) data_[dst] = data_[j];
            data_[j] = 0;
        }
    }
    sh %= sizeof(unsigned long)*8;
    if (sh > 0) {
        data_[dim_ - 1] <<= sh;
        for (j = dim_ - 2; j >= 0; --j) {
            data_[j+1] |= data_[j] >> (sizeof(unsigned long)*8 - sh);
            data_[j] <<= sh;
        }
    }
    return *this;
}

FixedBigNumber operator<< (FixedBigNumber x, short sh)
{
    return x <<= sh;
}

FixedBigNumber& FixedBigNumber::operator*=(const FixedBigNumber& y)
{
    FixedBigNumber x2(*this);
    if (!y[0]) {
        *this = 0UL;
    }
    short j = 0;
    for(short i = 1; i < y.size(); ++i) {
        if (y[i]) {
            x2 <<= (i - j);
            *this += x2;
            j = i;
        }
    }
    sign_ = (!(sign_ || y.sign_)) || (sign_ && y.sign_);
    return *this;
}

FixedBigNumber operator*(const FixedBigNumber& x, unsigned long y)
{
    FixedBigNumber z(x, x.dimention() + 1);
    FixedBigNumber yb(y, 1);
    z *= yb;
    return z;
}


FixedBigNumber operator*(unsigned long x, const FixedBigNumber& y)
{
    return operator*(y,x);
}

FixedBigNumber operator*(const FixedBigNumber& x, const FixedBigNumber& y)
{
    FixedBigNumber z(x, x.dimention() + y.dimention());
    z *= y;
    return z;
}

FixedBigNumber& FixedBigNumber::operator%=(const FixedBigNumber& yc)
{
    if (this->greater(0UL) && yc.greater(0UL)) {
        FixedBigNumber y(0UL, dim_);
        y = yc;
        short i = maxSignificantBit();
        short j = yc.maxSignificantBit();
        short shift = i - j;
        if (i >= j) {
            if (shift > 0) y <<= shift;
            for (; i > j; --i) {
                if (lessEqual(y, *this)) {
                    *this -= y;
                }
                if (equal(*this, 0UL)) return *this;
                y >>= 1;
            }
            if (lessEqual(y, *this)) {
                *this -= y;
            }
        }
        return *this;
    }
    *this = 0UL;
    return *this;
}

FixedBigNumber operator%(FixedBigNumber x, const FixedBigNumber& y)
{
    x %= y;
    return FixedBigNumber(x, y.dimention());
}

unsigned long operator%(FixedBigNumber x, unsigned long yl)
{
    FixedBigNumber y(yl, 1);
    x %= y;
    return x.getLong();
}

FixedBigNumber operator%(unsigned long xl, const FixedBigNumber& y)
{
    FixedBigNumber x(xl, y.dimention());
    x %= y;
    return x;
}

FixedBigNumber& FixedBigNumber::operator/=(const FixedBigNumber& yc)
{
    if (this->greater(0UL) && yc.greater(0UL)) {
        FixedBigNumber y(0UL, dim_);
        FixedBigNumber x(*this);
        *this = 0UL;
        y = yc;
        short i = x.maxSignificantBit();
        short j = yc.maxSignificantBit();
        short shift = i - j;
        if (i >= j) {
            if (shift > 0) y <<= shift;
            for (; i > j; --i) {
                if (lessEqual(y, x)) {
                    (*this)[i - j] = true;
                    x -= y;
                }
                if (equal(x, 0UL)) return *this;
                y >>= 1;
            }
            if (lessEqual(y, x)) {
                (*this)[i - j] = true;
                x -= y;
            }
        }
        return *this;
    }
    *this = 0UL;
    return *this;
}

FixedBigNumber operator/(FixedBigNumber x, const FixedBigNumber& y)
{
    return (x /= y);
}

FixedBigNumber operator/(FixedBigNumber x, unsigned long yl)
{
    FixedBigNumber y(yl, 1);
    return (x /= y);
}

FixedBigNumber operator/(unsigned long xl, FixedBigNumber y)
{
    FixedBigNumber x(xl, 1);
    return (x /= y);
}

bool FixedBigNumber::notEqual(unsigned long y) const
{
    if (data_[0] != y) return true;
    for (short i = 1; i < dim_; ++i) {
        if (data_[i] != 0) return true;
    }
    return false;
}

bool FixedBigNumber::notEqual(const FixedBigNumber& y) const
{
    const short min = (y.dim_ < dim_) ? y.dim_ : dim_;
    const FixedBigNumber& max = (y.dim_ < dim_) ? *this : y;
    short i;
    for (i = 0; i < min; ++i) {
        if (data_[i] != y.data_[i]) return true;
    }
    for (; i < max.dim_; ++i) {
        if (max.data_[i] != 0UL) return true;
    }
    return false;
}


bool FixedBigNumber::less(unsigned long y) const
{
    if (data_[0] >= y) return false;
    for (short i = 1; i < dim_; ++i) {
        if (data_[i] > 0) return false;
    }
    return true;
}

bool FixedBigNumber::less(const FixedBigNumber& y) const
{
    short i;
    if (dim_ > y.dim_) {
        for (i = dim_ - 1; i >= y.dim_; --i) {
            if (data_[i] > 0) return false;
        }
    } else {
        for (i = y.dim_ - 1; i >= dim_; --i) {
            if (y.data_[i] > 0) return true;
        }
    }
    for (; i >= 0; --i) {
        if (data_[i] > y.data_[i]) return false;
        else if (data_[i] < y.data_[i]) return true;
    }
    return false;
}



bool FixedBigNumber::greater(unsigned long y) const
{
    if (data_[0] > y) return true;
    for (short i = 1; i < dim_; ++i) {
        if (data_[i] > 0) return true;
    }
    return false;
}

bool FixedBigNumber::greater(const FixedBigNumber& y) const {
    short i;
    if (dim_ > y.dim_) {
        for (i = dim_ - 1; i >= y.dim_; --i) {
            if (data_[i] > 0) return true;
        }
    } else {
        for (i = y.dim_ - 1; i >= dim_; --i) {
            if (y.data_[i] > 0) return false;
        }
    }
    for (; i >= 0; --i) {
        if (data_[i] > y.data_[i]) return true;
        else if (data_[i] < y.data_[i]) return false;
    }
    return false;
}


bool FixedBigNumber::operator!=(unsigned long y) const
{
    return !this->positive() || this->notEqual(y);
}

bool FixedBigNumber::operator!=(const FixedBigNumber& y) const
{
    return (this->positive() != y.positive()) || this->notEqual(y);
}


bool FixedBigNumber::operator<(unsigned long y) const
{
    if (!this->positive()) return true;
    return this->less(y);
}

bool FixedBigNumber::operator<(const FixedBigNumber& y) const
{
    if (!this->positive() && y.positive()) return true;
    if (this->positive() && !y.positive()) return false;
    if (!this->positive() && !y.positive()) return !this->greater(y);
    return this->less(y);
}


bool FixedBigNumber::operator>(unsigned long y) const
{
    if (!this->positive()) return false;
    return this->greater(y);
}

bool FixedBigNumber::operator>(const FixedBigNumber& y) const
{
    if (!this->positive() && y.positive()) return false;
    if (this->positive() && !y.positive()) return true;
    if (!this->positive() && !y.positive()) return !this->less(y);
    return this->greater(y);
}



short FixedBigNumber::maxSignificantBit(unsigned long x)
{
    short res = 0;
    while ((x >>= 1) > 0) ++res;
    return res;
}

short FixedBigNumber::maxSignificantBit() const
{
    short i = dim_ - 1;
    for (; (i > 0) && (data_[i] == 0); --i);
    return i*8*sizeof(unsigned long) + maxSignificantBit(data_[i]);
}

ostream& operator<<(ostream& out, const FixedBigNumber& x)
{
    ios::fmtflags f = out.flags();
    char ch = out.fill();
    out << hex << setfill('0');
    if (!x.positive()) out << "- ";
    for (short i = x.dimention() - 1; i >= 0; --i) {
        out << setw(8) << x.data_[i] << " ";
    }
    out << setfill(ch);
    out.flags(f);
    return out;
}

void FixedBigNumber::leftShiftNoised()
{
    static short shift = maxSignificantBit(RAND_MAX) + 1;
    static unsigned long max4 = ULONG_MAX >> 4;
    do {
        *this <<= shift;
        data_[0] |= rand();
    } while (data_[dim_ - 1] < max4);
    (*this)[0] = true;
}

#ifdef HAVE_NAMESPACES
}
#endif
