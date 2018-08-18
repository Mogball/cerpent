#pragma once

template<typename...> struct types {
    using type = types;
};

template<typename Sig> struct args;

template<typename Ret, typename... Args>
struct args<Ret(Args...)> : types<Args...> {};

template<typename Sig>
using args_t = typename args<Sig>::type;
