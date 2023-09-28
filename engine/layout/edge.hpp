#ifndef ENGINE_LAYOUT_EDGE_H
#define ENGINE_LAYOUT_EDGE_H




class Edge {
public:
    unsigned int top, left, right, buttom;

    Edge():top(0),left(0),right(0),buttom(0) { }
    Edge(unsigned int t, unsigned int  r ,unsigned int  l ,unsigned int  b):top(t),left(l),right(r),buttom(b) {

    }


    ~Edge() {}
};

#endif // ENGINE_LAYOUT_EDGE_H