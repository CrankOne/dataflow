# ifndef H_DATAFLOW_SYS_IPARAMETER_H
# define H_DATAFLOW_SYS_IPARAMETER_H

# include <map>
# include <string>
# include <memory>

/*!
 * \addtogroup Parameters
 * @{
 */

namespace dataflow {

/// \addtogroup Parameters
/// @{

//! Configuration parameters sub-system.
namespace config {

template<typename T> class Parameter;  // fwd
template<typename T> struct ParameterTypeTraits;  // fwd

//! Code of the parameter type
enum ParameterType {
    kDict   = 0x1,  ///< parameter is a dictionary
    kTuple  = 0x2,  ///< parameter is a "tuple"

    kLogic  = 0x4 ,  ///< code of scalar parameter represents boolean value
    //kLogicDict  = kLogic | kDict,
    //kLogicTuple = kLogic | kTuple,

    kInt    = 0x8 ,  ///< code of scalar parameter for integer value
    //kDictLogic  = kInt | kDict,
    //kTupleLogic = kInt | kTuple,

    kReal   = 0x10,  ///< code of scalar parameter for floating-point value
    //kDictReal  = kReal | kDict,
    //kTupleReal = kReal | kTuple,

    kString = 0x20,  ///< code of scalar parameter for string value
    //kDictString  = kString | kDict,
    //kTupleString = kString | kTuple,
};

/// Template traits specification for logic value
template<> struct ParameterTypeTraits<bool> {
    constexpr static ParameterType code = kLogic;
    typedef bool CRef;
};

/// Template traits specification for integer value
template<> struct ParameterTypeTraits<int> {
    constexpr static ParameterType code = kInt;
    typedef int CRef;
};

/// Template traits specification for floating point value
template<> struct ParameterTypeTraits<double> {
    constexpr static ParameterType code = kReal;
    typedef const double & CRef;
};

/// Template traits specification for string value
template<> struct ParameterTypeTraits<std::string> {
    constexpr static ParameterType code = kString;
    typedef const std::string & CRef;
};

/// Base class for all the parameters.
class AbstractParameter {
private:
    /// Knows its concrete type.
    ParameterType _pType;
protected:
    /// Ctr, only permitted for descendants.
    AbstractParameter( ParameterType pt ) : _pType(pt) {}
public:
    /// Returns parameter's type code
    ParameterType type_code() const { return _pType; }

    /// Dtr, made virtual to force vtable generation
    virtual ~AbstractParameter() {}

    /// Retreival shortcut template method (checked downcast).
    template<typename T> typename ParameterTypeTraits<T>::CRef as() const {
        return dynamic_cast<const Parameter<T>&>(*this).value();
    }
};

/// Template instance of the parameter
template<typename T>
class Parameter : public AbstractParameter {
private:
    /// Value kept by the parameter instance.
    T _value;
public:
    ///\brief Default ctr.
    ///\warning Value is not initialized, like in raw C++.
    Parameter()
        : AbstractParameter( ParameterTypeTraits<T>::code ) {}
    /// Constructs parameter entry and assigns value given.
    Parameter( typename ParameterTypeTraits<T>::CRef v )
        : AbstractParameter( ParameterTypeTraits<T>::code )
        , _value( v ) {}
    /// Value getter.
    typename ParameterTypeTraits<T>::CRef value() const { return _value; }
    /// Value setter.
    void value( typename ParameterTypeTraits<T>::CRef v ) { _value = v; }
};

/// \brief Associative array for enumerated entries of various types.
/// \details Although it is not a real "tuple" (e.g. statically typed set of
/// values), this container is designed to keep parameter entries of various
/// types indexed by arbitrary number. Just as classical tuple it is suitable
/// to store arrays, pairs, triplets of values, etc. to be then accessed by
/// index.
/// \note Since it is built over std::map, it has all the features original
/// class has: entries are sorted by keys, it is unbalanced RB-tree,
/// collision-safe, etc.
class Tuple : public AbstractParameter
            , public std::map<size_t, std::shared_ptr<AbstractParameter> > {
public:
    Tuple() : AbstractParameter( kTuple ) {}
};  // class Tuple

/// \brief Associative array for named entries of various types.
/// \details Most natural way to represent a set of parameters by their names.
/// \note Since it is built over std::map, it has all the features original
/// class has: entries are sorted by keys, it is unbalanced RB-tree,
/// collision-safe, etc.
class Dictionary : public AbstractParameter
                 , public std::map<std::string, std::shared_ptr<AbstractParameter> > {
public:
    Dictionary() : AbstractParameter( kDict ) {}
};  // class Dictionary

}  // namespace ::dataflow::config
/// @} End of Parameters group

}  // namespace ::dataflow

/*! @} End of Parameters group */

# endif  // H_DATAFLOW_SYS_IPARAMETER_H
