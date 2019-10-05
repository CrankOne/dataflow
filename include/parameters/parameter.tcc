# ifndef H_DATAFLOW_SYS_IPARAMETER_H
# define H_DATAFLOW_SYS_IPARAMETER_H

# include <map>
# include <string>

namespace dataflow {
namespace config {

template<typename T> class Parameter;
template<typename T> struct ParameterTypeTraits;

/// Code of parameters type
enum ParameterType {
    kDict   = 0x1,
    kTuple  = 0x2,

    kLogic  = 0x4 ,
    //kLogicDict  = kLogic | kDict,
    //kLogicTuple = kLogic | kTuple,

    kInt    = 0x8 ,
    //kDictLogic  = kInt | kDict,
    //kTupleLogic = kInt | kTuple,

    kReal   = 0x10,
    //kDictReal  = kReal | kDict,
    //kTupleReal = kReal | kTuple,

    kString = 0x20,
    //kDictString  = kString | kDict,
    //kTupleString = kString | kTuple,
};

template<> struct ParameterTypeTraits<bool> {
    constexpr static ParameterType code = kLogic;
    typedef bool CRef;
};

template<> struct ParameterTypeTraits<int> {
    constexpr static ParameterType code = kInt;
    typedef int CRef;
};

template<> struct ParameterTypeTraits<double> {
    constexpr static ParameterType code = kReal;
    typedef const double & CRef;
};

template<> struct ParameterTypeTraits<std::string> {
    constexpr static ParameterType code = kString;
    typedef const std::string & CRef;
};

/// Base class for all the parameters.
class AbstractParameter {
private:
    ParameterType _pType;
protected:
    AbstractParameter( ParameterType pt ) : _pType(pt) {}
public:
    ParameterType type_code() const { return _pType; }

    virtual ~AbstractParameter() {}

    /// Retreival shortcut.
    template<typename T> typename ParameterTypeTraits<T>::CRef as() const {
        return dynamic_cast<const Parameter<T>&>(*this).value();
    }
};

/// Template instance of the parameter
template<typename T>
class Parameter : public AbstractParameter {
private:
    T _value;
public:
    Parameter()
        : AbstractParameter( ParameterTypeTraits<T>::code ) {}
    Parameter( typename ParameterTypeTraits<T>::CRef v )
        : AbstractParameter( ParameterTypeTraits<T>::code )
        , _value( v ) {}
    typename ParameterTypeTraits<T>::CRef value() const { return _value; }
    void value( typename ParameterTypeTraits<T>::CRef v ) { _value = v; }
};

class Tuple : public AbstractParameter
            , public std::map<size_t, AbstractParameter *> {
public:
    Tuple() : AbstractParameter( kTuple ) {}
};  // class Tuple

class Dictionary : public AbstractParameter
                 , public std::map<std::string, AbstractParameter *> {
public:
    Dictionary() : AbstractParameter( kDict ) {}
};  // class Dictionary

}  // namespace ::dataflow::config
}  // namespace ::dataflow

# endif  // H_DATAFLOW_SYS_IPARAMETER_H
