# ifndef H_DATAFLOW_SYS_PATH_H
# define H_DATAFLOW_SYS_PATH_H

# include "parameters/parameter.tcc"

# include <string>
# include <ostream>
# include <list>

namespace dataflow {
namespace config {

/// Path grammar piece: string tokens delimiter.
extern const char gPathDelimeter;
/// Path grammar piece: integer index brackets.
extern const char gIndexBrackets[2];

/// Exception thrown in case of invalid path.
class InvalidPathString : public std::runtime_error {
private:
    mutable char _bf[256];
    std::list<std::string> _toks;
public:
    InvalidPathString() = delete;
    /// Exception constructor: accepts human-readable generic error description.
    InvalidPathString( const char * reason );
    /// During parsing procedure, recieves the stack of tokens currently parsed.
    void push_token( const std::string & );
    /// Returns stack of tokens being parsed.
    std::list<std::string> stack() const throw () { return _toks; }
    /// Returns human-readable error description with stack.
    virtual const char* what() const throw ();
};

/// String path has to be tokenized into this representation. Can be either a
/// string denoting an element in a dictionary, or a number denting element in
/// a tuple.
class Path {
public:
    union {
        char * str;
        int n;
    } _key;
private:
    bool _isStr;  ///< true, if token is a string, false if it is a number
    Path * _next;  ///< pointer to the next element, if chained.
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
        if( _isStr ) delete _key.str;
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

std::ostream & operator<<( std::ostream &, const Path & );

/// Returns parameter instance reference by given path tokens list. If path
/// pointer is NULL, returns reference to the `root'.
AbstractParameter &
get_parameter_ref( AbstractParameter & root
                 , const Path * pathPtr=nullptr );


}  // namespace ::dataflow::config
}  // namespace ::dataflow

# endif  // H_DATAFLOW_SYS_PATH_H

