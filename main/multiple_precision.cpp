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

// MultiplePrecision::MultiplePrecision(const char* value):
//     mIntegerPart(),
//     mDecimalPart(),
//     mIsPositive(true)
// {
//     LEON::vector<char> elements;
//     while(*value)
//     {
//         if(*value >= '0' && *value <= '9')
//             elements.emplace_back(*value);
//         value++;
//     }

//     UINT_32 val = 0;
//     for(UINT_64 rate = 1; rate < CARRY; rate *= 10)
//     {
//         if(elements.empty())
//             break;

//         val += rate * (elements.back() - '0');
//         elements.pop_back();
//     }

//     std::cout << "value: " << val << std::endl;
// }

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
    mIntegerPart = std::move(other.mIntegerPart);
    mDecimalPart = std::move(other.mDecimalPart);
    mIsPositive  = other.mIsPositive;

    return *this;
}

MP& MultiplePrecision::operator+=(const MP& inOther)
{
    MP other(inOther);

    UINT_64 dSize 
        = (mDecimalPart.size() > other.mDecimalPart.size()) 
            ? mDecimalPart.size() : other.mDecimalPart.size();
    UINT_64 iSize
        = (mIntegerPart.size() > other.mIntegerPart.size())
            ? mIntegerPart.size() : other.mIntegerPart.size();

    mDecimalPart.resize(dSize, 0);
    other.mDecimalPart.resize(dSize, 0);
    mIntegerPart.resize(iSize, 0);
    other.mIntegerPart.resize(iSize, 0);

    UINT_64 carry = 0;
    
    // 小数部
    for(UINT_64 i = dSize; i > 0; i--)
    {
        UINT_64 val
            = static_cast<UINT_64>(mDecimalPart.at(i - 1))       +
              static_cast<UINT_64>(other.mDecimalPart.at(i - 1)) +
              carry;
        

        mDecimalPart.at(i - 1) = static_cast<UINT_32>(val);
        carry = val / CARRY;
    }
    // 整数部
    for(UINT_64 i = 0; i < iSize; i++)
    {
        UINT_64 val
            = static_cast<UINT_64>(mIntegerPart.at(i))       +
              static_cast<UINT_64>(other.mIntegerPart.at(i)) +
              carry;

        mIntegerPart.at(i) = static_cast<UINT_32>(val % CARRY);
        carry = val / CARRY;
    }
    mIntegerPart.push_back(carry);

    shrink();
    other.shrink();

    return *this;
}

MP operator+(const MP& lhs, const MP& rhs)
{
    MP mp;

    const auto& maxDecPart
        = (lhs.mDecimalPart.size() >  rhs.mDecimalPart.size())
            ? lhs.mDecimalPart : rhs.mDecimalPart;
    const auto& minDecPart
        = (lhs.mDecimalPart.size() <=  rhs.mDecimalPart.size())
            ? lhs.mDecimalPart : rhs.mDecimalPart;
    const auto& maxIntPart
        = (lhs.mIntegerPart.size() >  rhs.mIntegerPart.size())
            ? lhs.mDecimalPart : rhs.mDecimalPart;
    const auto& minIntPart
        = (lhs.mIntegerPart.size() <=  rhs.mIntegerPart.size())
            ? lhs.mDecimalPart : rhs.mDecimalPart;

    UINT_64 carry = 0;
    
    mp.mDecimalPart.resize(maxDecPart.size(), 0);
    mp.mIntegerPart.resize(maxIntPart.size(), 0);
    
    // 小数部
    for(UINT_64 i = maxDecPart.size();
        i > minDecPart.size();
        i--)
    {
        mp.mDecimalPart.at(i - 1)
            = maxDecPart.at(i - 1);
    }
    for(UINT_64 i = minDecPart.size();
        i > 0;
        i--)
    {
        UINT_64 val
            = static_cast<UINT_64>(maxDecPart.at(i - 1)) +
              static_cast<UINT_64>(minDecPart.at(i - 1)) +
              carry;
        
        mp.mDecimalPart.at(i - 1) 
            = val % MP::CARRY;
        carry 
            = val / MP::CARRY;
    }
    // 整数部
    for(UINT_64 i = 0;
        i < minIntPart.size();
        i++)
    {
        UINT_64 val
            = static_cast<UINT_64>(maxIntPart.at(i)) +
              static_cast<UINT_64>(minDecPart.at(i)) +
              carry;
        
        mp.mIntegerPart.at(i)
            = val % MP::CARRY;
        carry
            = val / MP::CARRY;
    }
    for(UINT_64 i = minIntPart.size();
        i < maxIntPart.size();
        i++)
    {
        UINT_64 val
            = static_cast<UINT_64>(maxIntPart.at(i)) +
              carry;
        
        mp.mIntegerPart.at(i)
            = val % MP::CARRY;
        carry
            = val / MP::CARRY;
    }

    mp.mIntegerPart.push_back(carry);

    mp.shrink();

    return mp;
}

void MultiplePrecision::print(const MP& mp)
{
    for(auto iter = mp.mIntegerPart.rbegin();
        iter != mp.mIntegerPart.rend();
        iter++)
    {
        std::cout << std::setw(8) 
                  << *iter
                  << " ";
        std::cout.fill('0');
    }
    std::cout << std::endl;

    for(auto iter = mp.mDecimalPart.rbegin();
        iter != mp.mDecimalPart.rend();
        iter++)
    {
        std::cout << std::setw(8) 
                  << *iter
                  << " ";
        std::cout.fill('0');
    }
    std::cout << std::endl;
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