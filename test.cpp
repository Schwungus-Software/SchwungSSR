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
            ),
            style("body { margin: 0 auto; }")
        ),
        body(
            h1(C("hello"), S("color", "red"), "Hello, world!"),
            p(S("color", "green"), S("background", "yellow"),
              S("border", "1px solid black"),
              "Damn, what an epic HTML page..."),
            p(C("multiple"), C("classes"), "I wonder...",
              " Will it work with multiple text nodes?")
        )
    );

    std::stringstream out;
    page.render(out);

    std::cout << out.rdbuf() << std::endl;

    return 0;
}
