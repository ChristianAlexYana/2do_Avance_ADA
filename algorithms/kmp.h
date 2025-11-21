#ifndef KMP_H
#define KMP_H

#include "searchalgorithm.h"

class KMP : public SearchAlgorithm {
public:
    SearchResult search(const QString& text, const QString& pattern) override;
    QString getName() const override { return "KMP (Knuth-Morris-Pratt)"; }

private:
    QVector<int> computeLPS(const QString& pattern);
};

#endif
