
#ifndef FINAL_BDD_H
#define FINAL_BDD_H

enum val {
    True = 1, False = 0, X = 'X'
};

class vertex {
private:
    vertex *low, *high;
    int id, index;
    val value;
    bool mark;

public:
    vertex(vertex *low, vertex *high, int id, int index, val value, bool mark) : low(low), high(high), id(id),
                                                                                 index(index), value(value),
                                                                                 mark(mark) {}

    vertex() {
        low = nullptr;
        high = nullptr;
        id = 0;
        index = 0;
        value = X;
        mark = false;
    }

    vertex *getLow() const {
        return low;
    }

    void setLow(vertex *l) {
        vertex::low = l;
    }

    vertex *getHigh() const {
        return high;
    }

    void setHigh(vertex *h) {
        vertex::high = h;
    }

    int getId() const {
        return id;
    }

    void setId(int i) {
        vertex::id = i;
    }

    int getIndex() const {
        return index;
    }

    void setIndex(int i) {
        vertex::index = i;
    }

    val getValue() const {
        return value;
    }

    void setValue(val v) {
        vertex::value = v;
    }

    bool getMark() const {
        return mark;
    }

    void setMark(bool m) {
        vertex::mark = m;
    }
};


#endif //FINAL_BDD_H
