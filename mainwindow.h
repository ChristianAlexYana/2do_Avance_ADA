#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>
#include <QCheckBox>
#include "algorithms/searchalgorithm.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void performSearch();
    void clearResults();
    void loadFromFile();
    void analyzeDNA();

private:
    void setupUI();
    void highlightMatches(const QVector<int>& positions, int patternLength);
    void updateDNAAnalysis();

    QTextEdit *textEdit;
    QLineEdit *patternEdit;
    QComboBox *algorithmCombo;
    QPushButton *searchButton;
    QPushButton *clearButton;
    QPushButton *loadFileButton;
    QCheckBox *dnaCheckBox;
    QPushButton *analyzeDNAButton;
    QLabel *timeLabel;
    QLabel *matchesLabel;
    QLabel *comparisonsLabel;
    QLabel *fileSizeLabel;
    QTextEdit *dnaAnalysisText;
    QTableWidget *resultsTable;

    QVector<SearchAlgorithm*> algorithms;
};

#endif
