# ifndef H_DATAFLOW_SYS_IPARAMETER_H
# define H_DATAFLOW_SYS_IPARAMETER_H

namespace dataflow {
namespace config {

template<typename T> struct ParameterTypeTraits;

/// Code of parameters type
enum ParameterType {
    kLogic  = 0x4,
    kInt    = 0x8 ,
    kReal   = 0x10,
    kString = 0x20
};

const int kDict = 0x1;
const int kTuple = 0x2;

template<> struct ParameterTypeTraits<bool> {
    constexpr static int code = kLogic;
    typedef bool CRef;
};

template<> struct ParameterTypeTraits<int> {
    constexpr static int code = kInt;
    typedef int CRef;
};

template<> struct ParameterTypeTraits<double> {
    constexpr static int code = kReal;
    typedef double& CRef;
};

/// Base class for all the parameters.
class AbstractParameter {
private:
    ParameterType _pType;
protected:
    AbstractParameter( ParameterType pt ) : _pType(pt) {}
public:
    ParameterType type_code() const { return _pType; }

    /// Retreival shortcut.
    template<typename T> const T & as() const {
        return dynamic_cast<T*>(*this).value();
    }
};

/// Template instance of the parameter
template<typename T>
class Parameter : public AbstractParameter {
private:
    T _value;
public:
    Parameter() : AbstractParameter( ParameterTypeTraits<T>::code ) {}
    T value() const { return _value; }
    void value( typename ParameterTypeTraits<T>::CRef v ) { _value = v; }
};

}  // namespace ::dataflow::config
}  // namespace ::dataflow

# endif  // H_DATAFLOW_SYS_IPARAMETER_H
