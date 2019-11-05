#include "multiple_precision.hpp"

const UINT_64 MP::CARRY      = 1000000000;
      UINT_64 MP::MAX_DEPTH  = 10000;
const INT_64  MP::INT_64_MAX = 999999999999999999;

static_assert(sizeof(UINT_32) == 4,
              "invalid size of \"unsigned int\"");
static_assert(sizeof(UINT_64) == 8,
              "invalid size of \"unsigned long long int\"");
static_assert(sizeof( INT_64) == 8,
              "invalid size of \"long long int\"");

MultiplePrecision::MultiplePrecision():
    mIntegerPart(),
    mDecimalPart(),
    mIsPositive(true)
{
}

MultiplePrecision::MultiplePrecision(INT_64 value):
    mIntegerPart(),
    mDecimalPart(),
    mIsPositive((value >= 0) ? true : false)
{
    if(!mIsPositive)
        value *= -1;
    if(value > INT_64_MAX)
        throw std::range_error("exceeded INT_64_MAX");
    
    
    UINT_32 high = value / CARRY;
    UINT_32 low  = value % CARRY;

    if(high > 0)
    {
        mIntegerPart.push_back(low);
        mIntegerPart.push_back(high);
    }
    else if(low > 0)
    {
        mIntegerPart.push_back(low);
    }
}

MultiplePrecision::MultiplePrecision(int value):
    MultiplePrecision(INT_64(value))
{
}

MultiplePrecision::MultiplePrecision(long value):
    MultiplePrecision(INT_64(value))
{
}

MultiplePrecision::MultiplePrecision(const char* value):
    mIntegerPart(),
    mDecimalPart(),
    mIsPositive(true)
{
    if(*value == '+')
    {
        mIsPositive = true;
        value++;
    }
    else if(*value == '-')
    {
        mIsPositive = false;
        value++;
    }

    LEON::vector<char> integer;
    LEON::vector<char> decimal;
    while(*value)
    {
        if(*value >= '0' && *value <= '9')
            integer.push_back(*value);
        else if(*value == '.')
        {
            value++;
            break;
        }
        value++;
    }
    while(*value)
    {
        if(*value >= '0' && *value <= '9')
            decimal.push_back(*value);
        
        value++;
    }

    UINT_32 rate = 1;
    UINT_32 val  = 0;
    for(auto iter = integer.rbegin();
        iter != integer.rend();
        iter++)
    {
        val   = val + (*iter - '0') * rate;
        rate *= 10;

        if(rate == MP::CARRY)
        {
            mIntegerPart.push_back(val);

            rate = 1;
            val  = 0;
        }
    }
    mIntegerPart.push_back(val);
    
    rate = MP::CARRY;
    val  = 0;
    
    for(auto iter = decimal.begin();
        iter != decimal.end();
        iter++)
    {
        rate /= 10;
        val   = val + (*iter - '0') * rate;
        
        if(rate == 1)
        {
            mDecimalPart.push_back(val);
            
            rate = MP::CARRY;
            val  = 0;
        }
    }
    mDecimalPart.push_back(val);

    shrink();
}

MultiplePrecision::MultiplePrecision(const MP& other)
{
    mIntegerPart = other.mIntegerPart;
    mDecimalPart = other.mDecimalPart;
    mIsPositive  = other.mIsPositive;
}
MultiplePrecision::MultiplePrecision(MP&& other)
{
    mIntegerPart = std::move(other.mIntegerPart);
    mDecimalPart = std::move(other.mDecimalPart);
    mIsPositive  = other.mIsPositive;
}

MultiplePrecision::~MultiplePrecision()
{
}

MP& MultiplePrecision::operator=(const MP& other)
{
    mIntegerPart = other.mIntegerPart;
    mDecimalPart = other.mDecimalPart;
    mIsPositive  = other.mIsPositive;

    return *this;
}

MP& MultiplePrecision::operator=(MP&& other)
{
    mIntegerPart = other.mIntegerPart;
    mDecimalPart = other.mDecimalPart;
    mIsPositive  = other.mIsPositive;

    return *this;
}

MP& MultiplePrecision::operator+=(const MP& other)
{
    if(mIsPositive == other.mIsPositive)
        MP::addition(*this, *this, other);
    else if(!mIsPositive && other.mIsPositive)
        MP::subtraction(*this, other, *this);
    else if(mIsPositive && !other.mIsPositive)
        MP::subtraction(*this, *this, other);

    return *this;
}

