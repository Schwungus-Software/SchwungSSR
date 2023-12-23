#include <iostream>

#include "SchwungSSR.hpp"

int main(int, char**) {
    using namespace html;

    const auto page = ::html::html(
        A("lang", "en"),
        head(
            meta(A("charset", "utf-8")),
            meta(
                A("name", "viewport"),
                A("content", "width=device-width, initial-scale=1")
            )
        ),
        body(
            h1("Hello, world!"), p("Damn, what an epic HTML page..."),
            p("I wonder...", " Will it work with multiple text nodes?")
        )
    );

    std::stringstream out;
    page.render(out);

    std::cout << out.rdbuf() << std::endl;

    return 0;
}
