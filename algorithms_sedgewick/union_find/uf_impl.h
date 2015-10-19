#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
#include <assert.h> 

// the union find interface

struct union_find {
    //never called, here just to compile wqupc
    union_find() {assert(false);}

    union_find(uint64_t N) : cnt{N} {
        ids.resize(N);
        std::iota(ids.begin(), ids.end(), 0);
    }

    virtual ~union_find(){}

    bool connected(uint64_t p, uint64_t q) {return find(p) == find(q);}
    virtual void connect(uint64_t p, uint64_t q) = 0;

    virtual uint64_t find(uint64_t p) = 0;

    uint64_t count() const { return cnt; }

    virtual std::string name() const = 0;
protected:
    uint64_t cnt;
    std::vector<uint64_t> ids;
};

// quick find (eager approach) implementation 

struct union_find_quick_find : public union_find {
    union_find_quick_find(uint64_t N) : union_find(N) {}

    std::string name() const override {return "quick find";}

    void connect(uint64_t p, uint64_t q) override {
        auto idp = find(p);
        auto idq = find(q);

        if (idp == idq)
            return;

        std::transform(ids.cbegin(), ids.cend(), ids.begin(), [idp, idq](uint64_t v){return (v == idp ? idq : v);});
        cnt--;
    }

    uint64_t find(uint64_t p) override { return ids.at(p); }
};

// quick union (lazy approach) implementation

struct union_find_quick_union : public union_find {
    //never called, here just to compile wqupc
    union_find_quick_union() {assert(false);}

    union_find_quick_union(uint64_t N) : union_find(N) {}

    std::string name() const override {return "quick union";}

    void connect(uint64_t p, uint64_t q) override {
        auto idp = find(p);
        auto idq = find(q);

        if (idp == idq)
            return;

        ids[idp] = idq;
        cnt--;
    }

    uint64_t find(uint64_t p) override {
        while (p != ids.at(p))
            p = ids[p];
        return p;
    }
};

// quick union with path compression implementation

struct union_find_quick_union_path_compression : public virtual union_find_quick_union {
    union_find_quick_union_path_compression(uint64_t N) : union_find_quick_union(N) {}

    std::string name() const override {return "quick union with path compression";}

    void connect(uint64_t p, uint64_t q) override {
        union_find_quick_union::connect(p,q);
    }

    uint64_t find(uint64_t p) override {
        while (p != ids.at(p)) {
            //make every other node in the path point to its grandparent
            //thereby halving the length => O(log(N))
            ids[p] = ids[ids[p]];
            p = ids[p];
        }
        return p;
    }
};

// weighted quick union implementation

struct union_find_weighted_quick_union : public virtual union_find_quick_union {
    union_find_weighted_quick_union(uint64_t N) : union_find_quick_union(N) {cnts.assign(N, 1);}

    std::string name() const override {return "weighted quick union";}

    void connect(uint64_t p, uint64_t q) override {
        auto idp = find(p);
        auto idq = find(q);

        if (idp == idq)
            return;

        // here the weighting is done based on the rank (number of elements)
        // but it can be done on height too
        if (cnts[idp] < cnts[idq]) {
            ids[idp] = idq;
            cnts[idq] += cnts[idp];
        } else {
            ids[idq] = idp;
            cnts[idp] += cnts[idq];
        }

        cnt--;
    }

    uint64_t find(uint64_t p) override {
        return union_find_quick_union::find(p);
    }

protected:
    std::vector<uint64_t> cnts;
};

// weighted quick union with path compression implementation

struct union_find_weighted_quick_union_path_compression
    : public union_find_quick_union_path_compression, public union_find_weighted_quick_union
{
    union_find_weighted_quick_union_path_compression(uint64_t N)
        : union_find_quick_union_path_compression(N), union_find_weighted_quick_union(N), union_find_quick_union(N)
    {}

    std::string name() const override {return "weighted quick union with path compression";}

    void connect(uint64_t p, uint64_t q) override {union_find_weighted_quick_union::connect(p,q);}

    uint64_t find(uint64_t p) override {return union_find_quick_union_path_compression::find(p);}
};
