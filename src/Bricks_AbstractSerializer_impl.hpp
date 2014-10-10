//---------------------------------------------------------------------------//
/*
  Copyright (c) 2014, Stuart R. Slattery
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are
  met:

  *: Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.

  *: Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.

  *: Neither the name of the Oak Ridge National Laboratory nor the
  names of its contributors may be used to endorse or promote products
  derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
//---------------------------------------------------------------------------//
/*!
 * \brief Bricks_AbstractSerializer_impl.hpp
 * \author Stuart R. Slattery
 * \brief Serializable object policy interface.
 */
//---------------------------------------------------------------------------//

#ifndef Bricks_ABSTRACTSERIALIZER_IMPL_HPP
#define Bricks_ABSTRACTSERIALIZER_IMPL_HPP

#include <string>

#include "Bricks_DBC.hpp"
#include "Bricks_AbstractBuilder.hpp"
#include <Teuchos_DefaultComm.hpp>

namespace Bricks
{
//---------------------------------------------------------------------------//
/*!
  \class AbstractSerializer
  \brief Serializer for abstract objects.
*/
//---------------------------------------------------------------------------//
//! This is an indirect serializer.
template<class Ordinal, class T>
const bool AbstractSerializer<Ordinal,T>::supportsDirectSerialization = false;

//---------------------------------------------------------------------------//
// Return the number of bytes for count objects.
template<class Ordinal, class T>
Ordinal AbstractSerializer<Ordinal,T>::fromCountToIndirectBytes( 
    const Ordinal count, const T buffer[])
{
    return count * ( sizeof(int) + ASOP::maxByteSize() );
}

//---------------------------------------------------------------------------//
// Serialize to an indirect char buffer.
template<class Ordinal, class T>
void AbstractSerializer<Ordinal,T>::serialize( const Ordinal count, 
					       const T buffer[], 
					       const Ordinal bytes, 
					       char charBuffer[] )
{
    // Get the builder for the objects.
    Teuchos::RCP<AbstractBuilder<T> > builder = ABOP::getBuilder();

    // Serialize the objects.
    Bricks_REQUIRE( fromCountToIndirectBytes(count,buffer) == bytes );
    char* buffer_pos = &charBuffer[0];
    int integral_key = 0;
    for ( Ordinal i = 0; i < count; ++i )
    {
	// Get a view of the buffer.
	Teuchos::ArrayView<char> buffer_view( 
	    buffer_pos, sizeof(int) + ASOP::maxByteSize() );

	// Fill the buffer with zeros to start.
	std::fill( buffer_view.begin(), buffer_view.end(), '0' );

	// Only serialize objects that have an underlying implementation.
	if ( ASOP::objectHasImplementation(buffer[i]) )
	{
	    // Serialize the integral key of the object.
	    integral_key = builder->getIntegralKey( ABOP::objectType(buffer[i]) );
	    std::memcpy( buffer_pos, &integral_key, sizeof(int) );

	    // Serialize the object.
	    ASOP::serialize( 
		buffer[i], buffer_view(sizeof(int), ASOP::maxByteSize()) );
	}

	// Move the front of the buffer forward.
	buffer_pos += sizeof(int) + ASOP::maxByteSize();
    }
    Bricks_ENSURE( &charBuffer[0] + bytes == buffer_pos );
}

//---------------------------------------------------------------------------//
// Return the number of objects for bytes of storage.
template<class Ordinal, class T>
Ordinal AbstractSerializer<Ordinal,T>::fromIndirectBytesToCount( 
    const Ordinal bytes, const char charBuffer[] )
{
    return bytes / ( sizeof(int) + ASOP::maxByteSize() );
}

//---------------------------------------------------------------------------//
// Deserialize from an indirect char buffer.
template<class Ordinal, class T>
void AbstractSerializer<Ordinal,T>::deserialize( const Ordinal bytes, 
						 const char charBuffer[], 
						 const Ordinal count, 
						 T buffer[] )
{
    // Get the builder for the objects.
    Teuchos::RCP<AbstractBuilder<T> > builder = ABOP::getBuilder();

    // Deserialize the objects.
    Bricks_REQUIRE( fromIndirectBytesToCount(bytes,charBuffer) == count );
    char* buffer_pos = const_cast<char*>(&charBuffer[0]);
    int integral_key = 0;
    for ( Ordinal i = 0; i < count; ++i )
    {
	// Get the integral key for the object.
	std::memcpy( &integral_key, buffer_pos, sizeof(int) );
	buffer_pos += sizeof(int);

	// Create an object of the correct derived class.
	buffer[i] = *( builder->create(integral_key) );
	Bricks_CHECK( ASOP::objectHasImplementation(buffer[i]) );

	// Deserialize the object.
	Teuchos::ArrayView<char> buffer_view( buffer_pos, ASOP::maxByteSize() );
	ASOP::deserialize( buffer[i], buffer_view );

	// Move the front of the buffer forward.
	buffer_pos += ASOP::maxByteSize();
    }
    Bricks_ENSURE( &charBuffer[0] + bytes == buffer_pos );
}

//---------------------------------------------------------------------------//

} // end namespace Bricks

#endif // end Bricks_ABSTRACTSERIALIZER_IMPL_HPP

//---------------------------------------------------------------------------//
// end Bricks_AbstractSerializer_impl.hpp
//---------------------------------------------------------------------------//
