# include "parameters/parameter.tcc"
# include "parameters/path.hpp"

# include "gtest/gtest.h"

/*
 * Unit test checking basic parameters indexing.
 */

using namespace dataflow::config;

// Tests simple dictionary basic parameters insertion and retrieval
TEST( Configuration, parametersDict ) {
    // Construct simple dictionary
    Dictionary dct;
    dct.emplace( "one"
               , new Parameter<bool>(true) );
    dct.emplace( "two"
               , new Parameter<int>(2) );
    dct.emplace( "three"
               , new Parameter<double>(3.) );
    dct.emplace( "four"
               , new Parameter<std::string>("some") );
    ASSERT_EQ( 4, dct.size() );
    // Retrieve data from simple dictionary
    Path * pOne   = Path::from_string( "one" )
       , * pTwo   = Path::from_string( "two" )
       , * pThree = Path::from_string( "three" )
       , * pFour  = Path::from_string( "four" )
       ;
    ASSERT_TRUE(  get_parameter_ref( dct, pOne   ).as<bool>() );
    ASSERT_EQ(    2, get_parameter_ref( dct, pTwo   ).as<int>() );
    ASSERT_EQ(    3., get_parameter_ref( dct, pThree ).as<double>() );
    ASSERT_STREQ( "some", get_parameter_ref( dct, pFour ).as<std::string>().c_str() );
    delete pOne;
    delete pTwo;
    delete pThree;
    delete pFour;
    // TODO: delete `new Parameter'
}

// Tests simple tuple basic parameters insertion and retrieval
TEST( Configuration, parametersTuple ) {
    // Construct simple dictionary
    Tuple tpl;
    tpl.emplace( 1
               , new Parameter<bool>(true) );
    tpl.emplace( 3
               , new Parameter<int>(2) );
    tpl.emplace( 12
               , new Parameter<double>(3.) );
    tpl.emplace( 42
               , new Parameter<std::string>("some") );
    ASSERT_EQ( 4, tpl.size() );
    // Retrieve data from simple dictionary
    Path * pOne   = Path::from_string( "[1]" )
       , * pTwo   = Path::from_string( "[3]" )
       , * pThree = Path::from_string( "[12]" )
       , * pFour  = Path::from_string( "[42]" )
       ;
    ASSERT_TRUE(  get_parameter_ref( tpl, pOne   ).as<bool>() );
    ASSERT_EQ(    2, get_parameter_ref( tpl, pTwo   ).as<int>() );
    ASSERT_EQ(    3., get_parameter_ref( tpl, pThree ).as<double>() );
    ASSERT_STREQ( "some", get_parameter_ref( tpl, pFour ).as<std::string>().c_str() );
    delete pOne;
    delete pTwo;
    delete pThree;
    delete pFour;
    // TODO: delete `new Parameter'
}

// Tests simple tuple basic parameters insertion and retrieval
TEST( Configuration, nestedRetrieval ) {
    Tuple tpl;
    Dictionary * dct = new Dictionary();  // has to be on heap
    tpl.emplace( 0
               , new Parameter<bool>(true) );
    tpl.emplace( 106
               , dct );
    dct->emplace( "_"
               , new Parameter<double>(3.) );
    dct->emplace( "foo"
               , new Parameter<std::string>("some") );
    ASSERT_EQ( 2, tpl.size() );
    ASSERT_EQ( 2, dct->size() );
    // Retrieve data from simple dictionary
    Path * pOne   = Path::from_string( "[0]" )
       , * pTwo   = Path::from_string( "[106]._" )
       , * pFour  = Path::from_string( "[106].foo" )
       ;
    ASSERT_TRUE(  get_parameter_ref( tpl, pOne   ).as<bool>() );
    ASSERT_EQ(    3., get_parameter_ref( tpl, pTwo   ).as<double>() );
    ASSERT_STREQ( "some", get_parameter_ref( tpl, pFour ).as<std::string>().c_str() );
    delete pOne;
    delete pTwo;
    delete pFour;
    // TODO: delete `new Parameter'
}


