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
 * \brief Bricks_AbstractSerializableObject_impl.hpp
 * \author Stuart R. Slattery
 * \brief Serializable abstract object interface.
 */
//---------------------------------------------------------------------------//

#ifndef Bricks_ABSTRACTSERIALIZABLEOBJECT_IMPL_HPP
#define Bricks_ABSTRACTSERIALIZABLEOBJECT_IMPL_HPP

#include <string>

#include "Bricks_DBC.hpp"
#include "Bricks_AbstractBuilder.hpp"

#include <Teuchos_RCP.hpp>
#include <Teuchos_ArrayView.hpp>

namespace Bricks
{
//---------------------------------------------------------------------------//
// Constructor.
template<class Object>
AbstractSerializableObject<Object>::AbstractSerializableObject()
{ /* ... */ }

//---------------------------------------------------------------------------//
//brief Destructor.
template<class Object>
AbstractSerializableObject<Object>::~AbstractSerializableObject()
{ /* ... */ }

//---------------------------------------------------------------------------//
// Static members.
//---------------------------------------------------------------------------//
// Byte size.
template<class Object>
std::size_t AbstractSerializableObject<Object>::b_max_byte_size = 0;

//---------------------------------------------------------------------------//
// Get the maximum byte size of subclasses of the given base class.
template<class Object>
std::size_t AbstractSerializableObject<Object>::maxByteSize()
{ 
    return b_max_byte_size;
}

//---------------------------------------------------------------------------//
// Set the byte size of a derived class with the base class.
template<class Object>
void AbstractSerializableObject<Object>::setDerivedClassByteSize(
    const std::size_t bytes )
{
    b_max_byte_size = std::max( b_max_byte_size, bytes );
}

//---------------------------------------------------------------------------//
// Set the byte size of a derived class with the base class for a derived
// class implementing the DerivedSerializableObjectPolicy.
template<class Object>
template<class DerivedObject>
void AbstractSerializableObject<Object>::setDerivedClassByteSize()
{
    setDerivedClassByteSize(
	DerivedSerializableObjectPolicy<DerivedObject>::byteSize() );
}

//---------------------------------------------------------------------------//

} // end namespace Bricks

//---------------------------------------------------------------------------//

#endif // end Bricks_ABSTRACTSERIALIZABLEOBJECT_IMPL_HPP

//---------------------------------------------------------------------------//
// end Bricks_AbstractSerializableObject_imple.hpp
//---------------------------------------------------------------------------//
