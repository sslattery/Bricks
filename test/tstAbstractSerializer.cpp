//----------------------------------*-C++-*----------------------------------//
/*!
 * \file   tstAbstractSerializer.cpp
 * \author Stuart Slattery
 * \date   Wed May 25 12:36:14 2011
 * \brief  Abstract serializer class unit tests.
 */
//---------------------------------------------------------------------------//

#include <iostream>
#include <vector>
#include <cmath>
#include <sstream>

#include <Bricks_AbstractSerializer.hpp>
#include <Bricks_AbstractSerializableObject.hpp>
#include <Bricks_AbstractBuilder.hpp>
#include <Bricks_AbstractBuildableObject.hpp>
#include <Bricks_AbstractObjectRegistry.hpp>

#include "Teuchos_UnitTestHarness.hpp"
#include "Teuchos_RCP.hpp"
#include "Teuchos_ArrayView.hpp"
#include "Teuchos_Array.hpp"
#include "Teuchos_DefaultComm.hpp"
#include "Teuchos_CommHelpers.hpp"
#include "Teuchos_SerializationTraits.hpp"

//---------------------------------------------------------------------------//
// HELPER CLASSES
//---------------------------------------------------------------------------//

// Base class implementation.
class BaseClassImpl
{
  public:

    BaseClassImpl() { /* ... */ }
    virtual ~BaseClassImpl() { /* ... */ }

    virtual int myNumber() = 0;
    virtual Teuchos::Array<double> myData() = 0;
        
    virtual std::string objectType() const = 0;
    virtual void serialize( const Teuchos::ArrayView<char>& buffer ) const = 0;
    virtual void deserialize( const Teuchos::ArrayView<const char>& buffer ) = 0;
};

// Base class interface.
class BaseClass : public Bricks::AbstractBuildableObject<BaseClass>
		, public Bricks::AbstractSerializableObject<BaseClass>
{
  public:

    BaseClass() { /* ... */ }
    virtual ~BaseClass() { /* ... */ }

    virtual int myNumber() { return b_impl->myNumber(); }
    virtual Teuchos::Array<double> myData() 
    { return b_impl->myData(); }
        
    virtual std::string objectType() const 
    { return b_impl->objectType(); }
    virtual void serialize( const Teuchos::ArrayView<char>& buffer ) const 
    { b_impl->serialize(buffer); }
    virtual void deserialize( const Teuchos::ArrayView<const char>& buffer )
    { b_impl->deserialize(buffer); }

    bool isImplNonnull() const { return Teuchos::nonnull(b_impl); }

  protected:

    Teuchos::RCP<BaseClassImpl> b_impl;
};

//---------------------------------------------------------------------------//
namespace Bricks
{
// AbstractBuildableObjectPolicy implementation for the base class.
template<>
class AbstractBuildableObjectPolicy<BaseClass>
{
  public:

    typedef BaseClass object_type;

    static std::string objectType( const BaseClass& object )
    {
	return object.objectType();
    }

    static Teuchos::RCP<Bricks::AbstractBuilder<BaseClass> > getBuilder()
    {
	return BaseClass::getBuilder();
    }
};

// AbstractSerializableObjectPolicy implementation for the base class.
template<>
class AbstractSerializableObjectPolicy<BaseClass>
{
  public:

    typedef BaseClass object_type;

    static bool objectHasImplementation( const BaseClass& object )
    {
	return object.isImplNonnull();
    }

    static std::size_t maxByteSize()
    {
	return BaseClass::maxByteSize();
    }

    static void serialize( const BaseClass& object,
			   const Teuchos::ArrayView<char>& buffer )
    {
	object.serialize( buffer );
    }

    static void deserialize( BaseClass& object,
			     const Teuchos::ArrayView<const char>& buffer )
    {
	object.deserialize( buffer );
    }
};
} // end namespace Bricks

