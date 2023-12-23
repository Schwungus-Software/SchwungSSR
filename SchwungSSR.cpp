#include "SchwungSSR.hpp"

void install_ssr_arg(SSR& ssr, const std::string& text) {
    auto child = SSR();
    child.text_content = text;
    ssr.children.push_back(child);
}

void SSR::install(SSR& ssr) const {
    ssr.children.push_back(*this);
}

void Elt::install(SSR& ssr) const {
    ssr.elt_name = name;
}

void A::install(SSR& ssr) const {
    ssr.attributes.insert_or_assign(attr, value);
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

void SSR::render(std::stringstream& out) const {
    if (elt_name.empty()) {
        escape(text_content, out);
        return;
    }

    out << "<";
    escape(elt_name, out);

    for (const auto& pair : attributes) {
        out << " ";

        escape(pair.first, out);
        out << "=";

        out << "\"";
        escape(pair.second, out);
        out << "\"";
    }

    out << ">";

    for (const auto& child : children) {
        child.render(out);
    }

    out << "</";
    escape(elt_name, out);
    out << ">";
}
