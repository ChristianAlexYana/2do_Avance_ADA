#include "rabinkarp.h"
#include <QElapsedTimer>

SearchResult RabinKarp::search(const QString& text, const QString& pattern) {
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

    long long patternHash = 0;
    long long textHash = 0;
    long long h = 1;

    for (int i = 0; i < m - 1; i++) {
        h = (h * BASE) % PRIME;
    }

    for (int i = 0; i < m; i++) {
        patternHash = (BASE * patternHash + pattern[i].unicode()) % PRIME;
        textHash = (BASE * textHash + text[i].unicode()) % PRIME;
    }

    for (int i = 0; i <= n - m; i++) {
        result.comparisons++;

        if (patternHash == textHash) {
            bool match = true;
            for (int j = 0; j < m; j++) {
                result.comparisons++;
                if (text[i + j] != pattern[j]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                result.positions.append(i);
            }
        }

        if (i < n - m) {
            textHash = (BASE * (textHash - text[i].unicode() * h) + text[i + m].unicode()) % PRIME;
            if (textHash < 0) {
                textHash = (textHash % PRIME + PRIME) % PRIME;
            }
        }
    }

    result.timeElapsed = timer.nsecsElapsed() / 1000000000.0;
    return result;
}
