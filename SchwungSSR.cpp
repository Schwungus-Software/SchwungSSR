#include "SchwungSSR.hpp"
#include <variant>

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

static void escape(const std::string& input, std::stringstream& out) {
    for (auto c : input) {
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

void SSR::render(std::stringstream& out, bool root) const {
    if (std::holds_alternative<TextNode>(node)) {
        const auto& text_node = std::get<TextNode>(node);
        escape(text_node.contents, out);
        return;
    }

    if (root) {
        out << "<!DOCTYPE html>";
    }

    const auto& html_node = std::get<HtmlNode>(node);

    out << "<";
    escape(html_node.elt_name, out);

    for (const auto& pair : html_node.attributes) {
        out << " ";

        escape(pair.first, out);
        out << "=";

        out << "\"";
        escape(pair.second, out);
        out << "\"";
    }

    out << ">";

    for (const auto& child : html_node.children) {
        child.render(out, false);
    }

    out << "</";
    escape(html_node.elt_name, out);
    out << ">";
}
