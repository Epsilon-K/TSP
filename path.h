#ifndef PATH_H
#define PATH_H

#include <utils.h>

class Path{
public:
    Path(QVector<int> v){
        order = v;
        length = LONG_MAX;
    }
    Path(){
        length = LONG_MAX;
    }

    QVector<int> order;
    long length;

    void operator = (const Path &p ) {
        order = p.order;
        length = p.length;
    }

    void shuffle(int times){
        for(int i = 0; i < times; i++){
            int ia = (rand()%(order.size()-1)) +1;
            int ib = (rand()%(order.size()-1)) +1;

            int t = order[ia];
            order[ia] = order[ib];
            order[ib] = t;
        }
    }

    void print(){
        for(int i = 0; i < order.size(); i++){
            qDebug() << order[i] << "  ";
        }
    }

};

#endif // PATH_H