MP& MultiplePrecision::operator-=(const MP& other)
{
    if(mIsPositive && other.mIsPositive)
        MP::subtraction(*this, *this, other);
    else if(mIsPositive != other.mIsPositive)
        MP::addition(*this, *this, other);
    else if(!mIsPositive && !other.mIsPositive)
        MP::subtraction(*this, other, *this);
    
    return *this;
}

MP& MultiplePrecision::operator*=(const MP& other)
{
    multiplication(*this, *this, other);
    
    return *this;
}

MP operator+(const MP& lhs, const MP& rhs)
{
    MP mp;
    
    if(lhs.mIsPositive == rhs.mIsPositive)
        MP::addition(mp, lhs, rhs);
    else if(!lhs.mIsPositive && rhs.mIsPositive)
        MP::subtraction(mp, rhs, lhs);
    else if(lhs.mIsPositive && rhs.mIsPositive)
        MP::subtraction(mp, lhs, rhs);

    return mp;
}

MP operator-(const MP& lhs, const MP& rhs)
{
    MP mp;

    if(lhs.mIsPositive && rhs.mIsPositive)
        MP::subtraction(mp, lhs, rhs);
    else if(lhs.mIsPositive != rhs.mIsPositive)
        MP::addition(mp, lhs, rhs);
    else if(!lhs.mIsPositive && rhs.mIsPositive)
        MP::subtraction(mp, rhs, lhs);

    return mp;
}

MP operator*(const MP& lhs, const MP& rhs)
{
    MP mp;

    MP::multiplication(mp, lhs, rhs);

    return mp;
}

void MultiplePrecision::print(const MP& mp)
{
    std::cout << "sign: "
              << ((mp.mIsPositive) ? "+" : "-")
              << std::endl;

    for(auto iter = mp.mIntegerPart.rbegin();
        iter != mp.mIntegerPart.rend();
        iter++)
    {
        std::cout << std::setw(9) 
                  << *iter
                  << " ";
        std::cout.fill('0');
    }
    std::cout << std::endl;

    for(auto iter = mp.mDecimalPart.begin();
        iter != mp.mDecimalPart.end();
        iter++)
    {
        std::cout << std::setw(9) 
                  << *iter
                  << " ";
        std::cout.fill('0');
    }
    std::cout << std::endl;
}

void MultiplePrecision::addition(MP& dst,
                                 const MP& lhs, const MP& rhs)
{
    const auto& decMaxPart
        = (lhs.mDecimalPart.size() > rhs.mDecimalPart.size())
            ? lhs.mDecimalPart : rhs.mDecimalPart;
    const auto& decMinPart
        = (lhs.mDecimalPart.size() <= rhs.mDecimalPart.size())
            ? lhs.mDecimalPart : rhs.mDecimalPart;
    const auto& intMaxPart
        = (lhs.mIntegerPart.size() > rhs.mIntegerPart.size())
            ? lhs.mIntegerPart : rhs.mIntegerPart;
    const auto& intMinPart
        = (lhs.mIntegerPart.size() <= rhs.mIntegerPart.size())
            ? lhs.mIntegerPart : rhs.mIntegerPart;

    dst.mDecimalPart.resize(decMaxPart.size(), 0);
    dst.mIntegerPart.resize(intMaxPart.size(), 0);

    UINT_64 carry = 0;

    // 小数部
    for(UINT_64 i = decMaxPart.size();
        i > decMinPart.size();
        i--)
    {
        dst.mDecimalPart.at(i - 1)
            = decMaxPart.at(i - 1);
    }
    for(UINT_64 i = decMinPart.size();
        i > 0;
        i--)
    {
        UINT_64 val
            = static_cast<UINT_64>(decMinPart.at(i - 1)) +
              static_cast<UINT_64>(decMaxPart.at(i - 1)) +
              carry;
        
        dst.mDecimalPart.at(i - 1)
            = static_cast<UINT_32>(val % MP::CARRY);
        carry
            = (val >= MP::CARRY)
                ? 1 : 0;
    }
    // 整数部
    for(UINT_64 i = 0;
        i < intMinPart.size();
        i++)
    {
        UINT_64 val
            = static_cast<UINT_64>(intMinPart.at(i)) +
              static_cast<UINT_64>(intMaxPart.at(i)) +
              carry;
        
        dst.mIntegerPart.at(i)
            = static_cast<UINT_32>(val % MP::CARRY);
        carry
            = (val >= MP::CARRY)
                ? 1 : 0;
    }
    for(UINT_64 i = intMinPart.size();
        i < intMaxPart.size();
        i++)
    {
        UINT_64 val
            = static_cast<UINT_64>(intMaxPart.at(i)) +
              carry;
        
        dst.mIntegerPart.at(i)
            = static_cast<UINT_32>(val % MP::CARRY);
        carry
            = (val >= MP::CARRY)
                ? 1 : 0;
    }

    dst.mIsPositive = lhs.mIsPositive;
    
    dst.mIntegerPart.push_back(static_cast<UINT_32>(carry));
    dst.shrink();
}

