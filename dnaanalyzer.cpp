#include "dnaanalyzer.h"

const QString DNAAnalyzer::VALID_BASES = "ATCG";

bool DNAAnalyzer::isValidDNABase(QChar base) {
    return VALID_BASES.contains(base.toUpper());
}

QString DNAAnalyzer::getComplementaryBase(QChar base) {
    switch(base.toUpper().unicode()) {
        case 'A': return "T";
        case 'T': return "A";
        case 'C': return "G";
        case 'G': return "C";
        default: return "?";
    }
}

QString DNAAnalyzer::getReverseComplement(const QString& sequence) {
    QString result;
    for (int i = sequence.length() - 1; i >= 0; i--) {
        result += getComplementaryBase(sequence[i]);
    }
    return result;
}

DNAAnalysis DNAAnalyzer::analyze(const QString& sequence) {
    DNAAnalysis analysis;
    analysis.isValid = true;
    analysis.atCount = 0;
    analysis.gcCount = 0;
    analysis.invalidBases = 0;
    analysis.gcContent = 0.0;

    if (sequence.isEmpty()) {
        analysis.isValid = false;
        analysis.errors.append("La secuencia esta vacia");
        return analysis;
    }

    for (int i = 0; i < sequence.length(); i++) {
        QChar base = sequence[i].toUpper();

        if (base == ' ' || base == '\n' || base == '\r' || base == '\t') {
            continue;
        }

        if (!isValidDNABase(base)) {
            analysis.isValid = false;
            analysis.invalidBases++;
            analysis.invalidPositions.append(i);
            analysis.errors.append(QString("Base invalida '%1' en posicion %2").arg(base).arg(i));
        } else {
            if (base == 'A' || base == 'T') {
                analysis.atCount++;
            } else if (base == 'G' || base == 'C') {
                analysis.gcCount++;
            }
        }
    }

    int totalBases = analysis.atCount + analysis.gcCount;
    if (totalBases > 0) {
        analysis.gcContent = (double)analysis.gcCount / totalBases * 100.0;
    }

    if (analysis.gcContent > 70) {
        analysis.warnings.append(QString("Contenido GC muy alto: %.2f%% (puede dificultar amplificacion por PCR)").arg(analysis.gcContent));
    } else if (analysis.gcContent < 30) {
        analysis.warnings.append(QString("Contenido GC muy bajo: %.2f%% (puede afectar estabilidad)").arg(analysis.gcContent));
    }

    if (totalBases < 20) {
        analysis.warnings.append("Secuencia muy corta (< 20 bases)");
    }

    int maxRepeat = 1;
    int currentRepeat = 1;
    QChar lastBase = sequence[0];

    for (int i = 1; i < sequence.length(); i++) {
        if (sequence[i].toUpper() == lastBase.toUpper()) {
            currentRepeat++;
            maxRepeat = qMax(maxRepeat, currentRepeat);
        } else {
            currentRepeat = 1;
            lastBase = sequence[i];
        }
    }

    if (maxRepeat >= 5) {
        analysis.warnings.append(QString("Repeticion homopolimerica detectada (%1 bases consecutivas)").arg(maxRepeat));
    }

    return analysis;
}
