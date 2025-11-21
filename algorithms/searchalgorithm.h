#ifndef SEARCHALGORITHM_H
#define SEARCHALGORITHM_H

#include <QString>
#include <QVector>

struct SearchResult {
    QVector<int> positions;
    double timeElapsed;
    int comparisons;
};

class SearchAlgorithm {
public:
    virtual ~SearchAlgorithm() = default;
    virtual SearchResult search(const QString& text, const QString& pattern) = 0;
    virtual QString getName() const = 0;
};

#endif
