# ifndef H_DATAFLOW_SYS_PATH_H
# define H_DATAFLOW_SYS_PATH_H

# include "parameters/parameter.tcc"

# include <string>
# include <ostream>
# include <list>

/*!\defgroup Parameters
 * \brief Run-time configuration sub-system module.
 *
 * API includes basic run-time configuration object model with set/get
 * lifecycle and implied to be a subject of various configuration adapters.
 *
 * See \ref parameters-tutorial for detailed explaination.
 * */

namespace dataflow {

/// \addtogroup Parameters
/// @{

/// \brief Run-time configuration subsystem namespace.
namespace config {

/// Path grammar piece: string tokens delimiter (global constant).
extern const char gPathDelimeter;
/// Path grammar piece: integer index brackets (global constant).
extern const char gIndexBrackets[2];

/// \brief Exception thrown on path validation/parsing error.
/// \details Inherited from `std::runtime_error` this exception subclass
/// represents a general error arised during parsing of the parameter path
/// string.
///
/// Usage example:
/// \code
/// try: {
///     dataflow::config::Path::from_string("one.two[3three");
/// } catch( dataflow::config::InvalidPathString & e ) {
///     // whill print message about unbalanced bracket
///     std::cerr << e.what() << std::endl;
/// }
/// \endcode
class InvalidPathString : public std::runtime_error {
private:
    mutable char _bf[256];
    std::list<std::string> _toks;
public:
    /// Trivial ctr is explicitly deleted as no appropriate use case foreseen.
    InvalidPathString() = delete;
    /// \brief Exception constructor: accepts human-readable generic error
    /// description.
    /// \details The parsed tokens history is then added using push_token()
    /// method, while parsing stack unwinds.
    InvalidPathString( const char * reason );
    /// \brief During parsing procedure, receives the stack of tokens currently
    /// parsed. \details Used in Path::from_string() constructor to fill the
    /// exception with information of tokens currenlty being parsed (and, thus,
    /// supposedly valid).
    void push_token( const std::string & );
    /// \brief Returns stack of tokens being parsed.
    /// \details As it supposed to be called mainly within what() method, has a
    /// no-throw qualifier.
    std::list<std::string> stack() const throw () { return _toks; }
    /// \brief Returns human-readable error description with stack.
    /// \details The stack is appended to the end of the string, tokens
    /// separated by comma. Uses associated buffer to render the message, so
    /// message length is limited by sizeof(_bf) characters.
    virtual const char* what() const throw ();
};

/// \brief A representation of the parameter path string being parsed.
/// \details String path has to be tokenized into this representation. Each
/// token within the path grammar can be either a string denoting an element
/// in a dictionary, or a number referencing element in a tuple.
///
/// The suggested way to construct the Path instance is to apply the
/// from_string() static method to string literal or `std::string` instance.
/// \warning Since from_string() allocates Path instances on heap with `new`
/// operator user must either to delete them with `delete` operator or to wrap
/// the resulting pointer to `std::shared_ptr` to avoid memory leaks.
///
/// Examples:
/// * Create from string literal and manually delete instance:
/// \code
/// Path * p = Path::from_string( "one.two[42][0]" );
/// delete p;
/// \endcode
/// * Create from string variable as memory-safe shared pointer:
/// \code
/// std::shared_ptr<Path> p(Path::from_string( userStr ));
/// \endcode
///
/// The resulting Path instance has to be then used in get_parameter_ref()
/// function to obtain a parameter over a subset (Dictionary or Tuple).
class Path {
public:
    union {
        char * str;  ///< Set to word identifier when token is string
        size_t n;  ///< Set to integer value when token is integer
    } _key;  ///< Stores either a string key, or an integer
private:
    bool _isStr;  ///< `true`, if token is a string, false if it is a number
    Path * _next;  ///< Pointer to the next element, if chained
protected:
    /// Constructs new path token instance with integer.
    Path( size_t );
    /// Constructs new path token from string _key.
    Path( const std::string & );
public:
    /// Tokenizes given string into path returning new list of tokens. Invokes
    /// `new' allocation internally, so one need to `delete' the pointer returned
    /// when object is no longer needed.
    static Path * from_string( const std::string & );
    /// Destructor recursively deleting the subsequent tokens in the list.
    ~Path() {
        if( _next ) delete _next; 
        if( _isStr ) delete [] _key.str;
    }
    /// Returns true if its is a string token.
    bool is_str() const { return _isStr; }
    /// Returns next token. `nullptr' will be returned in case it is the
    /// last/single token in the path.
    Path * next() { return _next; }
    /// Returns next token. `nullptr' will be returned in case it is the
    /// last/single token in the path. Const version.
    const Path * next() const { return _next; }

    /// Returns string token value, if this is a string token otherwise throws
    /// a `runtime_error').
    const char * str_first() const;
    /// Returns an int value, if this is a string token otherwise throws a
    /// `runtime_error').
    size_t n() const;
};

/// Renders Path list instance into string representation
std::ostream & operator<<( std::ostream &, const Path & );

///\brief Returns parameter instance reference by given path tokens list. If
/// path pointer is NULL, returns reference to the `root'.
///\details Performs recursive traversing of the given AbstractParameter
/// instance with respect to Path token list. Basic type checking is performed:
/// raises `std::runtime_error` if string path token derefeences Tuple or
/// integer path token dereferences Dictionary.
/// \code
/// Path * p = Path::from_string("one.two");
/// AbstractParameter &param = get_parameter_ref( dct, p );
/// delete p;
/// \endcode
/// Assuming dct is an instance of Dictionary.
/// \param root input parameters node
/// \param pathPtr Head of the path tokens list
AbstractParameter &
get_parameter_ref( AbstractParameter & root
                 , const Path * pathPtr=nullptr );


}  // namespace ::dataflow::config
/// @} End of Parameters group

}  // namespace ::dataflow

# endif  // H_DATAFLOW_SYS_PATH_H