//---------------------------------------------------------------------------//
// SerializationTraits implementation for the base class pointers.
namespace Teuchos
{
template<typename Ordinal>
class SerializationTraits<Ordinal,BaseClass> 
{
  public:

    typedef Bricks::AbstractSerializer<Ordinal,BaseClass>  
    AbstractSerializer;

    static const bool supportsDirectSerialization = 
	AbstractSerializer::supportsDirectSerialization;

    static Ordinal fromCountToIndirectBytes( const Ordinal count, 
					     const BaseClass buffer[] ) 
    { 
	return AbstractSerializer::fromCountToIndirectBytes( count, buffer );
    }

    static void serialize( const Ordinal count, 
			   const BaseClass buffer[], 
			   const Ordinal bytes, 
			   char charBuffer[] )
    { 
	AbstractSerializer::serialize( count, buffer, bytes, charBuffer );
    }

    static Ordinal fromIndirectBytesToCount( const Ordinal bytes, 
					     const char charBuffer[] ) 
    { 
	return AbstractSerializer::fromIndirectBytesToCount( bytes, charBuffer );
    }

    static void deserialize( const Ordinal bytes, 
			     const char charBuffer[], 
			     const Ordinal count, 
			     BaseClass buffer[] )
    { 
	AbstractSerializer::deserialize( bytes, charBuffer, count, buffer );
    }
};
} // end namespace Teuchos

//---------------------------------------------------------------------------//
// Derived class 1 implementation
class MyNumberIsOneImpl : public BaseClassImpl
{
  public:

    MyNumberIsOneImpl() : d_data( 1, 1.0 ) { /* ... */ }
    ~MyNumberIsOneImpl() { /* ... */ }

    int myNumber() { return 1; }
    Teuchos::Array<double> myData() { return d_data; }

    std::string objectType() const { return std::string("one"); }
    void serialize( const Teuchos::ArrayView<char>& buffer ) const
    { std::memcpy( const_cast<double*>(d_data.getRawPtr()), 
		   buffer.getRawPtr(), sizeof(double) ); }
    void deserialize( const Teuchos::ArrayView<const char>& buffer )
    { std::memcpy( const_cast<char*>(buffer.getRawPtr()), 
		   d_data.getRawPtr(), sizeof(double) ); }

    static std::size_t byteSize();

  private: 
    
    Teuchos::Array<double> d_data;
};

std::size_t MyNumberIsOneImpl::byteSize()
{ return sizeof(double); }

// Derived class 1 interface
class MyNumberIsOne : public BaseClass
{
  public:

    MyNumberIsOne()
    {
	this->b_impl = Teuchos::rcp( new MyNumberIsOneImpl() );
    }
    ~MyNumberIsOne() { /* ... */ }

    static std::size_t byteSize();
};

std::size_t MyNumberIsOne::byteSize()
{ return MyNumberIsOneImpl::byteSize(); }

// DerivedSerializableObjectPolicy
namespace Bricks
{
template<>
class DerivedSerializableObjectPolicy<MyNumberIsOne>
{
  public:
    typedef MyNumberIsOne object_type;
    static std::size_t byteSize()
    {
	return MyNumberIsOne::byteSize();
    }
};

// AbstractObjectRegistrationPolicy
template<>
class AbstractObjectRegistrationPolicy<MyNumberIsOne>
{
  public:

    //! Base class type.
    typedef MyNumberIsOne object_type;

    /*!
     * \brief Register a derived class with a base class.
     */
    static void registerDerivedClassWithBaseClass()
    {
	BaseClass::setDerivedClassFactory<MyNumberIsOne>();
	BaseClass::setDerivedClassByteSize<MyNumberIsOne>();
    }
};

} // end namespace Bricks

//---------------------------------------------------------------------------//
// Derived class 2 implementation.
class MyNumberIsTwoImpl : public BaseClassImpl
{
  public:

