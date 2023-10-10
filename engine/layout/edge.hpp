#ifndef ENGINE_LAYOUT_EDGE_H
#define ENGINE_LAYOUT_EDGE_H




class Edge {
public:
    int top, left, right, bottom;

    Edge():top(0),left(0),right(0),bottom(0) { }
    Edge(int t, int  r ,int  l ,int  b):top(t),left(l),right(r),bottom(b) {

    }

    void print() {
        printf("%d %d %d %d\n",top,left,bottom,right);
    }


    ~Edge() {}
};

#endif // ENGINE_LAYOUT_EDGE_H