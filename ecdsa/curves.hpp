//
//  curves.hpp
//  ecmpp
//
//  Created by garzol on 20/09/2018.
//  Copyright © 2018 AA55. All rights reserved.
//

#ifndef curves_hpp
#define curves_hpp

#include <gmpxx.h>
#include <vector>

typedef struct
{
    mpz_class X, Y, Z,T;
} Point4;

class EdwardCurve{
public:
    mpz_class d,n;
    EdwardCurve(const mpz_class &a,const mpz_class &m) : d(a), n(m){}
    ~EdwardCurve() {}
    void add(Point4 &, Point4 &);
    void double_(Point4 &);
    void mul(unsigned long ,  Point4& ,Point4 &out);
    void mul(mpz_class ,  Point4& ,Point4 &out);
    bool is_on(const mpz_class&,const mpz_class&);
    bool is_infty(const mpz_class&, const mpz_class&);
};



#endif /* curves_hpp */
