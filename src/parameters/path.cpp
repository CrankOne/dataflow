# include "parameters/path.hpp"

# include <cstring>
# include <regex>

static std::regex gReStrPathTok( "^[a-zA-Z_][a-zA-Z0-9_]*$" )
                , gReIntPathTok( "^0|([1-9][0-9]*)$" )
                ;

namespace dataflow {
namespace config {

const char gPathDelimeter = '.';
const char gIndexBrackets[2] = { '[', ']' };
static const char _gDelims[] = { gPathDelimeter, gIndexBrackets[0], '\0' };

InvalidPathString::InvalidPathString( const char * reason ) :
    std::runtime_error(reason) {}

void
InvalidPathString::push_token( const std::string & tok ) {
    _toks.push_back(tok);
}

const char *
InvalidPathString::what() const throw () {
    size_t nCharsUsed = snprintf( _bf, sizeof(_bf)
                                , "%s; stack: "
                                , std::runtime_error::what());
    for( auto it = _toks.crbegin(); it != _toks.crend(); ++it ) {
        nCharsUsed += snprintf( _bf + nCharsUsed
                              , sizeof(_bf) - nCharsUsed
                              , " \"%s\"", it->c_str() );
    }
    return _bf;
}

Path *
Path::from_string( const std::string & strPath ) {
    if( strPath.empty() ) return nullptr;
    Path * result;
    size_t nextTokStart;
    std::string cToken;
    if( gIndexBrackets[0] == strPath[0] ) {
        cToken = strPath.substr(1, (nextTokStart = strPath.find(gIndexBrackets[1]))-1 );
        if( ! std::regex_match(cToken, gReIntPathTok ) ) {
            char bf[128];
            snprintf( bf, sizeof(bf), "Bad integer path token: \"%s\""
                    , cToken.c_str() );
            throw InvalidPathString( bf );
        }
        result = new Path( std::stol( cToken ));
        // todo: ^^^ octal/hex literals
        if( std::string::npos == nextTokStart ) {
            delete result;
            throw InvalidPathString( "Unbalanced brackets." );
        }
        ++nextTokStart;
        if( gPathDelimeter == strPath[nextTokStart] ) ++nextTokStart;
    } else {
        nextTokStart = strPath.find_first_of( _gDelims );
        result = new Path( cToken = strPath.substr(0, nextTokStart) );
        if( nextTokStart != std::string::npos 
         && strPath[nextTokStart] != gIndexBrackets[0] ) ++nextTokStart;
    }
    # if 0
    // XXX, Useful to debug complex cases
    std::cout << " + token \"" << cToken << "\", "
              << nextTokStart
              << " ? "
              << strPath.size() << std::endl;
    # endif
    try {
        if( nextTokStart < strPath.size() ) {
            result->_next = from_string( strPath.substr( nextTokStart
                                                       , std::string::npos) );
        }
    } catch( InvalidPathString & e ) {
        e.push_token( cToken );
        delete result;
        throw e;
    }
    return result;
}

Path::Path( size_t index ) : _isStr(false)
                           , _next(nullptr) {
    _key.n = index;
}

Path::Path( const std::string & strTok ) : _isStr(false)  // protect from delete on failure
                                         , _next(nullptr) {
    if( strTok.empty() ) {
        throw InvalidPathString( "Empty path str-token provided" );
    }
    if( !std::regex_match(strTok, gReStrPathTok) ) {
        char bf[128];
        snprintf( bf, sizeof(bf), "Bad string path token: \"%s\""
                , strTok.c_str() );
        throw InvalidPathString( bf );
    }
    _key.str = new char [ strTok.size() + 1 ];
    _isStr = true;  // set it now -- dtr is now responsible for cleaning
    strncpy( _key.str, strTok.c_str(), strTok.size() );
    _key.str[strTok.size()] = '\0';
}

const char *
Path::str_first() const {
    if( ! is_str() ) {
        throw std::runtime_error( "Path token is an index while string requested." );
    }
    return _key.str;
}

size_t
Path::n() const {
    if( is_str() ) {
        throw std::runtime_error( "Path token is a string while index requested." );
    }
    return _key.n;
}

std::ostream &
operator<<( std::ostream & os, const Path & path ) {
    for( const Path * p = &path; p; p = p->next() ) {
        if( p->is_str() ) {
            if( p != &path ) os << gPathDelimeter;
            os << p->str_first();
        } else {
            os << gIndexBrackets[0] << p->n() << gIndexBrackets[1];
        }
    }
    return os;
}

AbstractParameter &
get_parameter_ref( AbstractParameter & root
                 , const Path * pathPtr ) {
    if( !pathPtr ) return root;
    if( pathPtr->is_str() ) {
        if( ! (root.type_code() & kDict) ) {
            char bf[128];
            snprintf( bf, sizeof(bf)
                    , "Unable to retrieve \"%s\" from %p: not a dictionary."
                    , pathPtr->str_first()
                    , &root );
            throw std::runtime_error( bf );
        }
        Dictionary & d = static_cast<Dictionary &>( root );
        return get_parameter_ref( *d[pathPtr->str_first()]
                                , pathPtr->next() );
    } else {
        if( ! (root.type_code() & kTuple) ) {
            char bf[128];
            snprintf( bf, sizeof(bf)
                    , "Unable to retrieve #%d from %p: not a list."
                    , pathPtr->_key.n
                    , &root );
            throw std::runtime_error( bf );
        }
        Tuple & t = static_cast<Tuple &>( root );
        return get_parameter_ref( *t[pathPtr->n()]
                                , pathPtr->next() );
    }
}


}  // namespace ::dataflow::config
}  // namespace ::dataflow

