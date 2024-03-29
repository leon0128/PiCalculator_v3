#include "multiple_precision.hpp"

const UINT_64 MP::CARRY      = 1000000000;
      UINT_64 MP::MAX_DEPTH  = 1000;
const  INT_64 MP::INT_64_MAX = 999999999999999999;

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
    mIntegerPart.reserve(MP::MAX_DEPTH + MP::MAX_DEPTH * 1.1);
    mDecimalPart.reserve(MP::MAX_DEPTH + MP::MAX_DEPTH * 1.1);
}

MultiplePrecision::MultiplePrecision(INT_64 value):
    mIntegerPart(),
    mDecimalPart(),
    mIsPositive((value >= 0) ? true : false)
{
    mIntegerPart.reserve(MP::MAX_DEPTH + MP::MAX_DEPTH * 1.1);
    mDecimalPart.reserve(MP::MAX_DEPTH + MP::MAX_DEPTH * 1.1);

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

MultiplePrecision::MultiplePrecision(UINT_64 value):
    MultiplePrecision(static_cast<INT_64>(value))
{
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
    mIntegerPart.reserve(MP::MAX_DEPTH + MP::MAX_DEPTH * 1.1);
    mDecimalPart.reserve(MP::MAX_DEPTH + MP::MAX_DEPTH * 1.1);

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
    {
        MP::addition(*this, *this, other);
    }
    else
    {
        const MP& larger
            = absoluteMax(*this, other);
        const MP& smaller
            = (&larger == this)
                ? other : *this;

        MP::subtraction(*this, larger, smaller);
        mIsPositive = larger.mIsPositive;
    }

    return *this;
}

MP& MultiplePrecision::operator-=(const MP& other)
{
    if(mIsPositive != other.mIsPositive)
    {
        MP::addition(*this, *this, other);
    }
    else
    {
        const MP& larger
            = absoluteMax(*this, other);
        const MP& smaller
            = (&larger == this)
                ? other : *this;

        MP::subtraction(*this, larger, smaller);
        mIsPositive
            = (&larger == this)
                ? mIsPositive : !mIsPositive;
    }
    
    return *this;
}

MP& MultiplePrecision::operator*=(const MP& other)
{
    multiplication(*this, *this, other);
    
    return *this;
}

MP& MultiplePrecision::operator/=(const MP& other)
{
    division(*this, *this, other);

    return *this;
}

MP operator+(const MP& lhs, const MP& rhs)
{
    MP mp;

    if(lhs.mIsPositive == rhs.mIsPositive)
    {
        MP::addition(mp, lhs, rhs);
    }
    else
    {
        const MP& larger
            = MP::absoluteMax(lhs, rhs);
        const MP& smaller
            = (&larger == &lhs)
                ? rhs : lhs;

        MP::subtraction(mp, larger, smaller);
        mp.mIsPositive = larger.mIsPositive;
    }

    return mp;
}

MP operator-(const MP& lhs, const MP& rhs)
{
    MP mp;

    if(lhs.mIsPositive != rhs.mIsPositive)
    {
        MP::addition(mp, lhs, rhs);
    }
    else
    {
        const MP& larger
            = MP::absoluteMax(lhs, rhs);
        const MP& smaller
            = (&larger == &lhs)
                ? rhs : lhs;

        MP::subtraction(mp, larger, smaller);
        mp.mIsPositive
            = (&larger == &lhs)
                ? lhs.mIsPositive : !lhs.mIsPositive;
    }

    return mp;
}

MP operator*(const MP& lhs, const MP& rhs)
{
    MP mp;

    MP::multiplication(mp, lhs, rhs);

    return mp;
}

MP operator/(const MP& lhs, const MP& rhs)
{
    MP mp;

    MP::division(mp, lhs, rhs);

    return mp;
}

bool operator==(const MP& lhs, const MP& rhs)
{
    if(lhs.mIsPositive != rhs.mIsPositive)
        return false;
    if(lhs.mIntegerPart.size() != rhs.mIntegerPart.size())
        return false;
    if(lhs.mDecimalPart.size() != rhs.mDecimalPart.size())
        return false;
    
    for(UINT_64 i = 0; i < lhs.mIntegerPart.size(); i++)
    {
        if(lhs.mIntegerPart[i] != rhs.mIntegerPart[i])
            return false;
    }
    for(UINT_64 i = 0; i < lhs.mDecimalPart.size(); i++)
    {
        if(lhs.mDecimalPart[i] != rhs.mDecimalPart[i])
            return false;
    }

    return true;
}

bool operator!=(const MP& lhs, const MP& rhs)
{
    return !(lhs == rhs);
}

bool operator<(const MP& lhs, const MP& rhs)
{
    if(!lhs.mIsPositive && rhs.mIsPositive)
        return true;
    else if(lhs.mIsPositive && !rhs.mIsPositive)
        return false;

    if(lhs.mIntegerPart.size() < rhs.mIntegerPart.size())
        return lhs.mIsPositive ? true : false;
    else if(lhs.mIntegerPart.size() > rhs.mIntegerPart.size())
        return lhs.mIsPositive ? false : true;
    
    for(UINT_64 i = lhs.mIntegerPart.size(); i > 0; i--)
    {
        if(lhs.mIntegerPart[i - 1] < rhs.mIntegerPart[i - 1])
            return lhs.mIsPositive ? true : false;
        else if(lhs.mIntegerPart[i - 1] > rhs.mIntegerPart[i - 1])
            return lhs.mIsPositive ? false : true;
    }
    
    UINT_64 dSize
        = (lhs.mDecimalPart.size() < rhs.mDecimalPart.size())
            ? lhs.mDecimalPart.size() : rhs.mDecimalPart.size();
    for(UINT_64 i = 0; i < dSize; i++)
    {
        if(lhs.mDecimalPart[i] < rhs.mDecimalPart[i])
            return lhs.mIsPositive ? true : false;
        else if(lhs.mDecimalPart[i] > rhs.mDecimalPart[i])
            return lhs.mIsPositive ? false : true;
    }
    
    if(lhs.mDecimalPart.size() < rhs.mDecimalPart.size())
        return lhs.mIsPositive ? true : false;
    else
        return lhs.mIsPositive ? false : true;
}

bool operator<=(const MP& lhs, const MP& rhs)
{
    if(!lhs.mIsPositive && rhs.mIsPositive)
        return true;
    else if(lhs.mIsPositive && !rhs.mIsPositive)
        return false;

    if(lhs.mIntegerPart.size() < rhs.mIntegerPart.size())
        return lhs.mIsPositive ? true : false;
    else if(lhs.mIntegerPart.size() > rhs.mIntegerPart.size())
        return lhs.mIsPositive ? false : true;
    
    for(UINT_64 i = lhs.mIntegerPart.size(); i > 0; i--)
    {
        if(lhs.mIntegerPart[i - 1] < rhs.mIntegerPart[i - 1])
            return lhs.mIsPositive ? true :false;
        else if(lhs.mIntegerPart[i - i] > rhs.mIntegerPart[i - 1])
            return lhs.mIsPositive ? false : true;
    }

    UINT_64 dSize
        = (lhs.mDecimalPart.size() < rhs.mDecimalPart.size())
            ? lhs.mDecimalPart.size() : rhs.mDecimalPart.size();
    for(UINT_64 i = 0; i < dSize; i++)
    {
        if(lhs.mDecimalPart[i] < rhs.mDecimalPart[i])
            return lhs.mIsPositive ? true : false;
        else if(lhs.mDecimalPart[i] > rhs.mDecimalPart[i])
            return lhs.mIsPositive ? false : true;
    }

    if(lhs.mDecimalPart.size() <= rhs.mDecimalPart.size())
        return lhs.mIsPositive ? true : false;
    else
        return lhs.mIsPositive ? false : true;
}

bool operator>(const MP& lhs, const MP& rhs)
{
    return !(lhs <= rhs);
}

bool operator>=(const MP& lhs, const MP& rhs)
{
    return !(lhs < rhs);
}

MP MultiplePrecision::factorial(INT_64 count)
{
    MP value(1);
    
    for(INT_64 i = 0; i < count; i++)
        value *= MP(i + 1);

    return value;
}

MP MultiplePrecision::power(const MP& radix, UINT_64 index)
{
    MP value(1);

    for(UINT_64 i = 0; i < index; i++)
        value *= radix;

    return value;
}

const MP& MultiplePrecision::absoluteMax(const MP& lhs, const MP& rhs)
{
    if(lhs.mIntegerPart.size() > rhs.mIntegerPart.size())
        return lhs;
    else if(lhs.mIntegerPart.size() < rhs.mIntegerPart.size())
        return rhs;

    for(UINT_64 i = lhs.mIntegerPart.size(); i > 0; i--)
    {
        if(lhs.mIntegerPart[i - 1] > rhs.mIntegerPart[i - 1])
            return lhs;
        else if(lhs.mIntegerPart[i - 1] < rhs.mIntegerPart[i - 1])
            return rhs;
    }

    UINT_64 dSize
        = (lhs.mDecimalPart.size() < rhs.mDecimalPart.size())
            ? lhs.mDecimalPart.size() : rhs.mDecimalPart.size();
    for(UINT_64 i = 0; i < dSize; i++)
    {
        if(lhs.mDecimalPart[i] > rhs.mDecimalPart[i])
            return lhs;
        else if(lhs.mDecimalPart[i] < rhs.mDecimalPart[i])
            return rhs;
    }

    if(lhs.mDecimalPart.size() > rhs.mDecimalPart.size())
        return lhs;
    else
        return rhs;
}

MP& MultiplePrecision::absoluteMax(MP& lhs, MP& rhs)
{
    const MP& l = lhs;
    const MP& r = rhs;
    return (&absoluteMax(l, r) == &lhs) ? lhs : rhs;
}

UINT_64 MultiplePrecision::convert(const MP& mp)
{
    if(mp.mIntegerPart.size() == 0)
        return 0;
    
    if(mp.mIntegerPart.size() == 1)
        return mp.mIntegerPart.front();
    
    UINT_64 val
        = static_cast<UINT_64>(mp.mIntegerPart[1]) *
          MP::CARRY +
          static_cast<UINT_64>(mp.mIntegerPart[0]);
    return val;
}

void MultiplePrecision::output(const MP& mp, bool isPunctuated)
{
    std::cout << (mp.mIsPositive ? "+" : "-") << std::flush;

    if(mp.mIntegerPart.empty())
        std::cout << "0";
    else
    {
        std::cout << (*mp.mIntegerPart.rbegin());
    
        for(auto iter = mp.mIntegerPart.rbegin() + 1;
            iter != mp.mIntegerPart.rend(); 
            iter++)
        {
            for(int i = MP::CARRY / 10; i >= 1; i /= 10)
            {
                if(*iter / i == 0)
                    std::cout << "0";
                else
                {
                    std::cout << (*iter);
                    break;
                }
            }
        }
    }

    std::cout << "." << std::flush;

    if(isPunctuated)
    {
        UINT_64 count = 0;
        for(auto iter = mp.mDecimalPart.begin();
            iter != mp.mDecimalPart.end();
            iter++)
        {
            for(int i = MP::CARRY / 10; i >= 1; i /= 10)
            {
                if(count++ >= MP::MAX_DEPTH)
                    break;
                std::cout << ((*iter / i) % 10);
            }
            if(count >= MP::MAX_DEPTH)
                break;
        }
    }
    else
    {
        for(auto iter = mp.mDecimalPart.begin();
            iter != mp.mDecimalPart.end();
            iter++)
        {
            for(int i = MP::CARRY / 10; i >= 1; i /= 10)
            {
                std::cout << (*iter / i);
                if(*iter / i == 0)
                    std::cout << "0";
                else
                {
                    std::cout << (*iter);
                    break;
                }
            }
        }        
    }

    std::cout << " ["
              << ((isPunctuated)
                    ? MP::MAX_DEPTH : mp.mDecimalPart.size() * 9) 
              << "]" << std::endl;
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
        dst.mDecimalPart[i - 1]
            = decMaxPart[i - 1];
    }
    for(UINT_64 i = decMinPart.size();
        i > 0;
        i--)
    {
        UINT_64 val
            = static_cast<UINT_64>(decMinPart[i - 1]) +
              static_cast<UINT_64>(decMaxPart[i - 1]) +
              carry;
        
        dst.mDecimalPart[i - 1]
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
            = static_cast<UINT_64>(intMinPart[i]) +
              static_cast<UINT_64>(intMaxPart[i]) +
              carry;
        
        dst.mIntegerPart[i]
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
            = static_cast<UINT_64>(intMaxPart[i]) +
              carry;
        
        dst.mIntegerPart[i]
            = static_cast<UINT_32>(val % MP::CARRY);
        carry
            = (val >= MP::CARRY)
                ? 1 : 0;
    }

    dst.mIsPositive
        = lhs.mIsPositive;
    
    dst.mIntegerPart.push_back(static_cast<UINT_32>(carry));
    dst.shrink();
}

