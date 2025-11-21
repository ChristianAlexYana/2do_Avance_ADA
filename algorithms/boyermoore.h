#ifndef BOYERMOORE_H
#define BOYERMOORE_H

#include "searchalgorithm.h"
#include <QHash>

class BoyerMoore : public SearchAlgorithm {
public:
    SearchResult search(const QString& text, const QString& pattern) override;
    QString getName() const override { return "Boyer-Moore"; }

private:
    QHash<QChar, int> computeBadChar(const QString& pattern);
};

#endif
