#pragma once

#include <exception>
#include <map>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

struct SSR_exception : std::exception {
    SSR_exception(const std::string& reason) : std::exception(reason.c_str()) {}
};

struct SSR;

template <typename T>
concept SSR_arg_external =
    requires(const T& arg, SSR& target) { install_ssr_arg(target, arg); };

template <typename T>
concept SSR_arg_internal =
    requires(const T& arg, SSR& target) { arg.install(target); };

template <typename T>
concept SSR_arg = SSR_arg_external<T> || SSR_arg_internal<T>;

struct SSR {
    struct HtmlNode {
        std::string elt_name;
        std::map<std::string, std::string> attributes;
        std::vector<SSR> children;
    };

    struct TextNode {
        std::string contents;
    };

    using AnyNode = std::variant<HtmlNode, TextNode>;

    AnyNode node;

    template <typename... Args>
    SSR(Args... args) {
        (*this)(args...);
    }

    template <SSR_arg... Args>
    SSR& operator()(Args... args) {
        (install_ssr_arg(*this, args), ...);
        return *this;
    }

    void install(SSR&) const;

    void render(std::stringstream&, bool = true) const;
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

struct S {
    const std::string attr, value;

    S(const std::string& attr, const std::string& value)
        : attr(attr), value(value) {}

    void install(SSR&) const;
};

struct C {
    const std::string clazz;

    C(const std::string& clazz) : clazz(clazz) {}

    void install(SSR&) const;
};

struct Href : public A {
    Href(const std::string& url) : A("href", url) {}
};

#define ELT(name)                                                              \
    struct name : public SSR {                                                 \
        template <typename... Args>                                            \
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

    ELT(nav);
    ELT(sidebar);
    ELT(main);
    ELT(header);
    ELT(footer);

    ELT(h1);
    ELT(h2);
    ELT(h3);
    ELT(h4);

    ELT(p);

    ELT(strong);
    ELT(i);

    ELT(a);
    ELT(img);
    ELT(svg);

    ELT(button);
    ELT(input);
}; // namespace html

#undef ELT
