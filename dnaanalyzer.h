#ifndef DNAANALYZER_H
#define DNAANALYZER_H

#include <QString>
#include <QVector>

struct DNAAnalysis {
    bool isValid;
    QVector<QString> errors;
    QVector<QString> warnings;
    double gcContent;
    int atCount;
    int gcCount;
    int invalidBases;
    QVector<int> invalidPositions;
};

class DNAAnalyzer {
public:
    static DNAAnalysis analyze(const QString& sequence);
    static bool isValidDNABase(QChar base);
    static QString getComplementaryBase(QChar base);
    static QString getReverseComplement(const QString& sequence);

private:
    static const QString VALID_BASES;
};

#endif
