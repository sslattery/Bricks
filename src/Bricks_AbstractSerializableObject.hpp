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
 * \brief Bricks_AbstractSerializableObject.hpp
 * \author Stuart R. Slattery
 * \brief Serializable abstract object interface.
 */
//---------------------------------------------------------------------------//

#ifndef Bricks_ABSTRACTSERIALIZABLEOBJECT_HPP
#define Bricks_ABSTRACTSERIALIZABLEOBJECT_HPP

#include <string>

#include "Bricks_DBC.hpp"
#include "Bricks_AbstractBuilder.hpp"

#include <Teuchos_RCP.hpp>
#include <Teuchos_ArrayView.hpp>

namespace Bricks
{
//---------------------------------------------------------------------------//
/*!
  \class UndefinedDerivedSerializableObjectPolicy
  \brief Complie time error indicator for policy implementations.
*/
//---------------------------------------------------------------------------//
template<typename T>
struct UndefinedDerivedSerializableObjectPolicy 
{
    static inline T notDefined() 
    {
	return T::this_type_is_missing_a_specialization();
    }
};

//---------------------------------------------------------------------------//
/*!
  \class DerivedSerializableObjectPolicy
  \brief Policy definition for objects that can be serialized.

  This class provides a runtime mechanism to serialize a derived class of
  arbitrary size through a base class interface and deserialize the base class
  with the correct underlying derived class.
*/
//---------------------------------------------------------------------------//
template<class T>
class DerivedSerializableObjectPolicy
{
  public:

    //! Base class type.
    typedef T object_type;

    //@{
    //! Serialization functions.
    /*!
     * \brief Get the byte size of derived class.
     * \return The byte size of the derived class.
     */
    static std::size_t byteSize()
    {
	UndefinedDerivedSerializableObjectPolicy<T>::notDefined();
	return 0;
    }
};

//---------------------------------------------------------------------------//
/*!
  \class UndefinedAbstractSerializableObjectPolicy
  \brief Complie time error indicator for policy implementations.
*/
//---------------------------------------------------------------------------//
template<typename T>
struct UndefinedAbstractSerializableObjectPolicy 
{
    static inline T notDefined() 
    {
	return T::this_type_is_missing_a_specialization();
    }
};

//---------------------------------------------------------------------------//
/*!
  \class AbstractSerializableObjectPolicy
  \brief Policy definition for objects that can be serialized.

  This class provides a runtime mechanism to serialize a derived class of
  arbitrary size through a base class interface and deserialize the base class
  with the correct underlying derived class.
*/
//---------------------------------------------------------------------------//
template<class T>
class AbstractSerializableObjectPolicy
{
  public:

    //! Base class type.
    typedef T object_type;

    //@{
    //! Implementation check functions.
    /*! Check that there is an underlying and non-null implementation. If
     *  there is, then this object can be serialized.
     * \param object Check that the implementation under this object is
     *  nonnull.
     */
    static bool objectHasImplementation( const T& object )
    {
	UndefinedAbstractSerializableObjectPolicy<T>::notDefined();
	return 0;
    }
    //@}

    //@{
    //! Serialization functions.
    /*!
     * \brief Get the maximum byte size of subclasses of the given base
     * class.
     * \return The maximum byte size of subclasses of the given base class.
     */
    static std::size_t maxByteSize()
    {
	UndefinedAbstractSerializableObjectPolicy<T>::notDefined();
	return 0;
    }

    /*
     * \brief Serialize the subclass into a buffer.
     * \param object Serialize this object into the buffer.
     * \param buffer A view into a data buffer of size byteSize(). Write the
     * serialized subclass into this view.
     */
    static void serialize( const T& object,
			   const Teuchos::ArrayView<char>& buffer )
    {
	UndefinedAbstractSerializableObjectPolicy<T>::notDefined();
    }

    /*!
     * \brief Deserialize a subclass from a buffer.
     * \param object Deserialze the buffer to construct this object.
     * \param buffer A view into a data buffer of size byteSize(). Deserialize
     * the object from this view.
     */
    static void deserialize( T& object,
			     const Teuchos::ArrayView<const char>& buffer )
    {
	UndefinedAbstractSerializableObjectPolicy<T>::notDefined();
    }
    //@}
};

//---------------------------------------------------------------------------//
/*!
  \class AbstractSerializableObject
  \brief Interface definition for objects that can be serialized.

  This class provides a static mechanism to track the byte size of a base
  class as the maximum size of its derived classes. Derived classes must
  register with this class.
*/
//---------------------------------------------------------------------------//
template<class Object>
class AbstractSerializableObject
{
  public:

    //! Base class type.
    typedef Object object_type;

    /*!
     * \brief Constructor.
     */
    AbstractSerializableObject();

    /*!
     * \brief Destructor.
     */
    virtual ~AbstractSerializableObject();

    /*!
     * \brief Get the maximum byte size of subclasses of the given base
     * class.
     * \return The maximum byte size of subclasses of the given base class.
     */
    static std::size_t maxByteSize();

    /*
     * \brief Set the byte size of a derived class with the base class for a
     * derived class implementing the DerivedSerializableObjectPolicy.
     * \param byte_size The byte size of the derived class.
     */
    template<class DerivedObject>
    static void setDerivedClassByteSize();

    /*
     * \brief Set the byte size of a derived class with the base class.
     * \param byte_size The byte size of the derived class.
     */
    static void setDerivedClassByteSize( const std::size_t bytes );

  private:

    // Maximum byte size for the base class.
    static std::size_t b_max_byte_size;
};

//---------------------------------------------------------------------------//

} // end namespace Bricks

//---------------------------------------------------------------------------//
// Template includes.
//---------------------------------------------------------------------------//

#include "Bricks_AbstractSerializableObject_impl.hpp"

//---------------------------------------------------------------------------//

#endif // end Bricks_ABSTRACTSERIALIZABLEOBJECT_HPP

//---------------------------------------------------------------------------//
// end Bricks_AbstractSerializableObject.hpp
//---------------------------------------------------------------------------//
