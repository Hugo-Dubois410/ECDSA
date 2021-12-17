//
//  main.cpp
//  ecdsa
//
//  Created by garzol on 06/01/2019.
//  Copyright © 2019 AA55. All rights reserved.
//

#include <iostream>
#include "curves.hpp"
#include "signature.hpp"

int main(int argc, const char * argv[]) {
    // insert code here...
    
    /*
     curve parameters :
     d = Edward curve with coefficient d
     p = curve over Fp
     X = abscissa of base point G
     */
    
    const mpz_class d("-67254");
    
    const mpz_class p("9850501549098619803069760025035903451269934817616361666987073351061430442874302652853566563721228910201656997576599");
    EdwardCurve C(d,p);

    mpz_class X("3914921414754292646847594472454013487047137431784830634731377862923477302047857640522480241298429278603678181725699");
    
    Point4 G = {X,17,1,(X*17)%p};
    
    std::string myString = "My message";
    const uint8_t *arr = reinterpret_cast<const uint8_t*>(&myString[0]);
    unsigned int length = (unsigned int)myString.size();
    mpz_class x,y, Kprv("2243892689661993117590870832367363359478653021822101739974986226109915499694279148746502635646805775474649269672401");
    Point4 KPub;
    C.mul(Kprv, G, KPub);
    sign(arr, length, x, y, Kprv);
    size_t size = (mpz_sizeinbase (x.get_mpz_t(), 2) + CHAR_BIT-1) / CHAR_BIT;
    uint8_t v[size];
    mpz_export(&v[0], &size, 1, 1, 0, 0, x.get_mpz_t());
    size_t size2 = (mpz_sizeinbase (y.get_mpz_t(), 2) + CHAR_BIT-1) / CHAR_BIT;
    uint8_t v2[size2];
    mpz_export(&v2[0], &size2, 1, 1, 0, 0, y.get_mpz_t());
    std::cout << arr << std::endl;
    std::cout << (char*)v << std::endl;
     std::cout << (char*)v2 << std::endl;
    if(check(arr, length, v, (unsigned int)size, v2, (unsigned int)size2,KPub))
        std::cout << "Correctly signed" << std::endl;
    else
        std::cout << "Corrupted data" << std::endl;
    return 0;
}
