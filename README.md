# RISC OS Present

RISC OS Present is a slide presentation tool initially intended for full screen use at a presentation for ROUGOL. It provides is able to render slides from a deck, described in Markdown.

## Markdown Format

The markdown format follows the style of markdown used by [Remark](https://github.com/gnab/remark). There are limitations to what can be handled by the parser and renderer, and there are some extensions.

### Slide delimiting and styles

Slides are separated by the horizontal rule marker (`---`). The slide separator may be immediately
followed by properties which should be applied to this (and subsequent) slides. The properties take the form of colon-separated key-value pairs, which give the property name and its value. Properties may be deleted by prefixing their name with a single `-` character.

The following properties are defined:

* `name`: Declares a name for this slide (used by the templates)
* `template`: Declares the slide name which should be used as the basis for this slide - must be the first property of the slide. The templates only take property information from the templated slide.
* `background-colour`/`foreground-colour`: Changes the slide background and text foreground colour. It is not currently possible to change the colour of text inline. Colours may be named CSS colours, `#RRGGBB`, `#RGB`, `rgb(r,g,b)` or `rgb(r%,g%,b%)` values (as supported by the WebColours module).
* `*.font`/`*.size`/`*.height`: Defines font characteristics for the main styles used within the slides. Each font name may be suffixed by `.bold` and `.italic` (in that order) to configure specific font styles. The size and height are only configurable on the base font at the current time. The following font names may be defined:
    * `body`
    * `h1`, `h2`, `h3`
    * `code`
    * `marginal`
* `h#.padding.top`/`h#.padding.bottom`/`h#.padding.inset`: Defines the padding at the top and the bottom of the headings, and the inset for the text in the heading.
* `vertical-align`: Alignment of the slide content, which may take the value `top`, `middle`, or `bottom`.
* `padding`/`padding-horizontal`/`padding-vertical`: Defines the size of the slide padding.
* `list.padding.top`/`list.padding.bottom`: Defines the space around lists.
* `item.padding.top`/`item.padding.bottom`: Defines the space around each list item.
* `item.padding.inset`: Defines the inset horizontally of each list item's text (or bullet).
* `item.padding.offset`: Defines the offset horizontally from the inset point to the text of the item's body.
* `pre.padding.inset`: Defines the inset horizontally that the preformatted text blocks use
* `pre.padding.inside`: Defines the space between the border and the content of the preformatted text.
* `pre.border.width`: Defines the width of the border around preformatted text.
* `pre.border.type`: Defines the type of border placed around the preformatted region, which can take the following values:
    * `none`: No border.
    * `solid`: Single colour, solid border.
    * `double`: Two borders, each with width 1/3 of the set width.
    * `inset`: A 3D effect, sunk into the region (light top left).
    * `outset`: A 3D effect, raised from the region (light top left).
    * `ridge`: A 3D effect, a ridge around the region (light top left).
    * `groove`: A 3D effect, a sunken groove around the region (light top left).
* `pre.border.background-colour`: Defines the colour of the background, or `none` to not use any
* `pre.border.colour`: Defines the colour of the border, or `none` to not use any
* `pre.border.fore-colour`: Defines the colour of the 3D effect fore colour, or `none` to not use any
* `pre.border.opposite-colour`: Defines the colour of the 3D effect opposite colour, or `none` to not use any
* `quote.padding.top`/`quote.padding.bottom`: Defines the space around a block quote
* `quote.padding.inset`: Defines the inset horizontally of the block quote.
* `logo.POSITION`: Defines an image to place in a given position (see below)
* `logo.POSITION.inset`: Defines the gap around the outside of the image to leave.
* `logo.POSITION.width`/`logo.POSITION.height`: Defines the width/height of the image (which will be limited by the size of the slide)
* `slidenumber.position`: defines a position for the slide number label on each slide, where the position value can be any of the position specifications (or `none` to remove the slide number).
* `slidenumber.padding`: defines the spacing from the edge of the screen for the slide number.
* `slidenumber.format`: defines how the slide number is printed as a string. `$s` is replaced by the slide number, and `$m` by the maximum slider number.

### Position specifications

Positions are used for a few of the properties. The following positions are defined:

* `top-left`
* `top`
* `top-right`
* `left`
* `centre`
* `right`
* `bottom-left`
* `bottom`
* `bottom-right`

### Size specifications

Sizes can be in pixels (which corresponds to 2 OS units, even in EX0/EY0 modes) or with size specifications:

* `#pt`: In points (effectively 1 pixels at present)
* `#pc`: In picas, which are 12 points.
* `#px`: Explicitly in pixels (which remain 2 OS units)
* `#in`: In inches, which are 72 pixels.
* `#cm`: In centimetres, which are 28.3125 pixels.
* `#em`: In multiples of the 'em' size of the plain base font in the slide; the size will differ in the x and y dimentions.
* `#%`: In multiples of the space available; the size will differ in the x and y dimentions. Because the size depends on the context in which it is used, this percentage may be relative to two spaces:
    * the slide space, for content which is global to the slide
    * the remaining space in a block, for content which is inline with the body of the slide

Sizes may be specified as floating point values.

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
* Block quotes, indicated by a line starting with a `>`.
* Code blocks delimited by three backticks.

The encoding used for rendering is the default alphabet (no specific alphabet is selected by the Font system at the current time).

### Unsupported markdown

Any features not documented above are not supported, though partial support may exist within the system. Non-exhaustively, the following are not supported:

* Tables in any form.
* Wiki links and task lists.
* HTML blocks.
* Latex maths specifications.
* HTML entities.

The incremental slide format (two hyphens, `--`) used by Remark is not supported by the tool.

