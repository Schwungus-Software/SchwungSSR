#pragma once

#include <exception>
#include <map>
#include <sstream>
#include <string>
#include <vector>

struct SSR_exception : std::exception {
    SSR_exception(const std::string& reason) : std::exception(reason.c_str()) {}
};

struct SSR;

template <typename T>
concept SSR_arg_external =
    requires(const T& obj, SSR& ssr) { install_ssr_arg(ssr, obj); };

template <typename T>
concept SSR_arg_internal =
    requires(const T& obj, SSR& ssr) { obj.install(ssr); };

template <typename T>
concept SSR_arg = SSR_arg_external<T> || SSR_arg_internal<T>;

struct SSR {
    std::string elt_name;
    std::map<std::string, std::string> attributes;

    std::vector<SSR> children;
    std::string text_content;

    template <SSR_arg... Args>
    SSR(Args... args) {
        (*this)(args...);
    }

    template <SSR_arg... Args>
    SSR& operator()(Args... args) {
        (install_ssr_arg(*this, args), ...);
        return *this;
    }

    void install(SSR&) const;

    void render(std::stringstream&) const;
};

void install_ssr_arg(SSR&, const std::string&);

template <SSR_arg_internal T>
void install_ssr_arg(SSR& ssr, const T& input) {
    input.install(ssr);
}

struct Elt {
    const std::string name;

    Elt(const std::string& name) : name(name) {}

    void install(SSR&) const;
};

struct A {
    const std::string attr, value;

    A(const std::string& attr, const std::string& value)
        : attr(attr), value(value) {}

    void install(SSR&) const;
};

#define ELT(name)                                                              \
    struct name : public SSR {                                                 \
        template <SSR_arg... Args>                                             \
        name(Args... args) : SSR(args..., Elt(#name)) {}                       \
    }

namespace html {
    ELT(html);
    ELT(head);
    ELT(body);
    ELT(meta);
    ELT(style);
    ELT(script);

    ELT(div);
    ELT(span);

    ELT(h1);
    ELT(h2);
    ELT(h3);
    ELT(h4);

    ELT(p);

    ELT(img);

    ELT(button);
    ELT(input);
}; // namespace html

#undef ELT
