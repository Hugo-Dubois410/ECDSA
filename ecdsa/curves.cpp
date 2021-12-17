//
//  curves.cpp
//  ecmpp
//
//  Created by garzol on 20/09/2018.
//  Copyright © 2018 AA55. All rights reserved.
//

#include "curves.hpp"

void EdwardCurve::add(Point4 &P,Point4 &Q)
{
    mpz_class A = (P.X*Q.X)%n;
    mpz_class B = (P.Y*Q.Y)%n;
    mpz_class C = (((P.T*d)%n)*Q.T)%n;
    mpz_class D = (P.Z*Q.Z)%n;
    mpz_class E = ((((P.X+P.Y)%n)*((Q.X+Q.Y)%n)%n)-A-B)%n;
    mpz_class F = (D-C)%n;
    mpz_class G = (D+C)%n;
    mpz_class H = (B-A)%n;
    P.X = (E*F)%n;
    P.Y = (G*H)%n;
    P.T = (E*H)%n;
    P.Z = (F*G)%n;
}

void EdwardCurve::double_(Point4 &P)
{
    mpz_class A = (P.X*P.X)%n;
    mpz_class B = (P.Y*P.Y)%n;
    mpz_class C = (2*(P.Z*P.Z))%n;
    mpz_class D = A;
    mpz_class I = P.X+P.Y;
    mpz_class E = ((I*I)-A-B)%n;
    mpz_class G = (D+B)%n;
    mpz_class F = (G-C)%n;
    mpz_class H = (D-B)%n;
    P.X = (E*F)%n;
    P.Y = (G*H)%n;
    P.T = (E*H)%n;
    P.Z = (F*G)%n;
}
void EdwardCurve::mul(unsigned long k, Point4& P ,Point4 &out)
{
    Point4 power_ = P;
    out = (Point4){.X = 0,.Y=1,.Z=1,.T=0};
    while(k>0){
        if(k & 1){
            add(out,power_);
        }
        k>>=1;
        double_(power_);
    }
    mpz_class zinv;
    mpz_invert(zinv.get_mpz_t(),out.Z.get_mpz_t(),n.get_mpz_t());
    out.X = (out.X*zinv)%n;
    out.Y = (out.Y*zinv)%n;
    out.Z = 1;
    out.T = (out.X*out.Y)%n;
}

void EdwardCurve::mul(mpz_class k, Point4& P ,Point4 &out)
{
    Point4 power_ = P;
    out = (Point4){.X = 0,.Y=1,.Z=1,.T=0};
    while(k>0){
        if(mpz_odd_p(k.get_mpz_t())){
            add(out,power_);
        }
        k>>=1;
        double_(power_);
    }
    mpz_class zinv;
    mpz_invert(zinv.get_mpz_t(),out.Z.get_mpz_t(),n.get_mpz_t());
    out.X = (out.X*zinv)%n;
    out.Y = (out.Y*zinv)%n;
    out.Z = 1;
    out.T = (out.X*out.Y)%n;
}

bool EdwardCurve::is_on(const mpz_class & x, const mpz_class & y)
{
    if(((x*x+y*y)-(1+d*x*x*y*y))%n == 0)
        return true;
    else
        return false;
}

bool EdwardCurve::is_infty(const mpz_class& x, const mpz_class& y)
{
    return (x==0 && ((y-1)%n)==0);
}
