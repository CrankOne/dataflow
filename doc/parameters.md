# Parameters {#parameters-tutorial}

These entities construct a simplistic yet functional configuration
sub-system whis is frequently needed in almost any real-world application.

\note Despite its primary aim is to parameterize construction of dataflow
handlers, utilizing this classes is not a must: users can still rely on
their own configuration facilities. See section \ref handlers-tutorial for details.

Parameters sub-system is defined over a sub-set of the following data types:
* boolean values represented in C++ as `bool`-typed values
* integer numbers represented in C++ as `int` type
* floating point numbers represented as C++ `double` type
* string values represented as `std::string` from STL

These values may be indexed in two types of containers:
* `Dictionary` is an associative array indexed by string keys
* `Tuple` is an associative array indexed by positive integer keys

\note Name `Tuple` can be quite a misleading in the sense it typically
denotes fixed-size multi-typed ordered aggregation, but here it is just
an associative array indexed by integer values. This naming was choosen,
however to highlight that `Tuple` does not stick for any particular type
and support sparse data.

The aim was to offer user an ability to refer to their configuration in
simplest possible manner, yet in frame of C++:

    \code{cpp}
    int factor = cfg<int>("myFactor");
    bool flag = cfg<int>("someFlag");
    \endcode

Indeed, to retrieve a value from config, you need:
1. The name of the parameter as config typically contains many of them and
only way to refer to them during run-time is string expression;
2. The type of the parameter as C++ is statically-typed language. Most
natural way to provide types in C++ language is the template substitution
here done within the `<`...`>` "brackets".

Assumption done above this case is that users may want to sort their
parameters into groups, ordered or named. So, for some imaginary example,
more close to the practice one would expect something like this:

    \code{cpp}
    double pedestal28 = cfg<double>("calibration.multiwiredChamber[28].pedestal")
    \endcode

If, say, we have parameter `pedestal` included in subgroup `28` of subgroup
`multiwiredChamber` of group `calibration`.

## Path grammar

The path grammar itself is very close to one we have in C++ and consists of
the following elementary entities:
* A string identifier, similar to what one would expect from C++ variable
name: begins with alphabetical character or underscore, optionally followed
by alphabetical, digit or underscore characters.
* A numeric identifier (only decimal notation is supported) consisting of
an arbitrary number of 0-9 digits, enclosed between `[` and `]`. Negative
indexes are not supported.
* Delimiting comma `.` concatenating string identifier with previous tokens

Thus, the following string expressions are valid as parameter paths:
 * `"a"`
 * `"one"`
 * `"_foo"`
 * `"_1"`
 * `"[0]"`
 * `"[42]"`
 * `"some.very._long_and[42][12].meaningless.keys"`

It is worth to note for experienced user that this grammar itself was way
too primitive to treat it with any specialized parser, so we decided to
parse it manually using `std::string::substr()` and couple of regular
expressions.

Though it is supposed that parameters will be read from configuration file
source, the library itself does not provide any configuration file parser.
We assume that building an adapter to any of the popular formats (YAML,
JSON, XML, ini, libconfuse, etc) is a matter of taste and may become
subject of debate once being implemented at the level of dataflow library.

## Filling the Parameters Set

...

## Retrieving the Parameter Value

...

## Advanced Usage: Configuration File Adaptors

...

