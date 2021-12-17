//
//  signature.hpp
//  ecdsa
//
//  Created by garzol on 06/01/2019.
//  Copyright © 2019 AA55. All rights reserved.
//

#ifndef signature_hpp
#define signature_hpp

#include <stdio.h>
#include "curves.hpp"
#include "sha.h"

void sign(const uint8_t *message_array,
          unsigned int length,mpz_class &x,mpz_class &y,const mpz_class & Kprv);

bool check(const uint8_t *message_array,
           unsigned int length,  const uint8_t *x_array,
           unsigned int xLength, const uint8_t *y_array,
           unsigned int yLength, Point4 &KPub);

#endif /* signature_hpp */
