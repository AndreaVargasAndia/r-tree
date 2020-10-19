#include <iostream>
#include <math.h>
#include <algorithm>

#ifndef RTREE_H
#define RTREE_H
#define C 3

using namespace std;

struct Point{
    float x,y;
};

struct Rectangulo{
    Point v_1,v_2;
};

class Nodo{
public:
    Rectangulo region;
    Rectangulo rg[C];
    Nodo* hijos[C];
    int r,h;
    Nodo(){
        for(int i=0;i<C;++i)
        {
            hijos[i]=0;
        }
        r=0;
        h=0;
    };
};


class RTree{
    public:
    Nodo* root;
    
    void insert(Rectangulo r)
    {
        Nodo* nd=choose_leaf(root,r);
        if(nd->r<C-1)
        {
            nd->rg[nd->r]=r;
            nd->r++;
        }
        else
        {
            Rectangulo* r1=0;
            Rectangulo* r2=0;
            pcik_seeds(nd,r1,r2);
            Rectangulo rr1[C/2+1];
            Rectangulo rr2[C/2+1];
            int c_1=0,c_2=0;
            for(int i=0;i<nd->r;++i){
                if(pick_next(nd->rg[i],(*r1),(*r2)))
                {
                    if(c_2<C/2+1){
                        rr2[c_2]=nd->rg[i];
                        c_2++;
                    }
                    else{
                        rr1[c_1]=nd->rg[i];
                        c_1++;
                    }
                }
                else{
                    if(c_1<C/2+1){
                        rr1[c_1]=nd->rg[i];
                        c_1++;
                    }
                    else{
                        rr2[c_2]=nd->rg[i];
                        c_2++;
                    }
                }
            }

        }
    }
    
    int Area(Rectangulo r1,Rectangulo r2)
    {
        int x,y;
        if(r2.v_1.x<r1.v_1.x)
            x=r1.v_2.x-r2.v_1.x;
        else{
            if(r2.v_2.x>r1.v_2.x)
                x=r2.v_2.x-r1.v_1.x;
            else
                x=r1.v_2.x-r1.v_1.x;
        }
        if(r2.v_1.y>r1.v_1.y)
            y=r2.v_1.y-r1.v_2.y;
        else{
            if(r2.v_2.y<r1.v_2.y)
                y=r1.v_1.y-r2.v_2.y;
            else
                y=r1.v_1.y-r1.v_2.y;
        }
        return x*y;
    }
    
    Nodo* choose_leaf(Nodo* nd,Rectangulo r)
    {
        if(!nd||!nd->h)
        {
            return nd;
        }
        int min_area=1<<30;
        int area_,indx=0;
        
        for(int i=0;i<nd->h;++i)
        {
            area_=Area(nd->hijos[i]->region,r);
            if(area_<min_area)
            {
                min_area=area_;
                indx=i;
            }
        }
        return choose_leaf(nd->hijos[indx],r);
        
    }
    
    void pcik_seeds(Nodo* nd,Rectangulo* r1,Rectangulo* r2)
    {
        float d1,d2,d,d_max=0;
        Rectangulo* tmp1;
        Rectangulo* tmp2;
        for(int i=0;i<C-1;++i)
        {
            tmp1=&(nd->rg[i]);
            for(int j=1;j<C;++j)
            {
                tmp2=&(nd->rg[j]);
                d1=abs(r1->v_1.x-r2->v_2.x);
                d2=abs(r2->v_1.x-r1->v_2.x);
                d=max(d1,d2);
                if(d > d_max)
                {
                    r1=tmp1;
                    r2=tmp2;
                }
            }
        }
    }
    bool pick_next(Rectangulo rp,Rectangulo r1,Rectangulo r2)
    {
        int area1=Area(rp,r1);
        int area2=Area(rp,r2);
        return (area1<area2);
    }
    void dividirNodo();

};
#endif // RTREE_H_INCLUDED
