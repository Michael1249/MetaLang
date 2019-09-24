#ifndef METALANG_H
#define METALANG_H
#include <typeinfo>
namespace mtl
{

template<class ...Items>
struct pack;
using None = pack<>;

template<class Pack>
struct function;

template<bool Value>
struct if_ {};

template<unsigned N, class Pack>
struct get_;

template<class Function, class Pack>
struct apply_;

//################################################################
// pack
template<class ...Items>
struct pack
{

	static const unsigned size = sizeof...(Items);

	template<template<class ...>class OtherPack>
	struct fill_
	{
		using type = OtherPack<Items...>;
	};

	template<class ...NewItems>
	struct insert_items_on_right_ : pack<Items..., NewItems...>
	{
		using type = pack<Items..., NewItems...>;
	};

	template<class ...NewItems>
	struct insert_items_on_left_ : pack<NewItems..., Items...>
	{
		using type = pack<NewItems..., Items...>;
	};

	template<class OtherPack>
	struct insert_pack_on_right_ : OtherPack::template insert_items_on_left_<Items...>
	{
		using type = typename OtherPack::template insert_items_on_left_<Items...>::type;
	};

	template<class OtherPack>
	struct insert_pack_on_left_ : OtherPack::template insert_items_on_right_<Items...>
	{
		using type = typename OtherPack::template insert_items_on_right_<Items...>::type;
	};

	template<template<class ...> class Function, class Result, class Iterator, class ...OtherIterators>
	struct __apply__
	{
		using type = typename __apply__<
			Function,
			typename Result::template insert_items_on_right_<Function<Iterator>>::type,
			OtherIterators...>
			::type;
	};

	template<template<class ...> class Function, class Result, class Iterator>
	struct __apply__<Function, Result, Iterator>
	{
		using type = typename Result::template insert_items_on_right_<Function<Iterator>>::type;
	};

	template<template<class ...> class Function>
	struct apply_ : if_<size>::template than_<typename __apply__<Function, None, Items...>::type>::template else_<None>::type
	{
		using type = typename if_<size>::template than_<typename __apply__<Function, None, Items...>::type>::template else_<None>::type;
	};

	template<class DeletedItem, class ...OthersItems>
	struct __remove_first__
	{
		using type = pack<OthersItems...>;
	};

	struct remove_first_
	{
		using type = __remove_first__<Items...>;
	};

	template<unsigned N, class Item, class ...OtherItems>
	struct __get__
	{
		using type = typename __get__<N - 1U, OtherItems...>::type;
	};

	template<class Item, class ...OtherItems>
	struct __get__<0U, Item, OtherItems...>
	{
		using type = Item;
	};

	template<unsigned N>
	struct get_
	{
		using type = typename __get__<N, Items...>::type;
	};

};


//################################################################
// if_ :: than_ :: else_
template<>
struct if_<true>
{
	template<class Variant_1>
	struct than_
	{
		template<class Variant_2>
		struct else_
		{
			using type = Variant_1;
		};

                template<bool Value>
                struct elif_
                {
                        template<class Variant_2>
                        struct than_
                        {
                                template<class Variant_3>
                                struct else_
                                {
                                        using type = Variant_1;
                                };
                                template<bool Value_2>
                                using elif_ = elif_<Value_2>;
                        };

                };
	};
};

template<>
struct if_<false>
{
	template<class Variant_1>
	struct than_
	{
		template<class Variant_2>
		struct else_
		{
			using type = Variant_2;
		};

        template<bool Value>
        using elif_ = if_<Value>;

	};
};

//################################################################
// get
template<unsigned N, class Pack>
struct get_
{
	using type = typename Pack::template get_<N>::type;
};

//################################################################
// apply for each
template<class Function, class Pack>
struct apply_
{
	using type = typename Pack::template apply_<Function>::type;
    };

//################################################################


}

#endif // METALANG_H
