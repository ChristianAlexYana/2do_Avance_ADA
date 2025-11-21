#include "naive.h"
#include <QElapsedTimer>

SearchResult Naive::search(const QString& text, const QString& pattern) {
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

    for (int i = 0; i <= n - m; i++) {
        int j;
        for (j = 0; j < m; j++) {
            result.comparisons++;
            if (text[i + j] != pattern[j]) {
                break;
            }
        }
        if (j == m) {
            result.positions.append(i);
        }
    }

    result.timeElapsed = timer.nsecsElapsed() / 1000000000.0;
    return result;
}
