// Copyright 2011 the V8 project authors. All rights reserved.
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
//     * Neither the name of Google Inc. nor the names of its
//       contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef V8_V8CONVERSIONS_H_
#define V8_V8CONVERSIONS_H_

#include "conversions.h"

namespace v8 {
namespace internal {


static inline bool IsMinusZero(double value) {
  static const DoubleRepresentation minus_zero(-0.0);
  return DoubleRepresentation(value) == minus_zero;
}


// Integer32 is an integer that can be represented as a signed 32-bit
// integer. It has to be in the range [-2^31, 2^31 - 1].
// We also have to check for negative 0 as it is not an Integer32.
static inline bool IsInt32Double(double value) {
  return !IsMinusZero(value) &&
         value >= kMinInt &&
         value <= kMaxInt &&
         value == FastI2D(FastD2I(value));
}


// Convert from Number object to C integer.
inline int32_t NumberToInt32(Object* number) {
  if (number->IsSmi()) return Smi::cast(number)->value();
  return DoubleToInt32(number->Number());
}


inline uint32_t NumberToUint32(Object* number) {
  if (number->IsSmi()) return Smi::cast(number)->value();
  return DoubleToUint32(number->Number());
}


double StringToDouble(UnicodeCache* unicode_cache,
                      String* string,
                      int flags,
                      double empty_string_val = 0.0);


inline bool TryNumberToSize(Isolate* isolate,
                            Object* number, size_t* result) {
  SealHandleScope shs(isolate);
  if (number->IsSmi()) {
    int value = Smi::cast(number)->value();
    ASSERT(static_cast<unsigned>(Smi::kMaxValue)
           <= std::numeric_limits<size_t>::max());
    if (value >= 0) {
      *result = static_cast<size_t>(value);
      return true;
    }
    return false;
  } else {
    ASSERT(number->IsHeapNumber());
    double value = HeapNumber::cast(number)->value();
    if (value >= 0 &&
        value <= std::numeric_limits<size_t>::max()) {
      *result = static_cast<size_t>(value);
      return true;
    } else {
      return false;
    }
  }
}

// Converts a number into size_t.
inline size_t NumberToSize(Isolate* isolate,
                           Object* number) {
  size_t result = 0;
  bool is_valid = TryNumberToSize(isolate, number, &result);
  CHECK(is_valid);
  return result;
}

} }  // namespace v8::internal

#endif  // V8_V8CONVERSIONS_H_
