#ifndef _FUNCTION_H
#define _FUNCTION_H
#include <complex>
#include <functional>
#include <type_traits>

#include <memory> // for std::unique_ptr
#include <utility> // for std::in_place, std::forward
#include <typeinfo> // for std::type_info


template <class Func, class ...Args>
struct Function
{
	static_assert(!std::is_same_v<Func, Func>, "请提供正确的函数使用方法");
};


//// 仅仅接受函数指针的方式吧
//template <class Func, class ...Args>
//struct Function<Func(Args...)>
//{
//private:
//	Func(*func)(Args...args);
//
//public:
//	Function(Func(*f)(Args...args)) : func(f) {}
//	Func operator()(Args...args) {
//		return func(std::forward<Args>(args)...);
//	}
//};

// lambda函数在不进行任何捕获操纵的情况下允许隐式转换到函数指针
// 但是一旦捕获这个Function就不能正常使用
// 因此要使用类型擦除，利用虚函数来实现

template <class Func, class ...Args>
class Function<Func(Args...)>
{
private:
	// 对于不同类型的参数传进来都能正常使用
	// 可能是lambda，可能是函数指针

	struct FuncBase
	{
		virtual Func _M_call(Args ...args) = 0; // 类型擦除后的统一接口
		virtual std::unique_ptr<FuncBase> _M_clone() const = 0; // 原型模式，克隆当前函数对象
		virtual std::type_info const& _M_type() const = 0; // 获得函数对象类型信息
		virtual ~FuncBase() = default; // 应对_Fn可能有非平凡析构的情况
	};

	template <class F>
	struct FuncImpl : public FuncBase
	{
		F _f;

		template <class ...CArgs>
		// 不知道这几把构造要干嘛
		explicit FuncImpl(std::in_place_t, CArgs&& ...args) : _f(std::forward<CArgs>(args)...) {}

		Func _M_call(Args ...args) override
		{
			return std::invoke(_f, std::forward<Args>(args)...);
		}

		std::unique_ptr<FuncBase> _M_clone() const override
		{
			return std::make_unique<FuncImpl>(std::in_place, _f);
		}

		const std::type_info& _M_type() const override
		{
			return typeid(_f);
		}
	};

	std::unique_ptr<FuncBase> _M_func; // 指向类型擦除后的函数对象
public:

	Function() = default;

	// 此处 enable_if_t 的作用：阻止 Function 从不可调用的对象中初始化
	// 另外标准要求 Function 还需要函数对象额外支持拷贝（用于 _M_clone）
	template <class _Fn, class = std::enable_if_t<std::is_invocable_r_v<Func, std::decay_t<_Fn>, Args...>
	&& std::is_copy_constructible_v<_Fn>
		&& !std::is_same_v<std::decay_t<_Fn>, Function<Func(Args...)>> >>
		Function(_Fn&& __f) // 没有 explicit，允许 lambda 表达式隐式转换成 Function
		: _M_func(std::make_unique<FuncImpl<std::decay_t<_Fn>>>(std::in_place, std::forward<_Fn>(__f)))
	{}

	Function(Function& other) : _M_func(other._M_func ? other._M_func._M_clone() : nullptr) {}
	
	Function& operator=(Function const& __that) {
		if (__that._M_func)
			_M_func = __that._M_func->_M_clone();
		else
			_M_func = nullptr;
	}

	explicit operator bool() const noexcept {
		return _M_func != nullptr;
	}

	bool operator==(std::nullptr_t) const noexcept {
		return _M_func == nullptr;
	}

	bool operator!=(std::nullptr_t) const noexcept {
		return _M_func != nullptr;
	}
	

	Func operator()(Args ...__args) const {
		if (!_M_func) [[unlikely]]
			throw std::bad_function_call();
		// 完美转发所有参数，这样即使 Args 中具有引用，也能不产生额外的拷贝开销
		return _M_func->_M_call(std::forward<Args>(__args)...);
	}


};



#endif
