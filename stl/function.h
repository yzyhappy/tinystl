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
	static_assert(!std::is_same_v<Func, Func>, "���ṩ��ȷ�ĺ���ʹ�÷���");
};


//// �������ܺ���ָ��ķ�ʽ��
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

// lambda�����ڲ������κβ�����ݵ������������ʽת��������ָ��
// ����һ���������Function�Ͳ�������ʹ��
// ���Ҫʹ�����Ͳ����������麯����ʵ��

template <class Func, class ...Args>
class Function<Func(Args...)>
{
private:
	// ���ڲ�ͬ���͵Ĳ�����������������ʹ��
	// ������lambda�������Ǻ���ָ��

	struct FuncBase
	{
		virtual Func _M_call(Args ...args) = 0; // ���Ͳ������ͳһ�ӿ�
		virtual std::unique_ptr<FuncBase> _M_clone() const = 0; // ԭ��ģʽ����¡��ǰ��������
		virtual std::type_info const& _M_type() const = 0; // ��ú�������������Ϣ
		virtual ~FuncBase() = default; // Ӧ��_Fn�����з�ƽ�����������
	};

	template <class F>
	struct FuncImpl : public FuncBase
	{
		F _f;

		template <class ...CArgs>
		// ��֪���⼸�ѹ���Ҫ����
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

	std::unique_ptr<FuncBase> _M_func; // ָ�����Ͳ�����ĺ�������
public:

	Function() = default;

	// �˴� enable_if_t �����ã���ֹ Function �Ӳ��ɵ��õĶ����г�ʼ��
	// �����׼Ҫ�� Function ����Ҫ�����������֧�ֿ��������� _M_clone��
	template <class _Fn, class = std::enable_if_t<std::is_invocable_r_v<Func, std::decay_t<_Fn>, Args...>
	&& std::is_copy_constructible_v<_Fn>
		&& !std::is_same_v<std::decay_t<_Fn>, Function<Func(Args...)>> >>
		Function(_Fn&& __f) // û�� explicit������ lambda ���ʽ��ʽת���� Function
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
		// ����ת�����в�����������ʹ Args �о������ã�Ҳ�ܲ���������Ŀ�������
		return _M_func->_M_call(std::forward<Args>(__args)...);
	}


};



#endif
