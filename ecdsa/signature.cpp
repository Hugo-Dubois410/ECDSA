//
//  signature.cpp
//  ecdsa
//
//  Created by garzol on 06/01/2019.
//  Copyright © 2019 AA55. All rights reserved.
//

#include "signature.hpp"
#include "sha.h"
#include "curves.hpp"
#include <iostream>


void sign(const uint8_t *message_array,
           unsigned int length,mpz_class &x,mpz_class &y,const mpz_class & Kprv)
{
    uint8_t Message_Digest[SHA256HashSize];
    mpz_class p("9850501549098619803069760025035903451269934817616361666987073351061430442874302652853566563721228910201656997576599");
    EdwardCurve C(-67254,p);
    
    mpz_class Gx("3914921414754292646847594472454013487047137431784830634731377862923477302047857640522480241298429278603678181725699");
    
    mpz_class l("2462625387274654950767440006258975862817483704404090416745738034557663054564649171262659326683244604346084081047321");
    gmp_randclass r(gmp_randinit_default);
    r.seed(time(NULL));
    mpz_class k = r.get_z_range(l);
    Point4 G = {Gx, 17,1 , (Gx*17)%p};
    Point4 res;
    C.mul(k, G, res);
    if(res.X<0)
        res.X+=p;
    x = res.X%l;
    if(x==0)
    {
        sign(message_array,length,x,y,Kprv);
        return;
    }
    mpz_class kinv;
    mpz_invert(kinv.get_mpz_t(),k.get_mpz_t(),l.get_mpz_t());
    SHA256Context cont;
    SHA256Reset(&cont);
    if(SHA256Input(&cont, message_array, length)!=shaSuccess)
        return;
    if(SHA256Result(&cont, Message_Digest) != shaSuccess)
        return;
    mpz_class Hm;
    mpz_import(Hm.get_mpz_t(), SHA256HashSize, 1, sizeof(Message_Digest[0]), 0, 0, Message_Digest);
    y = (kinv*(Hm+Kprv*x))%l;
    if(y==0)
    {
        sign(message_array,length,x,y,Kprv);
        return;
    }
}

bool check(const uint8_t *message_array,
         unsigned int length,  const uint8_t *x_array,
         unsigned int xLength, const uint8_t *y_array,
         unsigned int yLength, Point4 &KPub)
{
    uint8_t Message_Digest[SHA256HashSize];
    
    mpz_class x,y;
    mpz_import(x.get_mpz_t(), xLength, 1, 1, 0, 0, x_array);
    mpz_import(y.get_mpz_t(), yLength, 1, 1, 0, 0, y_array);
    mpz_class p("9850501549098619803069760025035903451269934817616361666987073351061430442874302652853566563721228910201656997576599");
    EdwardCurve C(-67254,p);
    
    mpz_class Gx("3914921414754292646847594472454013487047137431784830634731377862923477302047857640522480241298429278603678181725699");
    
    mpz_class l("2462625387274654950767440006258975862817483704404090416745738034557663054564649171262659326683244604346084081047321");
    if(x<1 || x>=l || y<1 || y >= l)
        return false;
    if(C.is_infty(KPub.X, KPub.Y))
        return false;
    if(!C.is_on(KPub.X, KPub.Y))
        return false;
    Point4 O;
    C.mul(l, KPub, O);
    if(!C.is_infty(O.X, O.Y))
        return false;
    SHA256Context cont;
    SHA256Reset(&cont);
    if(SHA256Input(&cont, message_array, length)!=shaSuccess)
        return false;
    if(SHA256Result(&cont, Message_Digest) != shaSuccess)
        return false;
    mpz_class Hm;
    mpz_import(Hm.get_mpz_t(), SHA256HashSize, 1, sizeof(Message_Digest[0]), 0, 0, Message_Digest);
    mpz_class yinv;
    mpz_invert(yinv.get_mpz_t(),y.get_mpz_t(),l.get_mpz_t());
    mpz_class a = (Hm*yinv)%l, b = (x*yinv)%l;
    Point4 G = {Gx,17,1,(Gx*17)%p}, res1, res2;
    C.mul(a, G, res1);
    C.mul(b, KPub, res2);
    C.add(res1, res2);
    mpz_class zinv;
    mpz_invert(zinv.get_mpz_t(),res1.Z.get_mpz_t(),p.get_mpz_t());
    res1.X = (res1.X*zinv)%p;
    if(res1.X<0)
        res1.X+=p;
    if((res1.X%l)!= x)
        return false;
    else
        return true;
}
