/* LEON::vector 用の ランダムアクセスイテレータ
* 
* <algorithm> などのライブラリで ::std::iterator_traits が
* 正常に動作するように
* ::std::random_access_iterator_tag をつけた
*/

#pragma once

#include <iterator> // ::std::random_access_iterator_tag

namespace LEON
{

template<typename Container>
class ReverseIterator
{
public:
    

};

};