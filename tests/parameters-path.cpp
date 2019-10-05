# include "parameters/path.hpp"

# include "gtest/gtest.h"

/*
 * Unit test checking basic parameter path treatment capabilities of the
 * dataflow framework.
 */

using namespace dataflow::config;

// Tests tokenization returns nullptr for empty string
TEST( Configuration, emptyPathToken ) {
    Path * p = Path::from_string( "" );
    ASSERT_EQ(nullptr, p);
    delete p;
}

// Test tokenization indeed returns string token for string token
TEST( Configuration, stringPathToken ) {
    Path * p = Path::from_string( "a" );
    ASSERT_NE(nullptr, p);
    ASSERT_TRUE( p->is_str() );
    ASSERT_STREQ( p->str_first(), "a" );
    ASSERT_EQ(nullptr, p->next());
    delete p;
    p = Path::from_string( "aleph" );
    ASSERT_NE(nullptr, p);
    ASSERT_TRUE( p->is_str() );
    ASSERT_STREQ( p->str_first(), "aleph" );
    ASSERT_EQ(nullptr, p->next());
    delete p;
}

// Test tokenization indeed returns int token for string digits token
TEST( Configuration, intPathToken ) {
    Path * p = Path::from_string( "[0]" );
    ASSERT_NE(nullptr, p);
    ASSERT_FALSE( p->is_str() );
    ASSERT_EQ( p->n(), 0 );
    ASSERT_EQ(nullptr, p->next());
    delete p;
    p = Path::from_string( "[321]" );
    ASSERT_NE(nullptr, p);
    ASSERT_FALSE( p->is_str() );
    ASSERT_EQ( p->n(), 321 );
    ASSERT_EQ(nullptr, p->next());
    delete p;
}

// Test tokenization chain.
TEST( Configuration, pathParsing ) {
    Path * p = Path::from_string( "[0]._one.a[3][43].four" ),
         * n;
    ASSERT_NE( nullptr, p );
    // [0]
    ASSERT_FALSE( p->is_str() );
    ASSERT_EQ( p->n(), 0 );
    ASSERT_NE( nullptr, p->next() );
    // on_e
    n = p->next();
    ASSERT_TRUE( n->is_str() );
    ASSERT_STREQ( n->str_first(), "_one" );
    ASSERT_NE( nullptr, n->next() );
    // a
    n = n->next();
    ASSERT_TRUE( n->is_str() );
    ASSERT_STREQ( n->str_first(), "a" );
    ASSERT_NE( nullptr, n->next() );
    // [3]
    n = n->next();
    ASSERT_FALSE( n->is_str() );
    ASSERT_EQ( n->n(), 3 );
    ASSERT_NE( nullptr, n->next() );
    // [43]
    n = n->next();
    ASSERT_FALSE( n->is_str() );
    ASSERT_EQ( n->n(), 43 );
    ASSERT_NE( nullptr, n->next() );
    // four
    n = n->next();
    ASSERT_TRUE( n->is_str() );
    ASSERT_STREQ( n->str_first(), "four" );
    ASSERT_EQ( nullptr, n->next() );
    delete p;
}

TEST( Configuration, emitErrors ) {
    // Test tokenization is capable to handle unbalanced brackets
    ASSERT_THROW( Path::from_string( "[" )
                , InvalidPathString );
    ASSERT_THROW( Path::from_string( "one[12.two" )
                , InvalidPathString );
    ASSERT_THROW( Path::from_string( "one[].two" )
                , InvalidPathString );
    // Bad path string format
    ASSERT_THROW( Path::from_string( ".two" )
                , InvalidPathString );
    ASSERT_THROW( Path::from_string( "one..two" )
                , InvalidPathString );
}

