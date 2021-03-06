/**************************************************************************/ /**
 *
 * \file
 * \brief Implementation of ContextImpl
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

#include <ikos/ar/semantic/bundle.hpp>
#include <ikos/ar/semantic/context.hpp>
#include <ikos/ar/semantic/function.hpp>
#include <ikos/ar/semantic/type.hpp>

#include "context_impl.hpp"

namespace ikos {
namespace ar {

ContextImpl::ContextImpl()
    : _ui1_ty(1, Unsigned),
      _ui8_ty(8, Unsigned),
      _ui16_ty(16, Unsigned),
      _ui32_ty(32, Unsigned),
      _ui64_ty(64, Unsigned),
      _si1_ty(1, Signed),
      _si8_ty(8, Signed),
      _si16_ty(16, Signed),
      _si32_ty(32, Signed),
      _si64_ty(64, Signed),
      _half_ty(16, Half),
      _float_ty(32, Float),
      _double_ty(64, Double),
      _x86_fp80_ty(80, X86_FP80),
      _fp128_ty(128, FP128),
      _ppc_fp128_ty(128, PPC_FP128) {}

ContextImpl::~ContextImpl() = default;

void ContextImpl::add_bundle(std::unique_ptr< Bundle > bundle) {
  this->_bundles.emplace_back(std::move(bundle));
}

IntegerType* ContextImpl::integer_type(unsigned bit_width, Signedness sign) {
  auto it = this->_integer_types.find(std::make_pair(bit_width, sign));
  if (it == this->_integer_types.end()) {
    auto type = new IntegerType(bit_width, sign);
    this->_integer_types.emplace(std::make_pair(bit_width, sign),
                                 std::unique_ptr< IntegerType >(type));
    return type;
  } else {
    return it->second.get();
  }
}

PointerType* ContextImpl::pointer_type(Type* pointee) {
  auto it = this->_pointer_types.find(pointee);
  if (it == this->_pointer_types.end()) {
    auto type = new PointerType(pointee);
    this->_pointer_types.emplace(pointee, std::unique_ptr< PointerType >(type));
    return type;
  } else {
    return it->second.get();
  }
}

ArrayType* ContextImpl::array_type(Type* element_type, ZNumber num_element) {
  auto it = this->_array_types.find(std::make_pair(element_type, num_element));
  if (it == this->_array_types.end()) {
    auto type = new ArrayType(element_type, num_element);
    this->_array_types.emplace(std::make_pair(element_type, num_element),
                               std::unique_ptr< ArrayType >(type));
    return type;
  } else {
    return it->second.get();
  }
}

VectorType* ContextImpl::vector_type(Type* element_type, ZNumber num_element) {
  auto it = this->_vector_types.find(std::make_pair(element_type, num_element));
  if (it == this->_vector_types.end()) {
    auto type = new VectorType(element_type, num_element);
    this->_vector_types.emplace(std::make_pair(element_type, num_element),
                                std::unique_ptr< VectorType >(type));
    return type;
  } else {
    return it->second.get();
  }
}

FunctionType* ContextImpl::function_type(
    Type* return_type,
    const FunctionType::ParamTypes& param_types,
    bool is_var_arg) {
  auto it = this->_function_types.find(
      std::make_tuple(return_type, param_types, is_var_arg));
  if (it == this->_function_types.end()) {
    auto type = new FunctionType(return_type, param_types, is_var_arg);
    this->_function_types.emplace(std::make_tuple(return_type,
                                                  param_types,
                                                  is_var_arg),
                                  std::unique_ptr< FunctionType >(type));
    return type;
  } else {
    return it->second.get();
  }
}

void ContextImpl::add_type(std::unique_ptr< Type > type) {
  this->_types.emplace_back(std::move(type));
}

UndefinedConstant* ContextImpl::undefined_cst(Type* type) {
  auto it = this->_undefined_constants.find(type);
  if (it == this->_undefined_constants.end()) {
    auto cst = new UndefinedConstant(type);
    this->_undefined_constants.emplace(type,
                                       std::unique_ptr< UndefinedConstant >(
                                           cst));
    return cst;
  } else {
    return it->second.get();
  }
}

IntegerConstant* ContextImpl::integer_cst(IntegerType* type, MachineInt value) {
  auto it = this->_integer_constants.find(std::make_pair(type, value));
  if (it == this->_integer_constants.end()) {
    auto cst = new IntegerConstant(type, value);
    this->_integer_constants.emplace(std::make_pair(type, value),
                                     std::unique_ptr< IntegerConstant >(cst));
    return cst;
  } else {
    return it->second.get();
  }
}

FloatConstant* ContextImpl::float_cst(FloatType* type,
                                      const std::string& value) {
  auto it = this->_float_constants.find(std::make_pair(type, value));
  if (it == this->_float_constants.end()) {
    auto cst = new FloatConstant(type, value);
    this->_float_constants.emplace(std::make_pair(type, value),
                                   std::unique_ptr< FloatConstant >(cst));
    return cst;
  } else {
    return it->second.get();
  }
}

NullConstant* ContextImpl::null_cst(PointerType* type) {
  auto it = this->_null_constants.find(type);
  if (it == this->_null_constants.end()) {
    auto cst = new NullConstant(type);
    this->_null_constants.emplace(type, std::unique_ptr< NullConstant >(cst));
    return cst;
  } else {
    return it->second.get();
  }
}

StructConstant* ContextImpl::struct_cst(StructType* type,
                                        const StructConstant::Values& values) {
  auto it = this->_struct_constants.find(std::make_pair(type, values));
  if (it == this->_struct_constants.end()) {
    auto cst = new StructConstant(type, values);
    this->_struct_constants.emplace(std::make_pair(type, values),
                                    std::unique_ptr< StructConstant >(cst));
    return cst;
  } else {
    return it->second.get();
  }
}

ArrayConstant* ContextImpl::array_cst(ArrayType* type,
                                      const ArrayConstant::Values& values) {
  auto it = this->_array_constants.find(std::make_pair(type, values));
  if (it == this->_array_constants.end()) {
    auto cst = new ArrayConstant(type, values);
    this->_array_constants.emplace(std::make_pair(type, values),
                                   std::unique_ptr< ArrayConstant >(cst));
    return cst;
  } else {
    return it->second.get();
  }
}

VectorConstant* ContextImpl::vector_cst(VectorType* type,
                                        const VectorConstant::Values& values) {
  auto it = this->_vector_constants.find(std::make_pair(type, values));
  if (it == this->_vector_constants.end()) {
    auto cst = new VectorConstant(type, values);
    this->_vector_constants.emplace(std::make_pair(type, values),
                                    std::unique_ptr< VectorConstant >(cst));
    return cst;
  } else {
    return it->second.get();
  }
}

AggregateZeroConstant* ContextImpl::aggregate_zero_cst(AggregateType* type) {
  auto it = this->_aggregate_zero_constants.find(type);
  if (it == this->_aggregate_zero_constants.end()) {
    auto cst = new AggregateZeroConstant(type);
    this->_aggregate_zero_constants
        .emplace(type, std::unique_ptr< AggregateZeroConstant >(cst));
    return cst;
  } else {
    return it->second.get();
  }
}

FunctionPointerConstant* ContextImpl::function_pointer_cst(Function* function) {
  auto it = this->_function_pointer_constants.find(function);
  if (it == this->_function_pointer_constants.end()) {
    ikos_assert_msg(function, "function is null");
    PointerType* fun_ptr_type = this->pointer_type(function->type());
    auto cst = new FunctionPointerConstant(fun_ptr_type, function);
    this->_function_pointer_constants
        .emplace(function, std::unique_ptr< FunctionPointerConstant >(cst));
    return cst;
  } else {
    return it->second.get();
  }
}

InlineAssemblyConstant* ContextImpl::inline_assembly_cst(
    PointerType* type, const std::string& code) {
  auto it = this->_inline_assembly_constants.find(std::make_pair(type, code));
  if (it == this->_inline_assembly_constants.end()) {
    auto cst = new InlineAssemblyConstant(type, code);
    this->_inline_assembly_constants
        .emplace(std::make_pair(type, code),
                 std::unique_ptr< InlineAssemblyConstant >(cst));
    return cst;
  } else {
    return it->second.get();
  }
}

} // end namespace ar
} // end namespace ikos
