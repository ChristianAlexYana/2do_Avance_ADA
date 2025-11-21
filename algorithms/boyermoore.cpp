#include "boyermoore.h"
#include <QElapsedTimer>

QHash<QChar, int> BoyerMoore::computeBadChar(const QString& pattern) {
    QHash<QChar, int> badChar;
    int m = pattern.length();

    for (int i = 0; i < m; i++) {
        badChar[pattern[i]] = i;
    }

    return badChar;
}

SearchResult BoyerMoore::search(const QString& text, const QString& pattern) {
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

    QHash<QChar, int> badChar = computeBadChar(pattern);
    int s = 0;

    while (s <= n - m) {
        int j = m - 1;

        while (j >= 0 && pattern[j] == text[s + j]) {
            result.comparisons++;
            j--;
        }

        if (j >= 0) {
            result.comparisons++;
        }

        if (j < 0) {
            result.positions.append(s);
            s += (s + m < n) ? m - badChar.value(text[s + m], -1) : 1;
        } else {
            s += qMax(1, j - badChar.value(text[s + j], -1));
        }
    }

    result.timeElapsed = timer.nsecsElapsed() / 1000000000.0;
    return result;
}
