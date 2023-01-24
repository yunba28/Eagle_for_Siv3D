#pragma once

#include <concepts>

namespace eagle::Concepts
{
	/*--------------------------------------------------------------*/
	/*                     Relation Concepts                        */
	/*--------------------------------------------------------------*/

	template<class T, class U>
	concept SameAs = std::is_same_v<T, U>; /* ��̌^�������^���ǂ��� */

	template<class Base,class Derived>
	concept BaseOf = std::is_base_of_v<Base, Derived>; /* Derived��Base���p�����Ă��邩 */

	template<class T, class U>
	concept LineageOf = BaseOf<T, U> or BaseOf<U, T>; /* T��U�̊�ꂠ�邢�͔h���N���X�ł��邩 */

	template<class T, class U>
	concept Convertible = std::is_convertible_v<T, U>; /* T��U�����݂ɕϊ��\�ł��邩 */

	/*--------------------------------------------------------------*/
	/*                       Type Concepts                          */
	/*--------------------------------------------------------------*/

	template<class T>
	concept Integral = std::is_integral_v<T>; /* T�������^�ł��邩 */

	template<class T>
	concept Signed = std::is_signed_v<T>; /* T����������̐����^�ł��邩 */

	template<class T>
	concept Unsigned = std::is_unsigned_v<T>; /* T�������Ȃ��̐����^�ł��邩 */

	template<class T>
	concept FloatingPoint = std::is_floating_point_v<T>; /* T�����������_�^�ł��邩 */

	template<class T>
	concept Arithmetic = std::is_arithmetic_v<T>; /* T���Z�p�^�ł��邩 */

	template<class T>
	concept Scalar = std::is_scalar_v<T>; /* T���X�J���[�^�ł��邩 */

	template<class T>
	concept ArrayAs = std::is_array_v<T>; /* T���z��^�� */

	template<class T>
	concept BoundedArrayAs = std::is_bounded_array_v<T>; /* T���v�f���̔������Ă���z��^�� */

	template<class T>
	concept ScopedEnum = std::is_scoped_enum_v<T>; /* T���X�R�[�v�t���̗񋓌^�ł��邩 */

	/*--------------------------------------------------------------*/
	/*                     Attribute Concepts                       */
	/*--------------------------------------------------------------*/

	template<class T, class ...Args>
	concept Contructible = std::is_constructible_v<T, Args...>; /* �C�ӂ̈����ō\�z�\�� */

	template<class T>
	concept DefaultContructible = std::is_default_constructible_v<T>; /* �g���r���A���\�z���\�� */

	template<class T>
	concept CopyConstructible = std::is_copy_constructible_v<T>; /* �R�s�[�\�z���\�� */

	template<class T>
	concept MoveConstructible = std::is_move_constructible_v<T>; /* ���[�u�\�z���\�� */

	template<class T>
	concept Destructible = std::is_destructible_v<T>; /* �j���\�� */

	template<class To,class From>
	concept Assignable = std::is_assignable_v<To, From>; /* ���蓖�ĉ\�� */

	template<class T>
	concept Swappable = std::is_swappable_v<T>; /* swap���\�� */

	template<class T, class U>
	concept SwappableWith = std::is_swappable_with_v<T, U>; /* �C�ӂ̕���swap���\�� */

	template<class T>
	concept Copyable = std::copyable<T>; /* �R�s�[���\�� */

	template<class T>
	concept TriviallyCopyable = std::is_trivially_copyable_v<T>; /* �g���r�A���ȃR�s�[���\�� */

	template<class T>
	concept Movable = std::movable<T>; /* ���[�u���\�� */

	template<class T>
	concept Semiregular = std::semiregular<T>; /* �������ł��邩 */

	template<class T>
	concept Regular = std::regular<T>; /* �����ł��邩 */

	/*--------------------------------------------------------------*/
	/*                      Function Concepts                       */
	/*--------------------------------------------------------------*/

	template<class F, class ...Args>
	concept Invocable = std::is_invocable_v<F, Args...>; /* F���C�ӂ̈����ŌĂт����\�� */

	template<class F, class ...Args>
	concept Predicate = std::predicate<F, Args...>; /* F���C�ӂ̈����ŌĂт����\����bool��߂�l�Ƃ��ĕԂ��� */

	template<class R, class F, class ...Args>
	concept PredicateWith = std::is_invocable_r_v<R, F, Args...>; /* F���C�ӂ̈����ŌĂт����\���C�ӂ̌^��Ԃ��� */
}