# ifndef H_DATAFLOW_HANDLER_H
# define H_DATAFLOW_HANDLER_H

# include <tuple>
# include <type_traits>
# include <list>

# include "handlers/index.hpp"

namespace dataflow {

/// Abstract base for all the handlers
class AbstractHandler {
private:

public:
    virtual ~AbstractHandler() {}

    // HandlerSignature signature();
};

/// Handler description entry
struct HandlerDescription {
    const std::type_info & returnType;
    std::list<const std::type_info *> argumentTypes;
};

namespace meta {

/// Meta-function used to retrieve return type (plain function case).
template<typename R, typename... A>
R ret( R (*)(A...) );

/// Meta-function used to retrieve return type (class method case).
template<typename C, typename R, typename... A>
R ret( R (C::*)(A...) );

/// Meta-function used to retrieve argument types tuple.
template <typename R, typename... T>
std::tuple<T...> function_args(R (*)(T...)) {
    return std::tuple<T...>();
}

/// Handler traits struct -- depending on T routes to either a function traits
/// for stateless handlers, or to class traits for stateful hadnlers.
template<typename T, bool=std::is_function<T>::value>
struct HandlerTraits;

template<typename T, typename... Ts>
inline typename std::enable_if<sizeof...(Ts) == 0, void>::type
append_arg_typeinfo( std::list<const std::type_info *> & l ) {
    l.push_back(&typeid(T));
}

template<typename T, typename... Ts>
inline typename std::enable_if<sizeof...(Ts) != 0, void>::type
append_arg_typeinfo( std::list<const std::type_info *> & l ) {
    l.push_back(&typeid(T));
    append_arg_typeinfo<Ts...>(l);
}

template<typename R, typename... Ts> void
retrieve_arg_types( R (*)(Ts...), std::list<const std::type_info *> & l ) {
    append_arg_typeinfo<Ts...>(l);
}

/// Function traits
template<typename T>
struct HandlerTraits<T, true> {
    template<T f>
    static HandlerDescription description() {
        static_assert( 0 < std::tuple_size<decltype(function_args(f))>::value
                     , "Unable to declare stateless handler with no I/O ports." );
        HandlerDescription hd { typeid( ret(f) ) };
        retrieve_arg_types(f, hd.argumentTypes);
        //hd.argumetTypes.push_back
        return hd;
    }
};

/// Class traits
template<typename T>
struct HandlerTraits<T, false> {
    template<typename TT>
    static HandlerDescription description() {
        
    }
};


}  // namespace ::dataflow::meta
}  // namespace dataflow

# define DATAFLOW_ADD_FUNCTION( strName, C )                                \
const static ::dataflow::HandlerDescription __ ## strName ## _handlerInfo   \
    = ::dataflow::meta::HandlerTraits<decltype(C)>::description<C>();       \
/* call HandlersIndex::self().emplace(strName, &__ ## f ## _handlerInfo ) */

# endif  // H_DATAFLOW_HANDLER_H

