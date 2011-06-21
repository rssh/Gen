#include <GradSoft/CryptHelper.h>

#ifdef HAVE_NAMESPACES
namespace GradSoft {
#endif


bool CryptHelper::witness(const FixedBigNumber& a, const FixedBigNumber& n,
        const FixedBigNumber& b) {
    FixedBigNumber d(1UL, n.dimention());
    FixedBigNumber x(0UL, n.dimention());
    for (short i = n.size() - 1; i >= 0; --i) {
         x = d;
         d = (d*d)%n;
         if ((d == 1) && (x != 1) && (x != b)) return true;
         if (b[i]) {
             d = (d*a)%n;
         }
    }
    if (d != 1) return true;
    return false;
}

FixedBigNumber CryptHelper::modExp(const FixedBigNumber& m,
        const FixedBigNumber& e, const FixedBigNumber& n) {
    FixedBigNumber r(0UL, n.dimention());
    r = 1UL;
    for (short i = (e.size() - 1); i >= 0; --i) {
        r = (r*r)%n;
        if (e[i]) r = (r*m)%n;
    }
    return r;
}

FixedBigNumber CryptHelper::getRandom(const FixedBigNumber& fr,
        const FixedBigNumber& to)
{
    FixedBigNumber comp = to - fr;
    FixedBigNumber res(0UL, to.dimention());
    FixedBigNumber tmp(0UL, 1);
    for(short i = 1; i < to.size(); ++i) {
        tmp = rand();
        for (short j = i; i < to.size()
                && i < j + maxRandomBit(); ++i) {
            res[i] = bool(tmp[i - j]);
            if (res > comp) {
                res[i] = false;
                break;
            }
        }
    }
    return res + fr;
}

bool CryptHelper::isPrime(const FixedBigNumber& n, short s)
{
    FixedBigNumber a(0UL, 1);
    FixedBigNumber nm1 = n - 1UL;
    for(; s > 0; --s) {
        a >>= 1;
        a.leftShiftNoised();
        if (witness(a, n, nm1)) return false;
    }
    return true;
}

bool CryptHelper::getInverse(FixedBigNumber x1, FixedBigNumber x0,
        FixedBigNumber& b)
{
    FixedBigNumber x2(0UL, x0.dimention());
    FixedBigNumber a(0UL, x0.dimention());
    FixedBigNumber buff(0UL, x0.dimention());
    a = 0UL;
    b = 1UL;
    while ((x2 = x0%x1) > 0) {
        buff = a;
        a = b;
        b = buff - (x0/x1)*b;
        x0 = x1;
        x1 = x2;
    }
    return (x1 == 1UL) ? true : false;
}

FixedBigNumber CryptHelper::gcd(FixedBigNumber a, FixedBigNumber b)
{
    FixedBigNumber& buff = a;
    while (b != 0UL) {
        buff = a%b;
        a = b;
        b = buff;
    }
    return a;
}


CryptHelper::RsaKeyPair CryptHelper::generateRSA(short dimention,
        short primeVerification)
{
    FixedBigNumber p(0UL, dimention);
    FixedBigNumber q(0UL, dimention);
    short j = 0;
    do {
        q >>= 1;
        q.leftShiftNoised();
        q[0] = true;
        if (isPrime(q, primeVerification)) {
            ++j;
            if (j == 1) p = q;
        }
    } while (j < 2);
    FixedBigNumber n = p*q;
    FixedBigNumber fn(0UL, dimention*2);
    fn = (p - 1) * (q - 1);
    FixedBigNumber d(0UL, dimention*2);
    unsigned long keyBitLen = dimention*16*sizeof(unsigned long);
    FixedBigNumber from(keyBitLen, dimention*2);
    FixedBigNumber to(0UL, dimention*2);
    for (j = 0; j < FixedBigNumber::maxSignificantBit(keyBitLen)+7; ++j)
            to[j] = true;
    FixedBigNumber e(0UL, 1);
    FixedBigNumber e2(0UL, dimention*2);
    bool hasInverse;
    do {
        do {
            e = getRandom(from, to);
            e[0] = true;
        } while (!isPrime(e, primeVerification));
        e2 = e;
        hasInverse = getInverse(e2, fn, d);
    } while (!d.positive() || !hasInverse);
    return RsaKeyPair(RsaKey(e, n), RsaKey(d, n));
}


unsigned long SHA1::s(short n, unsigned long x) {
    return (x << n) | (x >> (32 - n));
}

unsigned long SHA1::f(short t, unsigned long b, unsigned long c,
        unsigned long d)
{
    if ((0 <= t) && (t <= 19)) {
        return ((b & c) | ((~b) & d));
    } else if ((40 <= t) && (t <= 59)) {
        return ((b & c) | (b & d) | (c & d));
    }
    return (b ^ c ^ d);
}
const unsigned long& SHA1::k(short t) {
    static unsigned long k1 = 0x5A827999;
    static unsigned long k2 = 0x6ED9EBA1;
    static unsigned long k3 = 0x8F1BBCDC;
    static unsigned long k4 = 0xCA62C1D6;
    if ((0 <= t) && (t <= 19)) {
        return k1;
    } else if ((20 <= t) && (t <= 39)) {
        return k2;
    } else if ((40 <= t) && (t <= 59)) {
        return k3;
    }
    return k4;
}

SHA1::PaddedData::PaddedData(const void* data, unsigned long bytes):
        data_((const unsigned char*)data), len_(bytes), one_(0x80),
        empty_(0x00)
{
    bytes <<= 3;
    short i;
    for(i = 7; i > (8 - sizeof(unsigned long)) ; --i) {
        bits_[i] = bytes;
        bytes >>= 8;
    }
    bits_[i] = bytes;
    for(i = 0; i < (8 - sizeof(unsigned long)); ++i) {
        bits_[i] = empty_;
    }
    unsigned long tmp = len_%64;
    if (tmp > 55) {
        emptyLen_ = 119 - tmp;
    } else {
        emptyLen_ = 55 - tmp;
    }
}

unsigned char SHA1::PaddedData::at(unsigned long pos) {
    if (pos < len_) {
        return data_[pos];
    } else if (pos == len_) {
        return one_;
    } else if (pos <= (len_ + emptyLen_)) {
        return empty_;
    }
    return bits_[pos - len_ - emptyLen_ - 1];
}

unsigned long SHA1::PaddedData::blockCount() {
    return (len_ + emptyLen_ + 9)/64;
}

unsigned long SHA1::PaddedData::getWord32(unsigned long blockNum, short wordNum)
{
    unsigned long res = 0;
    for (short i = 3; i > 0; --i) {
        res |= long(at(blockNum*64 + wordNum*4 + 3 - i))
                << (i*8);
    }
    res |= long(at(blockNum*64 + wordNum*4 + 3));
    return res;
}

void SHA1::appendHashString(unsigned long x, std::string& res) {
    for (short i = 24; i > 0; i -= 8) {
        res += char(x >> i);
    }
    res += char(x);
}

std::string SHA1::getHex(const std::string& x) {
    static const char hex[] = "0123456789ABCDEF";
    std::string res(x.length()*2,'0');
    for(unsigned long i = 0; i < x.length(); ++i) {
        res[i*2] = hex[char(x[i]) >> 4];
        res[i*2 + 1] = hex[x[i] & 0x0F];
    }
    return res;
}

std::string SHA1::getDigest(const void* data, unsigned long len,
        bool isData) {
    PaddedData pd(data, len);
    unsigned long a,b,c,d,e;
    unsigned long h0 = 0x67452301UL;
    unsigned long h1 = 0xEFCDAB89UL;
    unsigned long h2 = 0x98BADCFEUL;
    unsigned long h3 = 0x10325476UL;
    unsigned long h4 = 0xC3D2E1F0UL;
    unsigned long w[80];
    unsigned long temp;
    for (unsigned long i = 0; i < pd.blockCount(); ++i) {
        short t;
        for (t = 0; t < 16; ++t) {
            w[t] = pd.getWord32(i, t);
        }
        for (t = 16; t < 80; ++t) {
            w[t] = s(1, (w[t-3] ^ w[t-8] ^ w[t-14] ^ w[t-16]));
        }
        a = h0;
        b = h1;
        c = h2;
        d = h3;
        e = h4;
        for (t = 0; t < 80; ++t) {
            temp = (s(5,a) + f(t, b, c, d) + e + w[t] + k(t));
            e = d;
            d = c;
            c = s(30, b);
            b = a;
            a = temp;
        }
        h0 = h0 + a;
        h1 = h1 + b;
        h2 = h2 + c;
        h3 = h3 + d;
        h4 = h4 + e;
    }
    std::string res("");
    appendHashString(h0, res);
    appendHashString(h1, res);
    appendHashString(h2, res);
    appendHashString(h3, res);
    appendHashString(h4, res);
    return (isData) ? res : getHex(res);
}

#ifdef HAVE_NAMESPACES
}
#endif
