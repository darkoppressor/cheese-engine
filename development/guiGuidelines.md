* Any/all scrolling buttons in a window must use the same size font

* When creating a window via scripting, always fully define the window before adding any infos or buttons

* Windows should be no larger than 800x480

* When creating colored text in an Information, use this format:
"<color_name>Colored text looks like this<>, while regular text looks like this."

## When specifying the location (or starting position in the case of windows) of a GUI object

* A value of -1 means "center in parent"
* A value of -2 means "align to parent's right/top"
* A value of -3 means "align to parent's left/bottom"

## Allowed input types for mutable text

* backspace
* newline
* space
* letters
* numbers
* symbols
* symbols_posix

## 256x144

* Initial object height: 18
* Object spacing: 14
* Spacing from arbitrary height (such as between bottom of scrolling text box and button): 2
* Window height: Height of bottom object + 16

## 1280x720

* Initial object height: 36
* Object spacing: 22
* Spacing from arbitrary height (such as between bottom of scrolling text box and button): 5
* Window height: Height of bottom object + 27
