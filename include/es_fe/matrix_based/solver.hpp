#pragma once
#include <es_fe/matrix_based/solver_base.hpp>
#include <es_fe/matrix_based/solution_view.hpp>

#include <es_la/dense.hpp>

#include <cstddef>
#include <memory>

namespace es_fe
{
template<class System_, class Linear_solver_>
class Matrix_based_solver : public Matrix_based_solver_base<System_, Linear_solver_>
{
private:
	using Base = Matrix_based_solver_base<System_, Linear_solver_>;

public:
	using Base::Base;

	template<class... Args>
	void init(Args&&... args)
	{
		Base::init(std::forward<Args>(args)...);
	}

	void solve()
	{
		matrix_.zero();
		rhs_ = 0;

		before_solve();

		set_bnd_values();
		assemble();
		after_assemble();

		linear_solver_.analyze_factorize_solve(rhs_, solution_);

		after_solve();
	}

protected:
	virtual void set_bnd_values() = 0;

	virtual void before_solve()
	{}

	virtual void after_solve()
	{}

	virtual void after_assemble()
	{}

	virtual void assemble() = 0;

protected:
	using Base::solution_;
	using Base::rhs_;
	using Base::matrix_;
	using Base::linear_solver_;
};
} // namespace es_fe