void MultiplePrecision::subtraction(MP& dst,
                                    const MP& lhs, const MP& rhs)
{

    const MP& larger  = lhs;
    const MP& smaller = rhs;

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
    if(larger.mDecimalPart.size() > smaller.mDecimalPart.size())
    {
        for(UINT_64 i = larger.mDecimalPart.size();
            i > smaller.mDecimalPart.size();
            i--)
        {
            dst.mDecimalPart[i - 1]
                = larger.mDecimalPart[i - 1];
        }
    }
    else if(larger.mDecimalPart.size() < smaller.mDecimalPart.size())
    {
        for(UINT_64 i = smaller.mDecimalPart.size();
            i > larger.mDecimalPart.size();
            i--)
        {
            UINT_64 val
                = MP::CARRY -
                  carry     -
                  static_cast<UINT_64>(smaller.mDecimalPart[i - 1]);
            
            dst.mDecimalPart[i - 1]
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
              static_cast<UINT_64>(larger.mDecimalPart[i - 1]) -
              static_cast<UINT_64>(smaller.mDecimalPart[i - 1]) -
              carry;
        
        dst.mDecimalPart[i - 1]
            = static_cast<UINT_32>(val % MP::CARRY);
        carry
            = (val >= MP::CARRY)
                ? 0 : 1;
    }
    // 整数部
    iSize
        = (larger.mIntegerPart.size() < smaller.mIntegerPart.size())
            ? larger.mIntegerPart.size() : smaller.mIntegerPart.size();
    for(UINT_64 i = 0;
        i < iSize;
        i++)
    {
        UINT_64 val
            = MP::CARRY +
              static_cast<UINT_64>(larger.mIntegerPart[i]) -
              static_cast<UINT_64>(smaller.mIntegerPart[i]) -
              carry;
        
        dst.mIntegerPart[i]
            = static_cast<UINT_32>(val % MP::CARRY);
        carry
            = (val >= MP::CARRY)
                ? 0 : 1;
    }
    if(larger.mIntegerPart.size() > smaller.mIntegerPart.size())
    {
        for(UINT_64 i = smaller.mIntegerPart.size();
            i < larger.mIntegerPart.size();
            i++)
        {
            UINT_64 val
                = MP::CARRY +
                  static_cast<UINT_64>(larger.mIntegerPart[i]) -
                  carry;
            
            dst.mIntegerPart[i]
                = static_cast<UINT_32>(val % MP::CARRY);
            carry
                = (val >= MP::CARRY)
                    ? 0 : 1;
        }
    }
    else if(larger.mIntegerPart.size() < smaller.mIntegerPart.size())
    {
        for(UINT_64 i = larger.mIntegerPart.size();
            i < smaller.mIntegerPart.size();
            i++)
        {
            UINT_64 val
                = MP::CARRY -
                  static_cast<UINT_64>(smaller.mIntegerPart[i]) -
                  carry;
            
            dst.mIntegerPart[i]
                = static_cast<UINT_32>(val % MP::CARRY);
            carry
                = (val >= MP::CARRY)
                    ? 0 : 1;
        }
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
                = static_cast<UINT_64>(lhs.mDecimalPart[j - 1]) *
                  static_cast<UINT_64>(rhs.mDecimalPart[i - 1]) +
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
                = static_cast<UINT_64>(lhs.mIntegerPart[j])     *
                  static_cast<UINT_64>(rhs.mDecimalPart[i - 1]) +
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
                = static_cast<UINT_64>(lhs.mDecimalPart[j - 1]) *
                  static_cast<UINT_64>(rhs.mIntegerPart[i])     +
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
                = static_cast<UINT_64>(lhs.mIntegerPart[j]) *
                  static_cast<UINT_64>(rhs.mIntegerPart[i]) +
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

    for(UINT_64 i = result.mIntegerPart.size();
        i < dSize;
        i++)
    {
        result.mIntegerPart.push_back(0);
    }

    for(UINT_64 i = dSize; i > 0; i--)
    {
        result.mDecimalPart
            .push_back(result.mIntegerPart[i - 1]);
    }
    result.mIntegerPart.erase(result.mIntegerPart.begin(),
                              result.mIntegerPart.begin() + dSize);

    result.mIsPositive
        = (lhs.mIsPositive == rhs.mIsPositive)
            ? true : false;

    result.shrink();
    dst = result;
}

void MultiplePrecision::division(MP& dst,
                                 const MP& lhs, const MP& rhs)
{
    MP result;
    MP diff(lhs);
    diff.mIsPositive = true;

    UINT_64 dividend = 0;
    UINT_64 divisor  = 0;
     INT_64 digit    = 0;
     INT_64 rhsOff   = rhs.offset();
 
    if(rhs.mIntegerPart.size() >= 2)
    {
        divisor
            = MP::CARRY *
              static_cast<UINT_64>(*rhs.mIntegerPart.rbegin()) +
              static_cast<UINT_64>(*(rhs.mIntegerPart.rbegin() + 1));
    }
    else if(rhs.mIntegerPart.size() == 1 &&
            !rhs.mDecimalPart.empty())
    {
        divisor
            = MP::CARRY *
              static_cast<UINT_64>(rhs.mIntegerPart.front()) +
              static_cast<UINT_64>(rhs.mDecimalPart.front());
    }
    else if(static_cast<INT_64>(rhs.mDecimalPart.size()) >= rhsOff * -1 + 1)
    {
        divisor
            = MP::CARRY *
              static_cast<UINT_64>(rhs.mDecimalPart[rhsOff * -1 - 1]) +
              static_cast<UINT_64>(rhs.mDecimalPart[rhsOff * -1]);
    }
    else if(rhs.mIntegerPart.size() == 1 &&
            rhs.mDecimalPart.empty())
    {
        divisor
            = MP::CARRY *
              static_cast<UINT_64>(rhs.mIntegerPart.front());
    }
    else if(static_cast<INT_64>(rhs.mDecimalPart.size()) == rhsOff * -1)
    {
        divisor
            = MP::CARRY *
              static_cast<UINT_64>(rhs.mDecimalPart.back());
    }
    else
    {
        diff = MP();
    }

    auto isValid
        = [&]
    {
        if(diff.mIntegerPart.empty() && diff.mDecimalPart.empty())
            return true;
        for(UINT_64 i = 0; i < diff.mDecimalPart.size(); i++)
        {
            if(diff.mDecimalPart[i] != 0)
            {
                if(i * 9 > MAX_DEPTH)
                    return true;
                else
                    return false;
            }
        }
        return false;
    };

    while(!isValid())
    {
        digitAlignment(digit, dividend, diff);

        UINT_64 val = 0;
        if(dividend >= divisor)
        {
            val    = dividend / divisor;
            digit -= rhsOff - 1;
        }
        else
        {
            val    = dividend / 
                    (divisor / MP::CARRY);
            digit -= rhsOff; 
        }
        
        MP temp;
        temp.mIsPositive
            = (diff.mIsPositive == rhs.mIsPositive)
                ? true : false;

        if(digit >= 0)
        {
            temp.mIntegerPart.resize(digit + 2, 0);
            temp.mIntegerPart[digit + 1]
                = static_cast<UINT_32>(val / MP::CARRY);
            temp.mIntegerPart[digit]
                = static_cast<UINT_32>(val % MP::CARRY);
        }
        else if(digit == -1)
        {
            temp.mIntegerPart
                .push_back(static_cast<UINT_32>(val / MP::CARRY));
            temp.mDecimalPart
                .push_back(static_cast<UINT_32>(val % MP::CARRY));
        }
        else
        {
            digit *= -1;
            temp.mDecimalPart.resize(digit, 0);
            temp.mDecimalPart[digit - 2]
                = static_cast<UINT_32>(val / MP::CARRY);
            temp.mDecimalPart[digit - 1]
                = static_cast<UINT_32>(val % MP::CARRY);
        }

        diff -= temp * rhs;
        result += temp;
    }   

    result.mIsPositive
        = (lhs.mIsPositive == rhs.mIsPositive)
            ? true : false;

    result.shrink();
    dst = result;
}

void MultiplePrecision::digitAlignment(  INT_64& digit,
                                        UINT_64& dividend,
                                       const MP& lhs)
{
    if(lhs.mIntegerPart.size() >= 2)
    {
        dividend
            = lhs.mIntegerPart[lhs.mIntegerPart.size() - 1] * MP::CARRY +
              lhs.mIntegerPart[lhs.mIntegerPart.size() - 2];
        digit
            = lhs.mIntegerPart.size() - 2;
    }
    else if(lhs.mIntegerPart.size() == 1 &&
            lhs.mDecimalPart.size() >= 1)
    {
        dividend
            = lhs.mIntegerPart.back() * MP::CARRY +
              lhs.mDecimalPart.front();
        digit
            = -1;
    }
    else if(lhs.mDecimalPart.size() >= 2)
    {
        digit = 0;
        while(lhs.mDecimalPart[digit] == 0)
            digit++;

        if(lhs.mDecimalPart.size() >= static_cast<UINT_64>(digit) + 2)
        {
            dividend
                = lhs.mDecimalPart[digit] * MP::CARRY +
                  lhs.mDecimalPart[digit + 1];
        }
        else
        {
            dividend
                = lhs.mDecimalPart[digit] * MP::CARRY;
        }

        digit
            = digit * -1 - 2;
    }
    else if(lhs.mIntegerPart.size() == 1 &&
            lhs.mDecimalPart.size() == 0)
    {
        dividend
            = lhs.mIntegerPart.front() * MP::CARRY;
        digit
            = -1;
    } 
    else if(lhs.mIntegerPart.size() == 0 &&
            lhs.mDecimalPart.size() == 1)
    {
        dividend
            = lhs.mDecimalPart.front() * MP::CARRY;
        digit
            = -2;
    }
    else
    {
        dividend
            = 0;
        digit
            = 0;
    }
}

void MultiplePrecision::shrink()
{
    // 整数部
    while(!mIntegerPart.empty())
    {
        if(mIntegerPart.back() == 0)
            mIntegerPart.pop_back();
        else
            break;
    }

    // 小数部
    while(!mDecimalPart.empty())
    {
        if(mDecimalPart.back() == 0)
            mDecimalPart.pop_back();
        else
            break;
    }
}

INT_64 MultiplePrecision::offset() const
{
    if(mIntegerPart.size() > 0)
        return mIntegerPart.size() - 1;

    for(UINT_64 i = 0; i < mDecimalPart.size(); i++)
    {
        if(mDecimalPart[i] != 0)
            return static_cast<INT_64>(i) * -1;
    }
    
    return 0;
}