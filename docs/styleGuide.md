# Style guide

This guide applies specifically to code written in C++.
It also applies in large part to C and Java.

## Identifier naming

Identifier length:
Single letters are only acceptable as iterators (such as in 'for' loops)
No hard maximum character limit
Identifiers should be long enough to make their meaning clear, and no longer
Prefer shorter, more concise identifiers when possible

Allowed characters:
Lower and upper case letters are allowed
Numerals are allowed
Underscores are allowed, but only as delimiters in specific types of identifiers
(see the rules on multiple word identifiers)
No other characters are allowed
Identifiers should always begin with a letter
(case determined by the rules on multiple word identifiers)

Multiple word identifiers:
Variables and functions should use lowerCamelCase
Constants, macros, and environment variables should use all uppercase, and separate words with underscores
Classes and structures should use UpperCamelCase
Regarding letter case, treat acronyms like any other normal word
('HTML file' becomes 'htmlFile', 'other HTML file' becomes 'otherHtmlFile')

Do not use either Apps or Systems Hungarian notation
Avoid acronyms and abbreviations unless they are a standard usage (such as HTML)

## Comments

Single line and multiline comments are both allowed
Lines including a comment should begin with that comment
(i.e., no lines with code and then a comment following on the same line)
An exception to this rule is commented out code within a line of code
Multiline comments should begin on the same line as their opener and end on the same line as their closer
Multiline comments should include a space after the opener and before the closer
Additional lines within multiline comments should not have any special prefix or suffix
(e.g., no '*' at the beginning or end of the line)
Multiline comments that are temporary "development" comments (e.g., commented out code) should use the '/**  */' style
All other multiline comments should use the '/*  */' style
Temporary multiline comments may omit the space after the opener and before the closer
(generally done if the comment is commented out code)
Single line comments should include a space after the opener
Single line comments should not have any special suffix
All other single line comments should use the '// ' style

The 'TODO' tag represents either temporary code or acts as a placeholder where additional code is required
The 'TODO' tag should only be used with temporary comments
In fact, the 'TODO' tag should always be included with temporary comments
The 'TODO' tag should always be followed by a description
In both single line and multiline comments, the 'TODO' tag should immediately follow the opener
(before the space following the opener, which should always be included when using the 'TODO' tag)
The 'TODO' tag's associated description should be included after the space following the opener
If additional comment follows the associated description, it should be preceded by a space
However, comments with 'TODO' tags should generally be placed on the line below the 'TODO' tag

Comments that are some sort of statement or description should begin with a capital letter
Other comments, such as those indicating what units a variable is in, can begin with a lower case letter
Generally, comments are not considered proper sentences, and do not include periods at the end
However, other punctuation is frequently used, and no punctuation is expressly forbidden

## Indentation, whitespace, line length, and source file format

The opening brace of a compound statement should be placed on the line
above the first line of the compound statement's code
If a compound statement is part of another statement (e.g., an 'if' statement),
its opening brace should be placed on the same line as that statement and should be preceded by a space
The closing brace of a compound statement should be placed on the line
below the last line of the compound statement's code, and follow the indentation of the line including the opening brace
All compound statements that are part of another statement should use braces,
even if they are not required by the language
'else' statements should begin on the same line as the closing brace of their parent 'if' statement

Lines may be indented (indentation format explained below)
A line's indentation should follow scope
Lines should never end in any whitespace

When defining a function or using a statement that involves parentheses,
there should be a space before the opening parenthesis, and a space after the closing parenthesis
When declaring or calling a function (or casting as in 'int(foo)'), there should be no space
before the opening parenthesis or after the closing parenthesis
When casting as in '(int) a', a space should follow the closing parenthesis
If any of the above rules regarding parentheses and spaces would leave a space
before a semicolon or at the end of a line, omit the space
Commas should always be followed by a space
Semicolons should always be followed by a space, except when they end a line
Standard operators (e.g., '=' or '==') should be preceded and followed by a space
When declaring a pointer or reference, the '*' or '&' should immediately follow the variable type
Regardless of the rules regarding spaces, there should never be a space next to another space

Line endings should always be '\n'
Tabs should be soft (i.e., spaces instead of tab characters) and represented by 4 spaces

Line length should be less than or equal to 120 characters

Source files should be UTF-8 encoded
Source files should end in an empty line
