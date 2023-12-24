#include <format>

#include "SchwungSSR.hpp"

static SSR::HtmlNode& expect_html(SSR& ssr) {
    if (std::holds_alternative<SSR::HtmlNode>(ssr.node)) {
        return std::get<SSR::HtmlNode>(ssr.node);
    } else {
        throw SSR_exception("Expected an HTML node, not a text node");
    }
}

void install_ssr_arg(SSR& target, const std::string& text) {
    auto child = SSR();
    child.node = SSR::TextNode(text);

    auto& node = expect_html(target);
    node.children.push_back(child);
}

void SSR::install(SSR& target) const {
    auto& node = expect_html(target);
    node.children.push_back(*this);
}

void Elt::install(SSR& target) const {
    auto& node = expect_html(target);
    node.elt_name = name;
}

void A::install(SSR& target) const {
    auto& node = expect_html(target);
    node.attributes.insert_or_assign(attr, value);
}

void S::install(SSR& target) const {
    auto& node = expect_html(target);

    const auto line = std::format("{}:{};", attr, value);

    if (node.attributes.contains("style")) {
        auto& style = node.attributes.at("style");
        style += line;
    } else {
        node.attributes.insert_or_assign("style", line);
    }
}

void C::install(SSR& target) const {
    auto& node = expect_html(target);

    if (node.attributes.contains("class")) {
        auto& style = node.attributes.at("class");
        style += " " + clazz;
    } else {
        node.attributes.insert_or_assign("class", clazz);
    }
}

static void escape_html(const std::string& input, std::stringstream& out) {
    for (const auto c : input) {
        switch (c) {
            case '<':
                out << "&lt;";
                break;
            case '>':
                out << "&gt;";
                break;
            case '"':
                out << "&quot;";
                break;
            default:
                out << c;
                break;
        }
    }
}

static void escape_quote(const std::string& input, std::stringstream& out) {
    for (const auto c : input) {
        if (c == '"') {
            out << "\\\"";
        } else {
            out << c;
        }
    }
}

void SSR::render(std::stringstream& out, bool root) const {
    if (std::holds_alternative<TextNode>(node)) {
        const auto& text_node = std::get<TextNode>(node);
        escape_html(text_node.contents, out);
        return;
    }

    if (root) {
        out << "<!DOCTYPE html>";
    }

    const auto& html_node = std::get<HtmlNode>(node);

    out << "<";
    escape_html(html_node.elt_name, out);

    for (const auto& pair : html_node.attributes) {
        out << " ";

        escape_html(pair.first, out);
        out << "=";

        out << "\"";
        escape_quote(pair.second, out);
        out << "\"";
    }

    out << ">";

    for (const auto& child : html_node.children) {
        child.render(out, false);
    }

    out << "</";
    escape_html(html_node.elt_name, out);
    out << ">";
}
