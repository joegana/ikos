/**************************************************************************/ /**
 *
 * \file
 * \brief Traceable class definition
 *
 * Author: Maxime Arthaud
 *
 * Contact: ikos@lists.nasa.gov
 *
 * Notices:
 *
 * Copyright (c) 2017-2018 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All Rights Reserved.
 *
 * Disclaimers:
 *
 * No Warranty: THE SUBJECT SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY OF
 * ANY KIND, EITHER EXPRESSED, IMPLIED, OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL CONFORM TO SPECIFICATIONS,
 * ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE,
 * OR FREEDOM FROM INFRINGEMENT, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL BE
 * ERROR FREE, OR ANY WARRANTY THAT DOCUMENTATION, IF PROVIDED, WILL CONFORM TO
 * THE SUBJECT SOFTWARE. THIS AGREEMENT DOES NOT, IN ANY MANNER, CONSTITUTE AN
 * ENDORSEMENT BY GOVERNMENT AGENCY OR ANY PRIOR RECIPIENT OF ANY RESULTS,
 * RESULTING DESIGNS, HARDWARE, SOFTWARE PRODUCTS OR ANY OTHER APPLICATIONS
 * RESULTING FROM USE OF THE SUBJECT SOFTWARE.  FURTHER, GOVERNMENT AGENCY
 * DISCLAIMS ALL WARRANTIES AND LIABILITIES REGARDING THIRD-PARTY SOFTWARE,
 * IF PRESENT IN THE ORIGINAL SOFTWARE, AND DISTRIBUTES IT "AS IS."
 *
 * Waiver and Indemnity:  RECIPIENT AGREES TO WAIVE ANY AND ALL CLAIMS AGAINST
 * THE UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL
 * AS ANY PRIOR RECIPIENT.  IF RECIPIENT'S USE OF THE SUBJECT SOFTWARE RESULTS
 * IN ANY LIABILITIES, DEMANDS, DAMAGES, EXPENSES OR LOSSES ARISING FROM SUCH
 * USE, INCLUDING ANY DAMAGES FROM PRODUCTS BASED ON, OR RESULTING FROM,
 * RECIPIENT'S USE OF THE SUBJECT SOFTWARE, RECIPIENT SHALL INDEMNIFY AND HOLD
 * HARMLESS THE UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS,
 * AS WELL AS ANY PRIOR RECIPIENT, TO THE EXTENT PERMITTED BY LAW.
 * RECIPIENT'S SOLE REMEDY FOR ANY SUCH MATTER SHALL BE THE IMMEDIATE,
 * UNILATERAL TERMINATION OF THIS AGREEMENT.
 *
 ******************************************************************************/

#pragma once

#include <type_traits>
#include <typeinfo>

#include <ikos/ar/support/assert.hpp>

namespace ikos {
namespace ar {

/// \brief Add traceability to derived classes
///
/// Add the ability to hold a pointer to a front-end object.
///
/// This can be used for instance to attach debug information to an object.
///
/// Note that Traceable holds a non-owning pointer, so you have to make sure
/// the pointee object is still alive before calling frontend().
class Traceable {
private:
  // Pointer to front-end object, or null
  void* _frontend = nullptr;

#ifndef NDEBUG
  // Front-end object type info, or null
  const std::type_info* _frontend_type_info = nullptr;
#endif

public:
  /// \brief Default constructor
  Traceable() = default;

  /// \brief Constructor taking a pointer to a front-end object
  template <
      typename T,
      class = std::enable_if_t< !std::is_base_of< Traceable, T >::value > >
  explicit Traceable(T* frontend) : _frontend(static_cast< void* >(frontend)) {
#ifndef NDEBUG
    this->_frontend_type_info = &typeid(T);
#endif
  }

  /// \brief Default copy constructor
  Traceable(const Traceable&) = default;

  /// \brief Default move constructor
  Traceable(Traceable&&) noexcept = default;

  /// \brief Default copy assignment operator
  Traceable& operator=(const Traceable&) = default;

  /// \brief Default move assignment operator
  Traceable& operator=(Traceable&&) noexcept = default;

  /// \brief Destructor
  ~Traceable() = default;

  /// \brief Update the pointer to the front-end object
  template <
      typename T,
      class = std::enable_if_t< !std::is_base_of< Traceable, T >::value > >
  void set_frontend(T* frontend) {
    this->_frontend = static_cast< void* >(frontend);
#ifndef NDEBUG
    this->_frontend_type_info = &typeid(T);
#endif
  }

  /// \brief Update the pointer to the front-end object
  void set_frontend(const Traceable& o) {
    this->_frontend = o._frontend;
#ifndef NDEBUG
    this->_frontend_type_info = o._frontend_type_info;
#endif
  }

  /// \brief Return true if this object has a pointer to a front-end object
  bool has_frontend() const { return this->_frontend != nullptr; }

  /// \brief Return the pointer to a front-end object
  ///
  /// Precondition: has_frontend() is true
  template < typename T >
  T* frontend() const {
    ikos_assert_msg(this->_frontend, "no front-end pointer");
#ifndef NDEBUG
    ikos_assert_msg(*this->_frontend_type_info == typeid(T),
                    "invalid front-end type");
#endif
    return reinterpret_cast< T* >(this->_frontend); // NOLINT
  }

  /// \brief Return the pointer to the front-end object, or null
  template < typename T >
  T* frontend_or_null() const {
#ifndef NDEBUG
    ikos_assert_msg(this->_frontend == nullptr ||
                        *this->_frontend_type_info == typeid(T),
                    "invalid front-end type");
#endif
    return reinterpret_cast< T* >(this->_frontend); // NOLINT
  }

}; // end class Traceable

} // end namespace ar
} // end namespace ikos
