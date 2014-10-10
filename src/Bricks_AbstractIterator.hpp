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
 * \brief Bricks_AbstractIterator.hpp
 * \author Stuart R. Slattery
 * \brief Abstract iterator interface.
 */
//---------------------------------------------------------------------------//

#ifndef Bricks_ABSTRACTITERATOR_HPP
#define Bricks_ABSTRACTITERATOR_HPP

#include <iterator>
#include <functional>

#include <Teuchos_RCP.hpp>

namespace Bricks
{
//---------------------------------------------------------------------------//
/*!
  \class AbstractIterator
  \brief Abstract iterator interface.

  This class provides a mechanism to iterate over a group of abstract objects
  with a specified predicate operation for selection.
*/
//---------------------------------------------------------------------------//
template<class ValueType>
class AbstractIterator : 
	public std::iterator<std::forward_iterator_tag,ValueType>
{
  public:

    //! The value type under the iterator.
    typedef ValueType value_type;

    /*!
     * \brief Constructor.
     */
    AbstractIterator();

    /*!
     * \brief Copy constructor.
     */
    AbstractIterator( const AbstractIterator<ValueType>& rhs );

    /*!
     * \brief Assignment operator.
     */
    AbstractIterator& 
    operator=( const AbstractIterator<ValueType>& rhs );

    /*!
     * \brief Destructor.
     */
    virtual ~AbstractIterator();

    // Pre-increment operator.
    virtual AbstractIterator<ValueType>& operator++();

    // Post-increment operator.
    virtual AbstractIterator<ValueType> operator++(int);

    // Dereference operator.
    virtual ValueType& operator*(void);

    // Dereference operator.
    virtual ValueType* operator->(void);

    // Equal comparison operator.
    virtual bool operator==( const AbstractIterator<ValueType>& rhs ) const;

    // Not equal comparison operator.
    virtual bool operator!=( const AbstractIterator<ValueType>& rhs ) const;

    // Number of elements in the iterator that meet the predicate criteria.
    virtual std::size_t size() const;

    // An iterator assigned to the first valid element in the iterator.
    virtual AbstractIterator<ValueType> begin() const;

    // An iterator assigned to the end of all elements under the iterator.
    virtual AbstractIterator<ValueType> end() const;

  protected:

    // Create a clone of the iterator. We need this for the copy constructor
    // and assignment operator to pass along the underlying implementation.
    virtual AbstractIterator<ValueType>* clone() const;

  private:

    // Advance the iterator to the first valid element that satisfies the
    // predicate or the end of the iterator.
    void advanceToFirstValidElement();

  protected:

    // Implementation.
    AbstractIterator<ValueType>* b_iterator_impl;

    // Predicate.
    std::function<bool(ValueType&)> b_predicate;
};

//---------------------------------------------------------------------------//

} // end namespace Bricks

//---------------------------------------------------------------------------//
// Template includes.
//---------------------------------------------------------------------------//

#include "Bricks_AbstractIterator_impl.hpp"

//---------------------------------------------------------------------------//

#endif // end Bricks_ABSTRACTITERATOR_HPP

//---------------------------------------------------------------------------//
// end Bricks_AbstractIterator.hpp
//---------------------------------------------------------------------------//
