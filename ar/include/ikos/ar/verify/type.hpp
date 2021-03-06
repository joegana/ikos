/**************************************************************************/ /**
 *
 * \file
 * \brief Type checker for the abstract representation
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

#include <iosfwd>

#include <ikos/ar/semantic/bundle.hpp>
#include <ikos/ar/semantic/code.hpp>
#include <ikos/ar/semantic/function.hpp>
#include <ikos/ar/semantic/statement.hpp>
#include <ikos/ar/semantic/value.hpp>

namespace ikos {
namespace ar {

/// \brief Type checker
class TypeVerifier {
private:
  // Find all errors, do not stop at the first one
  bool _all;

public:
  /// \brief Public constructor
  ///
  /// \param all Find all errors, do not stop at the first one
  explicit TypeVerifier(bool all = true) : _all(all) {}

  /// \brief Default copy constructor
  TypeVerifier(const TypeVerifier&) = default;

  /// \brief Default move constructor
  TypeVerifier(TypeVerifier&&) noexcept = default;

  /// \brief Default copy assignment operator
  TypeVerifier& operator=(const TypeVerifier&) = default;

  /// \brief Default move assignment operator
  TypeVerifier& operator=(TypeVerifier&&) noexcept = default;

  /// \brief Destructor
  ~TypeVerifier() = default;

  /// \brief Type check the given bundle
  ///
  /// \param err The output stream for errors
  bool verify(Bundle* bundle, std::ostream& err) const;

  /// \brief Type check the given global variable
  ///
  /// \param err The output stream for errors
  bool verify(GlobalVariable* gv, std::ostream& err) const;

  /// \brief Type check the given function
  ///
  /// \param err The output stream for errors
  bool verify(Function* f, std::ostream& err) const;

  /// \brief Type check the given code
  ///
  /// \param err The output stream for errors
  /// \param return_type For a function body, the returned type, otherwise null
  bool verify(Code* code, std::ostream& err, Type* return_type = nullptr) const;

  /// \brief Type check the given basic block
  ///
  /// \param err The output stream for errors
  /// \param return_type For a function body, the returned type, otherwise null
  bool verify(BasicBlock* bb,
              std::ostream& err,
              Type* return_type = nullptr) const;

public:
  // Check if there is an implicit bitcast between two types.
  //
  // An implicit bitcast is between:
  //  - integer types of the same bit-width (i.e, signed <-> unsigned)
  //  - pointer types (i.e, A* <-> B*)
  static bool is_implicit_bitcast(Type* a, Type* b) {
    return (a->is_integer() && b->is_integer() &&
            cast< IntegerType >(a)->bit_width() ==
                cast< IntegerType >(b)->bit_width()) ||
           (a->is_pointer() && b->is_pointer());
  }

  /// \brief Return true if the given call statement is a valid function call to
  /// the given function type.
  ///
  /// Checks the return type and parameter types.
  /// Does not check the called value type.
  static bool is_valid_call(ar::CallBase* call, ar::FunctionType* fun_ty);

}; // end class TypeVerifier

} // end namespace ar
} // end namespace ikos
