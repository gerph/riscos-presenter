# RISC OS Present

RISC OS Present is a slide presentation tool initially intended for full screen use at a presentation for ROUGOL. It provides is able to render slides from a deck, described in Markdown.

## Markdown Format

The markdown format follows the style of markdown used by [Remark](https://github.com/gnab/remark). There are limitations to what can be handled by the parser and renderer, and there are some extensions.

### Slide delimiting and styles

Slides are separated by the horizontal rule marker (`---`). The slide separator may be immediately
followed by properties which should be applied to this (and subsequent) slides. The properties take the form of colon-separated key-value pairs, which give the property name and its value. The following properties are defined:

* `background-colour`/`foreground-colour`: Changes the slide background and text foreground colour. It is not currently possible to change the colour of text inline. Colours may be named CSS colours, `#RRGGBB`, `#RGB`, `rgb(r,g,b)` or `rgb(r%,g%,b%)` values (as supported by the WebColours module).
* `*.font`/`*.size`/`*.height`: Defines font characteristics for the main styles used within the slides. The following font names may be defined (each may be suffixed by `.bold` and `.italic`):
    * `body`
    * `h1`, `h2`, `h3`
    * `code`
* `vertical-align`: Alignment of the slide content, which may take the value `top`, `middle`, or `bottom`.


### Supported markdown

The markdown supported is in line with most implementations but is limited - the [MD4C](https://github.com/mity/md4c) library is used as the back end parser. The following elements of markdown are supported:

* Heading levels 1-3 indicated by lines prefixed by `#`, `##`, `###`.
* Inline code indicated by being surrounded by backticks.
* Italic, indicated by `*` surrounding text.
* Underline, indicated by `_` surrounding text.
* Bold, indicated by `**` surrounding text.
* Bold and italic, indicated by `***` surrounding text.
* Strikethrough, indicated by `~` surrounding text.
* Ordered lists, indicated by a line starting with a `1.` (or other digit sequence), which may be indented to indicate nesting levels.
* Unordered lists, indicated by a line starting with a `*` or `-`, which may be indented to indicate nesting levels.

The encoding used for rendering is the default alphabet (no specific alphabet is selected by the Font system at the current time).

### Unsupported markdown

Any features not documented above are not supported, though partial support may exist within the system. Non-exhaustively, the following are not supported:

* Code blocks delimited by three backticks.
* Tables in any form.
* Wiki links and task lists.
* HTML blocks.
* Latex maths specifications.
* HTML entities.

The incremental slide format (two hyphens, `--`) used by Remark is not supported by the tool.