void MultiplePrecision::subtraction(MP& dst,
                                    const MP& lhs, const MP& rhs)
{
    UINT_64 dSize
        = (lhs.mDecimalPart.size() > rhs.mDecimalPart.size())
            ? lhs.mDecimalPart.size() : rhs.mDecimalPart.size();
    UINT_64 iSize
        = (lhs.mIntegerPart.size() > rhs.mIntegerPart.size())
            ? lhs.mIntegerPart.size() : rhs.mIntegerPart.size();
    
    dst.mDecimalPart.resize(dSize, 0);
    dst.mIntegerPart.resize(iSize, 0);

    UINT_64 carry = 0;

    // 小数部
    if(lhs.mDecimalPart.size() > rhs.mDecimalPart.size())
    {
        for(UINT_64 i = lhs.mDecimalPart.size();
            i > rhs.mDecimalPart.size();
            i--)
        {
            dst.mDecimalPart.at(i - 1)
                = lhs.mDecimalPart.at(i - 1);
        }
    }
    else if(lhs.mDecimalPart.size() < rhs.mDecimalPart.size())
    {
        for(UINT_64 i = rhs.mDecimalPart.size();
            i > lhs.mDecimalPart.size();
            i--)
        {
            UINT_64 val
                = MP::CARRY -
                  carry     -
                  static_cast<UINT_64>(rhs.mDecimalPart.at(i - 1));
            
            dst.mDecimalPart.at(i - 1)
                = static_cast<UINT_32>(val % MP::CARRY);
            carry
                = (val >= MP::CARRY)
                    ? 0 : 1;
        }
    }
    dSize
        = (lhs.mDecimalPart.size() < rhs.mDecimalPart.size())
            ? lhs.mDecimalPart.size() : rhs.mDecimalPart.size();
    for(UINT_64 i = dSize;
        i > 0;
        i--)
    {
        UINT_64 val
            = MP::CARRY +
              static_cast<UINT_64>(lhs.mDecimalPart.at(i - 1)) -
              static_cast<UINT_64>(rhs.mDecimalPart.at(i - 1)) -
              carry;
        
        dst.mDecimalPart.at(i - 1)
            = static_cast<UINT_32>(val % MP::CARRY);
        carry
            = (val >= MP::CARRY)
                ? 0 : 1;
    }
    // 整数部
    iSize
        = (lhs.mIntegerPart.size() < rhs.mIntegerPart.size())
            ? lhs.mIntegerPart.size() : rhs.mIntegerPart.size();
    for(UINT_64 i = 0;
        i < iSize;
        i++)
    {
        UINT_64 val
            = MP::CARRY +
              static_cast<UINT_64>(lhs.mIntegerPart.at(i)) -
              static_cast<UINT_64>(rhs.mIntegerPart.at(i)) -
              carry;
        
        dst.mIntegerPart.at(i)
            = static_cast<UINT_32>(val % MP::CARRY);
        carry
            = (val >= MP::CARRY)
                ? 0 : 1;
    }
    if(lhs.mIntegerPart.size() > rhs.mIntegerPart.size())
    {
        for(UINT_64 i = rhs.mIntegerPart.size();
            i < lhs.mIntegerPart.size();
            i++)
        {
            UINT_64 val
                = MP::CARRY +
                  static_cast<UINT_64>(lhs.mIntegerPart.at(i)) -
                  carry;
            
            dst.mIntegerPart.at(i)
                = static_cast<UINT_32>(val % MP::CARRY);
            carry
                = (val >= MP::CARRY)
                    ? 0 : 1;
        }
    }
    else if(lhs.mIntegerPart.size() < rhs.mIntegerPart.size())
    {
        for(UINT_64 i = lhs.mIntegerPart.size();
            i < rhs.mIntegerPart.size();
            i++)
        {
            UINT_64 val
                = MP::CARRY -
                  static_cast<UINT_64>(rhs.mIntegerPart.at(i)) -
                  carry;
            
            dst.mIntegerPart.at(i)
                = static_cast<UINT_32>(val % MP::CARRY);
            carry
                = (val >= MP::CARRY)
                    ? 0 : 1;
        }
    }

    // 正負反転
    if(carry == 1)
    {
        for(auto& e : dst.mDecimalPart)
            e = MP::CARRY - e;
        for(auto& e : dst.mIntegerPart)
            e = MP::CARRY - e;

        dst.mIsPositive = !dst.mIsPositive;
    }

    dst.shrink();
}

