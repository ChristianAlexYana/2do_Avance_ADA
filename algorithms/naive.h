#ifndef NAIVE_H
#define NAIVE_H

#include "searchalgorithm.h"

class Naive : public SearchAlgorithm {
public:
    SearchResult search(const QString& text, const QString& pattern) override;
    QString getName() const override { return "Busqueda Naive (Fuerza Bruta)"; }
};

#endif