    MyNumberIsTwoImpl() : d_data( 2, 2.0 ) { /* ... */ }
    ~MyNumberIsTwoImpl() { /* ... */ }

    int myNumber() { return 2; }
    Teuchos::Array<double> myData() { return d_data; }

    std::string objectType() const { return std::string("two"); }
    void serialize( const Teuchos::ArrayView<char>& buffer ) const
    { std::memcpy( const_cast<double*>(d_data.getRawPtr()), 
		   buffer.getRawPtr(), sizeof(double) ); }
    void deserialize( const Teuchos::ArrayView<const char>& buffer )
    { std::memcpy( const_cast<char*>(buffer.getRawPtr()), 
		   d_data.getRawPtr(), sizeof(double) ); }

    static std::size_t byteSize();

  private: 
    
    Teuchos::Array<double> d_data;
};

std::size_t MyNumberIsTwoImpl::byteSize()
{ return 2*sizeof(double); }

// Derived class 2 interface
class MyNumberIsTwo : public BaseClass
{
  public:

    MyNumberIsTwo()
    {
	this->b_impl = Teuchos::rcp( new MyNumberIsTwoImpl() );
    }
    ~MyNumberIsTwo() { /* ... */ }

    static std::size_t byteSize();
};

std::size_t MyNumberIsTwo::byteSize()
{ return MyNumberIsTwoImpl::byteSize(); }

// DerivedSerializableObjectPolicy
namespace Bricks
{
template<>
class DerivedSerializableObjectPolicy<MyNumberIsTwo>
{
  public:
    typedef MyNumberIsTwo object_type;
    static std::size_t byteSize()
    {
	return MyNumberIsTwo::byteSize();
    }
};

// AbstractObjectRegistrationPolicy
template<>
class AbstractObjectRegistrationPolicy<MyNumberIsTwo>
{
  public:

    //! Base class type.
    typedef MyNumberIsTwo object_type;

    /*!
     * \brief Register a derived class with a base class.
     */
    static void registerDerivedClassWithBaseClass()
    {
	BaseClass::setDerivedClassFactory<MyNumberIsTwo>();
	BaseClass::setDerivedClassByteSize<MyNumberIsTwo>();
    }
};
} // end namespace Bricks

//---------------------------------------------------------------------------//
// TESTS
//---------------------------------------------------------------------------//
TEUCHOS_UNIT_TEST( AbstractSerializer, abstract_serializer )
{
    using namespace Bricks;

    // Register derived classes.
    AbstractObjectRegistry<BaseClass,
			   MyNumberIsOne,MyNumberIsTwo>::registerDerivedClasses();

    // Get the communicator.
    Teuchos::RCP<const Teuchos::Comm<int> > comm_default = 
	Teuchos::DefaultComm<int>::getComm();
    int comm_rank = comm_default->getRank();

    // Construct an array of base class objects
    Teuchos::RCP<AbstractBuilder<BaseClass> > builder = BaseClass::getBuilder();
    Teuchos::Array<BaseClass> objects( 2 );
    if ( comm_rank == 0 )
    {
	objects[0] = *( builder->create("one") );
	objects[1] = *( builder->create("two") );
    }

    // Broadcast the objects.
    Teuchos::broadcast( *comm_default, 0, objects() );

    // Check the objects.
    TEST_EQUALITY( 1, objects[0].myNumber() );
    TEST_EQUALITY( 1, objects[0].myData().size() );
    TEST_EQUALITY( 1.0, objects[0].myData()[0] );

    TEST_EQUALITY( 2, objects[1].myNumber() );
    TEST_EQUALITY( 2, objects[1].myData().size() );
    TEST_EQUALITY( 2.0, objects[1].myData()[0] );
    TEST_EQUALITY( 2.0, objects[1].myData()[1] );
}

//---------------------------------------------------------------------------//
// end of tstAbstractSerializer.cpp
//---------------------------------------------------------------------------//
