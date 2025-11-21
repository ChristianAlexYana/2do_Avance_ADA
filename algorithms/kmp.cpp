#include "kmp.h"
#include <QElapsedTimer>

QVector<int> KMP::computeLPS(const QString& pattern) {
    int m = pattern.length();
    QVector<int> lps(m, 0);
    int len = 0;
    int i = 1;

    while (i < m) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
    return lps;
}

SearchResult KMP::search(const QString& text, const QString& pattern) {
    SearchResult result;
    QElapsedTimer timer;
    timer.start();

    int n = text.length();
    int m = pattern.length();
    result.comparisons = 0;

    if (m == 0 || n == 0 || m > n) {
        result.timeElapsed = timer.nsecsElapsed() / 1000000000.0;
        return result;
    }

    QVector<int> lps = computeLPS(pattern);
    int i = 0;
    int j = 0;

    while (i < n) {
        result.comparisons++;
        if (pattern[j] == text[i]) {
            i++;
            j++;
        }

        if (j == m) {
            result.positions.append(i - j);
            j = lps[j - 1];
        } else if (i < n && pattern[j] != text[i]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }

    result.timeElapsed = timer.nsecsElapsed() / 1000000000.0;
    return result;
}
