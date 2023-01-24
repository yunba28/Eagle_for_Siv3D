#pragma once

#include <concepts>

namespace eagle::Concepts
{
	/*--------------------------------------------------------------*/
	/*                     Relation Concepts                        */
	/*--------------------------------------------------------------*/

	template<class T, class U>
	concept SameAs = std::is_same_v<T, U>; /* 二つの型が同じ型かどうか */

	template<class Base,class Derived>
	concept BaseOf = std::is_base_of_v<Base, Derived>; /* DerivedがBaseを継承しているか */

	template<class T, class U>
	concept LineageOf = BaseOf<T, U> or BaseOf<U, T>; /* TがUの基底あるいは派生クラスであるか */

	template<class T, class U>
	concept Convertible = std::is_convertible_v<T, U>; /* TとUが相互に変換可能であるか */

	/*--------------------------------------------------------------*/
	/*                       Type Concepts                          */
	/*--------------------------------------------------------------*/

	template<class T>
	concept Integral = std::is_integral_v<T>; /* Tが整数型であるか */

	template<class T>
	concept Signed = std::is_signed_v<T>; /* Tが符号ありの整数型であるか */

	template<class T>
	concept Unsigned = std::is_unsigned_v<T>; /* Tが符号なしの整数型であるか */

	template<class T>
	concept FloatingPoint = std::is_floating_point_v<T>; /* Tが浮動小数点型であるか */

	template<class T>
	concept Arithmetic = std::is_arithmetic_v<T>; /* Tが算術型であるか */

	template<class T>
	concept Scalar = std::is_scalar_v<T>; /* Tがスカラー型であるか */

	template<class T>
	concept ArrayAs = std::is_array_v<T>; /* Tが配列型か */

	template<class T>
	concept BoundedArrayAs = std::is_bounded_array_v<T>; /* Tが要素数の判明している配列型か */

	template<class T>
	concept ScopedEnum = std::is_scoped_enum_v<T>; /* Tがスコープ付きの列挙型であるか */

	/*--------------------------------------------------------------*/
	/*                     Attribute Concepts                       */
	/*--------------------------------------------------------------*/

	template<class T, class ...Args>
	concept Contructible = std::is_constructible_v<T, Args...>; /* 任意の引数で構築可能か */

	template<class T>
	concept DefaultContructible = std::is_default_constructible_v<T>; /* トリビュアル構築が可能か */

	template<class T>
	concept CopyConstructible = std::is_copy_constructible_v<T>; /* コピー構築が可能か */

	template<class T>
	concept MoveConstructible = std::is_move_constructible_v<T>; /* ムーブ構築が可能か */

	template<class T>
	concept Destructible = std::is_destructible_v<T>; /* 破棄可能か */

	template<class To,class From>
	concept Assignable = std::is_assignable_v<To, From>; /* 割り当て可能か */

	template<class T>
	concept Swappable = std::is_swappable_v<T>; /* swapが可能か */

	template<class T, class U>
	concept SwappableWith = std::is_swappable_with_v<T, U>; /* 任意の方とswapが可能か */

	template<class T>
	concept Copyable = std::copyable<T>; /* コピーが可能か */

	template<class T>
	concept TriviallyCopyable = std::is_trivially_copyable_v<T>; /* トリビアルなコピーが可能か */

	template<class T>
	concept Movable = std::movable<T>; /* ムーブが可能か */

	template<class T>
	concept Semiregular = std::semiregular<T>; /* 半正則であるか */

	template<class T>
	concept Regular = std::regular<T>; /* 正則であるか */

	/*--------------------------------------------------------------*/
	/*                      Function Concepts                       */
	/*--------------------------------------------------------------*/

	template<class F, class ...Args>
	concept Invocable = std::is_invocable_v<F, Args...>; /* Fが任意の引数で呼びだし可能か */

	template<class F, class ...Args>
	concept Predicate = std::predicate<F, Args...>; /* Fが任意の引数で呼びだし可能かつboolを戻り値として返すか */

	template<class R, class F, class ...Args>
	concept PredicateWith = std::is_invocable_r_v<R, F, Args...>; /* Fが任意の引数で呼びだし可能かつ任意の型を返すか */
}