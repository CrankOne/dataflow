# A Dataflow Library

This project implements a bare minimum framework to organize set of reentrant
routines into extensible dataflow. It is based entirely on STL with only couple
of optional dependencies: for unit tests
([gtest](https://github.com/google/googletest)) and documentation
([Doxygen](http://www.doxygen.nl)).

The purpose of this project is to provide efficient and robust minimalistic
solution for organizing complex data-processing applications basing on the
dataflow ([pipeline](https://en.wikipedia.org/wiki/Pipeline_(software)))
pattern. By heavily relying on C++ metaprogramming the library hides
complex details of the runtime-extensibility, parallel data routing and
state-awareness inside, offering user a subset of diversified extension points.
Semi-parallel execution is applied to gain performance but remain aware of the
stateful nature of user's handlers.

The application area of this framework is data processing in high-energy
physics, from which many restrictions had come:

* If we had to choose between flexibility and simplicity, we prefer simplicity
as our primary users are not familiar with C++11, data introspection,
modern serialization libraries and frameworks
* We try to expose C++ templates (being heavily used here) to user as less as
possible. Weirdly they do feel more comfortable with opaue preprocessor macro

## Index of Handlers

Handlers have to be instantiated by registered constructors in order to provide
a unified way to build the dataflow framework (pipeline). These constructors
have to be retrieved from the registry object and parameterised with
configuration data (see below).

For user's convenience the following forms of constructors implementations are
foreseen:

* Simple stateless handler -- a function, expressed as an ordinary C++ function
* Stateful handler -- a class implementing an interface

From given function declaration the modern C++ metaprogramming features are
applied to deduce the input and output (if any) handler types. Run-time
RTTI-based check is then performed to assure handlers I/O type correctness.

## Handlers Parameterisation

For the sake of simplicity, the hierarchical parameterisation supports only the
following subset of C++ types:

* Logic value (`bool`)
* Signed integer value (`int`)
* Floating point value (`double`)
* String (`std::string`)

Two complex types are represented by associative arrays:

* Indexed by string ("dictionary")
* Indexed by positive long integer value ("tuple")

All the types are stored in wrapper C++ classes with common base (similar to
`boost::any`) supporting type-casting shortcut (`.as<type>()`).

The goal of this configuration objects is to standardize handlers
instantiation during pipeline creation process.

# Examples

Full API documentation contains `examples` section offering more elaborated use
cases as well as references to particular API details.

## Simple Cut

Following C++ source code will create a simple stateless handler, which blocks
propagation of negative numbers.

    \code{cpp}
    # include <dataflow.h>

    bool cut( double & x ) {
        // Handlers may return special values to steer the pipeline processing
        // status. For boolean return type, `false' abrupts the further
        // propagation of the result.
        return x > 0;
    }

    DATAFLOW_REGISTER_HANDLER( "cut_negative", cut )
    \endcode

Primitive pipeline processing is then could be done with:

    \code{cpp}
    # include <dataflow.h>

    int main() {
        // This sequence will go to the pipeline input
        const double values[] = { -1, -2, 3, -4, 5 };
        // Create a pipeline with single handler within
        Pipeline p("cut_negative");
        // Feed the pipeline with numbers and print value only if something
        // is "succeed".
        for( int i = 0; i < 5; ++i ) {
            if( (p << values[i]).succeed() ) {
                std::cout << p.get<double>() << std::endl;
            }
        }
        return 0;
    }
    \endcode

As a result only `3` and `5` will be printed to `stdout`. This toy example
may be expressed as single `.cpp` file as well as of library and executable.

## Cut-and-double example

With `main()` function from "simple cut" example above one may impose a
slightly modified version of handler:

    \code{cpp}
    # include <dataflow.h>

    bool cut_n_double( double & x ) {
        // Argument in this function's signature is given by non-constant
        // reference meaning that it accepts value that may be modified.
        // Dataflow understands such handlers as data-transforming handler.
        if( x < 0 ) {
            return false;
        } else {
            x = 2*x;
            return true;
        }
    }

    DATAFLOW_REGISTER_HANDLER( "cut_negative_double_positive", cut )
    \endcode

Which, whith `main()` being modified only to construct pipeline with processor
`"cut_negative_double_positive"` will now print `6` and `10`.

More elaborated details may be found in tutorial.

## Alternatives

The dataflow pattern implemented in C++ is a somewhat hot topic on the open
source stage: solutions of varying grade of maturity may be
[seen on GitHub](https://github.com/topics/dataflow-programming?l=c%2B%2B).
Some notorious specimens of them are:

* [RaftLib](https://github.com/RaftLib/RaftLib)
* [DSPatch](http://flowbasedprogramming.com/docs/html/index.html)
* [dataflow-cpp](https://github.com/Archie3d/dataflow-cpp)

Albeit these solutions are outperform this tiny library by their features and
concepts beneath the API, leading by our higly-specialized auditorium were
forced to implement our own approach: simplicity goes first!
