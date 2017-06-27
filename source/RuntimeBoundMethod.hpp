//
//  RuntimeBoundMethod.hpp
//

#ifndef RuntimeBoundMethod_h
#define RuntimeBoundMethod_h

#include <functional>
#include <type_traits>
#include <stdexcept>

namespace detail
{

template<typename ObjType, typename R, typename... Args>
class ConstRuntimeBoundMethodImpl {
    
public:
    typedef ObjType Type;
    
    explicit ConstRuntimeBoundMethodImpl(ObjType *pThis)
    : pThis_(pThis)
    {
        if (!pThis) {
            throw std::runtime_error("Null this pointer");
        }
    }
    
    ConstRuntimeBoundMethodImpl(const ConstRuntimeBoundMethodImpl&) = default;
    ConstRuntimeBoundMethodImpl& operator=(const ConstRuntimeBoundMethodImpl&) = default;
    ConstRuntimeBoundMethodImpl(ConstRuntimeBoundMethodImpl&&) = default;
    ConstRuntimeBoundMethodImpl& operator=(ConstRuntimeBoundMethodImpl&&) = default;
    
    template<typename Callable>
    ConstRuntimeBoundMethodImpl& operator=(Callable&& f)
    {
        f_ = std::forward<Callable>(f);
        return *this;
    }
    
    template<typename... X>
    R operator()(X&&... args) const
    {
        if (!isBound()) {
            throw std::runtime_error("Unbound method");
        }
        
        return f_(*pThis_, std::forward<X>(args)...);
    }
    
    bool isBound() const
    {
        return !!f_;
    }
    
private:
    ObjType *pThis_;
    std::function<R(ObjType&, Args...)> f_;
};


template<typename ObjType, typename R, typename... Args>
class RuntimeBoundMethodImpl {

public:
    typedef ObjType Type;
    
    explicit RuntimeBoundMethodImpl(ObjType *pThis)
    : pThis_(pThis)
    {
        if (!pThis) {
            throw std::runtime_error("Null this pointer");
        }
    }
    
    RuntimeBoundMethodImpl(const RuntimeBoundMethodImpl&) = default;
    RuntimeBoundMethodImpl& operator=(const RuntimeBoundMethodImpl&) = default;
    RuntimeBoundMethodImpl(RuntimeBoundMethodImpl&&) = default;
    RuntimeBoundMethodImpl& operator=(RuntimeBoundMethodImpl&&) = default;
    
    template<typename Callable>
    RuntimeBoundMethodImpl& operator=(Callable&& f)
    {
        f_ = std::forward<Callable>(f);
        return *this;
    }
    
    template<typename... X>
    R operator()(X&&... args) const
    {
        if (!isBound()) {
            throw std::runtime_error("Unbound method");
        }
        
        return f_(*pThis_, std::forward<X>(args)...);
    }
    
    template<typename... X>
    R operator()(X&&... args)
    {
        if (!isBound()) {
            throw std::runtime_error("Unbound method");
        }
        
        return f_(*pThis_, std::forward<X>(args)...);
    }
    
    bool isBound() const
    {
        return !!f_;
    }
    
private:
    ObjType *pThis_;
    std::function<R(ObjType&, Args...)> f_;
};
    
} // namespace detail


template<typename ObjType, typename R, typename... Args>
class RuntimeBoundMethod : public std::conditional<std::is_const<ObjType>::value,
                                            detail::ConstRuntimeBoundMethodImpl<ObjType, R, Args...>,
                                            detail::RuntimeBoundMethodImpl<ObjType, R, Args...> >::type
{
    typedef typename std::conditional<std::is_const<ObjType>::value,
                        detail::ConstRuntimeBoundMethodImpl<ObjType, R, Args...>,
                        detail::RuntimeBoundMethodImpl<ObjType, R, Args...> >::type BaseType;
    
public:
    using BaseType::operator=;
    using BaseType::operator();
    using BaseType::isBound;
    
    explicit RuntimeBoundMethod(ObjType *pThis)
    : BaseType(pThis)
    {
        
    }
};

#endif /* RuntimeBoundMethod_h */
