#ifndef __BIT_ARRAY_H__
#define __BIT_ARRAY_H__

/*
 *  BitArray.h
 *  zxing
 *
 *  Created by Christian Brunschen on 09/05/2008.
 *  Copyright 2008 Google UK. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Counted.h"
//#include "IllegalArgumentException.h"
#include <valarray>
#include <limits>
//#include <iostream>

using namespace std;

namespace common {
  class BitArray : public Counted {
  private:
    size_t size_;
    valarray<unsigned int> bits_;
    static const unsigned int bitsPerWord_;
    static const unsigned int logBits_;
    static const unsigned int bitsMask_;
    static size_t wordsForBits(size_t bits);
    explicit BitArray();
    
  public:
    BitArray(size_t size);
    ~BitArray();
    size_t getSize();
    bool get(size_t i);
    void set(size_t i);
    void setBulk(size_t i, unsigned int newBits);
    void clear();
    bool isRange(size_t start, size_t end, bool value);
    valarray<unsigned int>& getBitArray();
    void reverse();
  };
}

#endif // __BIT_ARRAY_H__
