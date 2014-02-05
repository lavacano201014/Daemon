/*
===========================================================================
Daemon BSD Source Code
Copyright (c) 2013-2014, Daemon Developers
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
===========================================================================
*/

#ifndef COMMON_UTIL_H_
#define COMMON_UTIL_H_

// Various utility functions

#include "../engine/qcommon/q_shared.h"
#include <utility>
#include <type_traits>

namespace Util {

// Binary search function which returns an iterator to the result or end if not found
template<typename Iter, typename T>
Iter binary_find(Iter begin, Iter end, const T& value)
{
	Iter i = std::lower_bound(begin, end, value);
	if (i != end && !(value < *i))
		return i;
	else
		return end;
}
template<typename Iter, typename T, typename Compare>
Iter binary_find(Iter begin, Iter end, const T& value, Compare comp)
{
	Iter i = std::lower_bound(begin, end, value, comp);
	if (i != end && !comp(value, *i))
		return i;
	else
		return end;
}

// Compile-time integer sequences
template<size_t...> struct seq {
	typedef seq type;
};

template<class S1, class S2> struct concat;
template<size_t... I1, size_t... I2> struct concat<seq<I1...>, seq<I2...>>: seq<I1..., (sizeof...(I1) + I2)...> {};

template<size_t N> struct gen_seq;
template<size_t N> struct gen_seq: concat<typename gen_seq<N / 2>::type, typename gen_seq<N - N / 2>::type>::type {};
template<> struct gen_seq<0>: seq<>{};
template<> struct gen_seq<1>: seq<0>{};

// Invoke a function using parameters from a tuple
template<typename Func, typename Tuple, size_t... Seq>
decltype(std::declval<Func>()(std::get<Seq>(std::declval<Tuple>())...)) apply_impl(Func&& func, Tuple&& tuple, seq<Seq...>)
{
	return std::forward<Func>(func)(std::get<Seq>(std::forward<Tuple>(tuple))...);
}
template<typename Func, typename Tuple>
decltype(apply_impl(std::declval<Func>(), std::declval<Tuple>(), gen_seq<std::tuple_size<typename std::decay<Tuple>::type>::value>())) apply(Func&& func, Tuple&& tuple)
{
	return apply_impl(std::forward<Func>(func), std::forward<Tuple>(tuple), gen_seq<std::tuple_size<typename std::decay<Tuple>::type>::value>());
}

} // namespace Util

#endif // COMMON_UTIL_H_