void MultiplePrecision::multiplication(MP& dst,
                                       const MP& lhs, const MP& rhs)
{
    MP result;
    
    UINT_64 dSize
        = lhs.mDecimalPart.size() + rhs.mDecimalPart.size();
    UINT_64 iSize
        = lhs.mIntegerPart.size() + rhs.mIntegerPart.size();

    result.mIntegerPart.resize(dSize + iSize, 0);

    UINT_64 carry = 0;

    // 小数部
    for(UINT_64 i = rhs.mDecimalPart.size();
        i > 0;
        i--)
    {
        MP temp;
        temp.mIntegerPart.resize(rhs.mDecimalPart.size() - i, 0);

        carry = 0;

        for(UINT_64 j = lhs.mDecimalPart.size();
            j > 0;
            j--)
        {
            UINT_64 val
                = static_cast<UINT_64>(lhs.mDecimalPart.at(j - 1)) *
                  static_cast<UINT_64>(rhs.mDecimalPart.at(i - 1)) +
                  carry;
            
            UINT_32 v
                = static_cast<UINT_32>(val % MP::CARRY);
            carry
                = val / MP::CARRY;

            temp.mIntegerPart.push_back(v);
        }
        for(UINT_64 j = 0;
            j < lhs.mIntegerPart.size();
            j++)
        {
            UINT_64 val
                = static_cast<UINT_64>(lhs.mIntegerPart.at(j))     *
                  static_cast<UINT_64>(rhs.mDecimalPart.at(i - 1)) +
                  carry;
            
            UINT_32 v
                = static_cast<UINT_32>(val % MP::CARRY);
            carry
                = val / MP::CARRY;

            temp.mIntegerPart.push_back(v);
        }

        temp.mIntegerPart.push_back(static_cast<UINT_32>(carry));

        addition(result, temp, result);
    }
    // 整数部
    for(UINT_64 i = 0;
        i < rhs.mIntegerPart.size();
        i++)
    {
        MP temp;
        temp.mIntegerPart.resize(rhs.mDecimalPart.size() + i, 0);

        carry = 0;

        for(UINT_64 j = lhs.mDecimalPart.size();
            j > 0;
            j--)
        {
            UINT_64 val
                = static_cast<UINT_64>(lhs.mDecimalPart.at(j - 1)) *
                  static_cast<UINT_64>(rhs.mIntegerPart.at(i))     +
                  carry;
            
            UINT_32 v
                = static_cast<UINT_32>(val % MP::CARRY);
            carry
                = val / MP::CARRY;

            temp.mIntegerPart.push_back(v);
        }
        for(UINT_64 j = 0;
            j < lhs.mIntegerPart.size();
            j++)
        {
            UINT_64 val
                = static_cast<UINT_64>(lhs.mIntegerPart.at(j)) *
                  static_cast<UINT_64>(rhs.mIntegerPart.at(i)) +
                  carry;
            
            UINT_32 v
                = static_cast<UINT_32>(val % MP::CARRY);
            carry
                = val / MP::CARRY;
            
            temp.mIntegerPart.push_back(v);
        }

        temp.mIntegerPart.push_back(static_cast<UINT_32>(carry));

        addition(result, temp, result);
    }

    for(UINT_64 i = dSize; i > 0; i--)
    {
        result.mDecimalPart
            .push_back(result.mIntegerPart.at(i - 1));
    }
    result.mIntegerPart.erase(result.mIntegerPart.begin(),
                              result.mIntegerPart.begin() + dSize);

    result.mIsPositive
        = (lhs.mIsPositive == rhs.mIsPositive)
            ? true : false;

    result.shrink();
    dst = result;
}

void MultiplePrecision::shrink()
{
    // 整数部
    if(!mIntegerPart.empty() &&
        mIntegerPart.back() == 0)
    {
        mIntegerPart.pop_back();
    }

    // 小数部
    if(!mDecimalPart.empty() &&
        mDecimalPart.back() == 0)
    {
        mDecimalPart.pop_back();
    }
}