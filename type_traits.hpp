#pragma once
#include <es/fe/types.hpp>
#include <es/fe/forward.hpp>
#include <es/fe/var_list.hpp>

#include <es_util/type_traits.hpp>

#include <type_traits>

namespace fe::internal
{
//////////////////////////////////////////////////////////////////////////
// Element tag type by index type, and vice versa

template<class Element_tag>
struct Element_index_by_tag_impl;

template<> struct Element_index_by_tag_impl<Vertex_tag>
{ using Type = Vertex_index; };

template<> struct Element_index_by_tag_impl<Halfedge_tag>
{ using Type = Halfedge_index; };

template<> struct Element_index_by_tag_impl<Edge_tag>
{ using Type = Edge_index; };

template<> struct Element_index_by_tag_impl<Face_tag>
{ using Type = Face_index; };

template<class Element_index>
struct Element_tag_by_index_impl;

template<> struct Element_tag_by_index_impl<Vertex_index>
{ using Type = Vertex_tag; };

template<> struct Element_tag_by_index_impl<Halfedge_index>
{ using Type = Halfedge_tag; };

template<> struct Element_tag_by_index_impl<Edge_index>
{ using Type = Edge_tag; };

template<> struct Element_tag_by_index_impl<Face_index>
{ using Type = Face_tag; };

template<class Element_tag>
using Element_index_by_tag = typename Element_index_by_tag_impl<Element_tag>::Type;

template<class Element_tag>
using Element_tag_by_index = typename Element_tag_by_index_impl<Element_tag>::Type;

//////////////////////////////////////////////////////////////////////////
}

namespace fe
{
	template<std::size_t var>
using Var_index = es_util::Index<var>;

template<class Var_list, class Var_index>
struct Var_by_var_index_impl;

template<class Var_list, std::size_t index>
struct Var_by_var_index_impl<Var_list, Var_index<index>>
{
	using Type = typename Var_list::template Var<index>;
};

template<class Var_list, class Var_index>
using Var_by_var_index = typename Var_by_var_index_impl<Var_list, es_util::Remove_cvref<Var_index>>::Type;

}

namespace fe::internal
{
template<class T>
struct Is_var_list : std::false_type { };

template<class... Vars>
struct Is_var_list<Var_list<Vars...>> : std::true_type { };

template<class Var_or_var_list>
using Wrap_into_var_list_t = std::conditional_t<
	Is_var_list<Var_or_var_list>::value, Var_or_var_list, Var_list<Var_or_var_list>>;

template<class Var_list>
struct Vars_traits;

template<class Var, class... Vars>
struct Vars_traits<Var_list<Var, Vars...>>
{
	// TODO : enforce_same_dim
	static_assert(es_util::all_same<Var::space_dim, Vars::space_dim...>,
		"Incompatible dimensions of variables");

	static constexpr auto n_vars = 1 + sizeof...(Vars);
	static constexpr auto space_dim = Var::space_dim;
};

template<class System>
struct System_traits;

template<class T_Var_list, template<class> class T_Dof_mapper>
struct System_traits<System<T_Var_list, T_Dof_mapper>>
{
	using Var_list = T_Var_list;
};

template<class Tag>
struct Is_vertex_tag : std::is_same<Tag, Vertex_tag> { };

template<class Tag>
constexpr bool is_vertex_tag_v = Is_vertex_tag<Tag>::value;

template<class Tag>
struct Is_edge_tag : std::is_same<Tag, Edge_tag> { };

template<class Tag>
constexpr bool is_edge_tag_v = Is_edge_tag<Tag>::value;

template<class Element_index>
struct Decay_element_index_impl
{
	using Type = Element_index;
};

template<>
struct Decay_element_index_impl<Edge_index>
{
	using Type = Halfedge_index;
};

// For Edge_index returns Halfedge_index,
// for other index types returns them intact
template<class Element_index>
using Decay_element_index =
typename Decay_element_index_impl<Element_index>::Type;

//////////////////////////////////////////////////////////////////////////

template<class Element_tag, class Circ_tag>
struct Circulator_type_selector;

template<class Circ_tag>
struct Circulator_type_selector<Vertex_tag, Circ_tag>
{
	using Type = Circulator_vertex_face<Vertex_tag, Circ_tag>;
};

template<class Circ_tag>
struct Circulator_type_selector<Face_tag, Circ_tag>
{
	using Type = Circulator_vertex_face<Face_tag, Circ_tag>;
};

template<class Circ_tag>
struct Circulator_type_selector<Halfedge_tag, Circ_tag>
{
	using Type = Circulator_halfedge_edge<Halfedge_tag, Circ_tag>;
};

template<class Circ_tag>
struct Circulator_type_selector<Edge_tag, Circ_tag>
{
	using Type = Circulator_halfedge_edge<Edge_tag, Circ_tag>;
};
}

namespace fe
{
template<class Element_tag, class Circ_tag>
using Circulator = typename internal::Circulator_type_selector<Element_tag, Circ_tag>::Type;
}