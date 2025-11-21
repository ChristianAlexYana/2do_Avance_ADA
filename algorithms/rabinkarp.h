#ifndef RABINKARP_H
#define RABINKARP_H

#include "searchalgorithm.h"

class RabinKarp : public SearchAlgorithm {
public:
    SearchResult search(const QString& text, const QString& pattern) override;
    QString getName() const override { return "Rabin-Karp"; }

private:
    static const int PRIME = 101;
    static const int BASE = 256;
};

#endif
