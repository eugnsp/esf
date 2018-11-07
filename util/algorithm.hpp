#pragma once
#include <es/fe/types.hpp>
#include <cstddef>
#include <type_traits>
#include <utility>

namespace fe
{
namespace internal
{
template<std::size_t... vars, class Function>
void for_each_var_impl(std::index_sequence<vars...>, Function func)
{
	(static_cast<void>(func(Var_index<vars>{})), ...);
}

template<class Var_list, std::size_t var, class Element_tag, class Function>
void for_each_var_element_impl(Function func)
{
	if constexpr (Var_list::template Var<var>::Element::has_dofs(Element_tag{}))
		func(Var_index<var>{}, Element_tag{});
}

template<class Var_list, std::size_t var, class... Element_tags, class Function>
void for_each_var_element_impl2(Function func)
{
	(for_each_var_element_impl<Var_list, var, Element_tags>(func), ...);

	if constexpr (var + 1 < Var_list::size)
		for_each_var_element_impl2<Var_list, var + 1, Element_tags...>(func);
}
}

template<class Var_list, class Function>
void for_each_var(Function func)
{
	internal::for_each_var_impl(std::make_index_sequence<Var_list::size>{}, func);
}

template<class Var_list, class Function>
void for_each_var_element(Function func)
{
	constexpr auto space_dim = internal::Vars_traits<Var_list>::space_dim;
	static_assert(space_dim == 1 || space_dim == 2, "Bad dimension");

	if constexpr (space_dim == 1)
		internal::for_each_var_element_impl2<Var_list, 0, Vertex_tag, Edge_tag>(func);

	if constexpr (space_dim == 2)
		internal::for_each_var_element_impl2<Var_list, 0, Vertex_tag, Edge_tag, Face_tag>(func);
}
}

//namespace fe
//{
//namespace internal
//{
//template<std::size_t... vars, class Function>
//void for_each_var_impl(std::index_sequence<vars...>, Function func)
//{
//	(static_cast<void>(func(util::Index_t<vars>{})), ...);
//}
//
//template<class System, std::size_t var, class Tag, class Function>
//void for_each_var_tag_impl(Function func)
//{
//	if constexpr (System::template Var_t<var>::has_dof(Tag{}))
//		func(util::Index_t<var>{}, Tag{});
//}
//
//template<class System, std::size_t var, class... Tags, class Function>
//void for_each_var_tag_impl2(Function func)
//{
//	(for_each_var_tag_impl<System, var, Tags>(func), ...);
//
//	if constexpr (var + 1 < System::n_vars)
//		for_each_var_tag_impl2<System, var + 1, Tags...>(func);
//}
//}
//
//template<class System, class Function>
//void for_each_var(Function func)
//{
//	internal::for_each_var_impl(std::make_index_sequence<System::n_vars>{}, func);
//}
//
//template<class System, class Function>
//void for_each_var_tag(Function func)
//{
//	if constexpr (System::dim == 1)
//		internal::for_each_var_tag_impl2<System, 0, Vertex_tag, Edge_tag>(func);
//
//	if constexpr (System::dim == 2)
//		internal::for_each_var_tag_impl2<System, 0, Vertex_tag, Edge_tag, Face_tag>(func);
//}
//}