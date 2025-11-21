#include "mainwindow.h"
#include "algorithms/kmp.h"
#include "algorithms/boyermoore.h"
#include "algorithms/rabinkarp.h"
#include "algorithms/naive.h"
#include "dnaanalyzer.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QTextCharFormat>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    algorithms.append(new KMP());
    algorithms.append(new BoyerMoore());
    algorithms.append(new RabinKarp());
    algorithms.append(new Naive());

    setupUI();
}

MainWindow::~MainWindow() {
    qDeleteAll(algorithms);
}

void MainWindow::setupUI() {
    setWindowTitle("Buscador de Patrones - Algoritmos de Busqueda");
    setMinimumSize(950, 800);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    setStyleSheet(R"(
        QMainWindow {
            background-color: #ffffff;
        }
        QGroupBox {
            font-weight: bold;
            border: 2px solid #e0e0e0;
            border-radius: 8px;
            margin-top: 10px;
            padding-top: 15px;
            background-color: #fafafa;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 5px;
            color: #2c3e50;
        }
        QLineEdit, QTextEdit {
            border: 2px solid #e0e0e0;
            border-radius: 6px;
            padding: 8px;
            background-color: white;
            font-size: 13px;
        }
        QLineEdit:focus, QTextEdit:focus {
            border-color: #3498db;
        }
        QPushButton {
            background-color: #3498db;
            color: white;
            border: none;
            border-radius: 6px;
            padding: 10px 20px;
            font-weight: bold;
            font-size: 13px;
        }
        QPushButton:hover {
            background-color: #2980b9;
        }
        QPushButton:pressed {
            background-color: #21618c;
        }
        QPushButton#clearButton {
            background-color: #95a5a6;
        }
        QPushButton#clearButton:hover {
            background-color: #7f8c8d;
        }
        QPushButton#loadFileButton {
            background-color: #27ae60;
        }
        QPushButton#loadFileButton:hover {
            background-color: #229954;
        }
        QPushButton#analyzeDNAButton {
            background-color: #e74c3c;
        }
        QPushButton#analyzeDNAButton:hover {
            background-color: #c0392b;
        }
        QComboBox {
            border: 2px solid #e0e0e0;
            border-radius: 6px;
            padding: 8px;
            padding-right: 30px;
            background-color: white;
            font-size: 13px;
            min-height: 20px;
        }
        QComboBox:focus {
            border-color: #3498db;
        }
        QComboBox::drop-down {
            border: none;
            width: 30px;
        }
        QComboBox::down-arrow {
            image: none;
            border-left: 5px solid transparent;
            border-right: 5px solid transparent;
            border-top: 5px solid #555;
            margin-right: 10px;
        }
        QComboBox QAbstractItemView {
            border: 2px solid #e0e0e0;
            background-color: white;
            selection-background-color: #3498db;
            selection-color: white;
            padding: 5px;
        }
        QLabel {
            color: #2c3e50;
            font-size: 13px;
        }
        QTableWidget {
            border: 2px solid #e0e0e0;
            border-radius: 6px;
            background-color: white;
            gridline-color: #ecf0f1;
        }
        QTableWidget::item {
            padding: 8px;
        }
        QHeaderView::section {
            background-color: #3498db;
            color: white;
            padding: 10px;
            border: none;
            font-weight: bold;
        }
        QCheckBox {
            font-size: 13px;
            spacing: 8px;
        }
        QCheckBox::indicator {
            width: 18px;
            height: 18px;
            border: 2px solid #e0e0e0;
            border-radius: 4px;
            background-color: white;
        }
        QCheckBox::indicator:checked {
            background-color: #3498db;
            border-color: #3498db;
        }
    )");

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    QLabel *titleLabel = new QLabel("Buscador de Patrones");
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #2c3e50; margin-bottom: 10px;");
    mainLayout->addWidget(titleLabel);

    QGroupBox *textGroup = new QGroupBox("Texto donde buscar");
    QVBoxLayout *textLayout = new QVBoxLayout(textGroup);

    QHBoxLayout *fileLayout = new QHBoxLayout();
    loadFileButton = new QPushButton("Cargar desde archivo TXT");
    loadFileButton->setObjectName("loadFileButton");
    fileSizeLabel = new QLabel("Tamanio: 0 caracteres");
    fileSizeLabel->setStyleSheet("font-weight: bold; color: #7f8c8d;");
    dnaCheckBox = new QCheckBox("Modo ADN");
    dnaCheckBox->setStyleSheet("font-weight: normal;");
    analyzeDNAButton = new QPushButton("Analizar ADN");
    analyzeDNAButton->setObjectName("analyzeDNAButton");
    analyzeDNAButton->setVisible(false);

    fileLayout->addWidget(loadFileButton);
    fileLayout->addWidget(fileSizeLabel);
    fileLayout->addStretch();
    fileLayout->addWidget(dnaCheckBox);
    fileLayout->addWidget(analyzeDNAButton);
    textLayout->addLayout(fileLayout);

    textEdit = new QTextEdit();
    textEdit->setPlaceholderText("Ingrese o pegue el texto aqui, o cargue un archivo TXT...");
    textEdit->setMinimumHeight(120);
    textLayout->addWidget(textEdit);

    mainLayout->addWidget(textGroup);

    dnaAnalysisText = new QTextEdit();
    dnaAnalysisText->setReadOnly(true);
    dnaAnalysisText->setMaximumHeight(100);
    dnaAnalysisText->setVisible(false);
    dnaAnalysisText->setStyleSheet("background-color: #fff3cd; border: 2px solid #ffc107;");
    mainLayout->addWidget(dnaAnalysisText);

    QGroupBox *searchGroup = new QGroupBox("Configuracion de busqueda");
    QVBoxLayout *searchLayout = new QVBoxLayout(searchGroup);

    QHBoxLayout *patternLayout = new QHBoxLayout();
    QLabel *patternLabel = new QLabel("Patron a buscar:");
    patternLabel->setStyleSheet("font-weight: normal;");
    patternEdit = new QLineEdit();
    patternEdit->setPlaceholderText("Ingrese el patron...");
    patternLayout->addWidget(patternLabel);
    patternLayout->addWidget(patternEdit, 1);
    searchLayout->addLayout(patternLayout);

    QHBoxLayout *algorithmLayout = new QHBoxLayout();
    QLabel *algoLabel = new QLabel("Algoritmo:");
    algoLabel->setStyleSheet("font-weight: normal;");
    algoLabel->setMinimumWidth(80);
    algorithmCombo = new QComboBox();
    for (auto algo : algorithms) {
        algorithmCombo->addItem(algo->getName());
    }
    algorithmLayout->addWidget(algoLabel);
    algorithmLayout->addWidget(algorithmCombo, 1);
    searchLayout->addLayout(algorithmLayout);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    searchButton = new QPushButton("Buscar");
    clearButton = new QPushButton("Limpiar");
    clearButton->setObjectName("clearButton");
    buttonLayout->addStretch();
    buttonLayout->addWidget(searchButton);
    buttonLayout->addWidget(clearButton);
    searchLayout->addLayout(buttonLayout);

    mainLayout->addWidget(searchGroup);

    QGroupBox *resultsGroup = new QGroupBox("Resultados");
    QVBoxLayout *resultsLayout = new QVBoxLayout(resultsGroup);

    QHBoxLayout *statsLayout = new QHBoxLayout();
    timeLabel = new QLabel("Tiempo: - segundos");
    timeLabel->setStyleSheet("font-weight: bold; color: #27ae60;");
    matchesLabel = new QLabel("Coincidencias: 0");
    matchesLabel->setStyleSheet("font-weight: bold; color: #2980b9;");
    comparisonsLabel = new QLabel("Comparaciones: 0");
    comparisonsLabel->setStyleSheet("font-weight: bold; color: #8e44ad;");
    statsLayout->addWidget(timeLabel);
    statsLayout->addWidget(matchesLabel);
    statsLayout->addWidget(comparisonsLabel);
    statsLayout->addStretch();
    resultsLayout->addLayout(statsLayout);

    resultsTable = new QTableWidget();
    resultsTable->setColumnCount(2);
    resultsTable->setHorizontalHeaderLabels({"Posicion", "Contexto"});
    resultsTable->horizontalHeader()->setStretchLastSection(true);
    resultsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    resultsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    resultsTable->setAlternatingRowColors(true);
    resultsTable->setMinimumHeight(180);
    resultsLayout->addWidget(resultsTable);

    mainLayout->addWidget(resultsGroup);

    connect(searchButton, &QPushButton::clicked, this, &MainWindow::performSearch);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::clearResults);
    connect(patternEdit, &QLineEdit::returnPressed, this, &MainWindow::performSearch);
    connect(loadFileButton, &QPushButton::clicked, this, &MainWindow::loadFromFile);
    connect(dnaCheckBox, &QCheckBox::toggled, [this](bool checked) {
        analyzeDNAButton->setVisible(checked);
        if (checked) {
            updateDNAAnalysis();
        } else {
            dnaAnalysisText->setVisible(false);
        }
    });
    connect(analyzeDNAButton, &QPushButton::clicked, this, &MainWindow::analyzeDNA);
    connect(textEdit, &QTextEdit::textChanged, [this]() {
        fileSizeLabel->setText(QString("Tamanio: %1 caracteres").arg(textEdit->toPlainText().length()));
        if (dnaCheckBox->isChecked()) {
            updateDNAAnalysis();
        }
    });
}

void MainWindow::loadFromFile() {
    QString fileName = QFileDialog::getOpenFileName(this,
        "Abrir archivo de texto", "",
        "Archivos de texto (*.txt *.fasta *.fa *.seq);;Todos los archivos (*)");

    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "No se pudo abrir el archivo: " + file.errorString());
        return;
    }

    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    textEdit->setPlainText(content);
    fileSizeLabel->setText(QString("Tamanio: %1 caracteres | Archivo: %2")
                          .arg(content.length())
                          .arg(QFileInfo(fileName).fileName()));

    QMessageBox::information(this, "Archivo cargado",
                            QString("Se cargaron %1 caracteres correctamente").arg(content.length()));
}

void MainWindow::updateDNAAnalysis() {
    if (!dnaCheckBox->isChecked()) {
        return;
    }

    QString text = textEdit->toPlainText();
    if (text.isEmpty()) {
        dnaAnalysisText->setVisible(false);
        return;
    }

    DNAAnalysis analysis = DNAAnalyzer::analyze(text);

    QString analysisText = "<b>Analisis de Secuencia ADN:</b><br>";

    if (analysis.isValid) {
        analysisText += QString("<span style='color: green;'>Secuencia valida</span><br>");
    } else {
        analysisText += QString("<span style='color: red;'>Secuencia invalida (%1 bases incorrectas)</span><br>")
                       .arg(analysis.invalidBases);
    }

    analysisText += QString("<b>Bases A-T:</b> %1 | <b>Bases G-C:</b> %2<br>")
                   .arg(analysis.atCount).arg(analysis.gcCount);
    analysisText += QString("<b>Contenido GC:</b> %1%<br>").arg(analysis.gcContent, 0, 'f', 2);

    if (!analysis.errors.isEmpty()) {
        analysisText += "<br><b style='color: red;'>Errores:</b><br>";
        for (const QString& error : analysis.errors) {
            analysisText += "- " + error + "<br>";
        }
    }

    if (!analysis.warnings.isEmpty()) {
        analysisText += "<br><b style='color: orange;'>Advertencias geneticas:</b><br>";
        for (const QString& warning : analysis.warnings) {
            analysisText += "- " + warning + "<br>";
        }
    }

    dnaAnalysisText->setHtml(analysisText);
    dnaAnalysisText->setVisible(true);
}

void MainWindow::analyzeDNA() {
    updateDNAAnalysis();
}

void MainWindow::performSearch() {
    QString text = textEdit->toPlainText();
    QString pattern = patternEdit->text();

    if (pattern.isEmpty()) {
        matchesLabel->setText("Coincidencias: 0 (patron vacio)");
        return;
    }

    int algoIndex = algorithmCombo->currentIndex();
    SearchResult result = algorithms[algoIndex]->search(text, pattern);

    timeLabel->setText(QString("Tiempo: %1 segundos").arg(result.timeElapsed, 0, 'f', 9));
    matchesLabel->setText(QString("Coincidencias: %1").arg(result.positions.size()));
    comparisonsLabel->setText(QString("Comparaciones: %1").arg(result.comparisons));

    resultsTable->setRowCount(result.positions.size());
    for (int i = 0; i < result.positions.size(); i++) {
        int pos = result.positions[i];
        resultsTable->setItem(i, 0, new QTableWidgetItem(QString::number(pos)));

        int contextStart = qMax(0, pos - 20);
        int contextEnd = qMin(text.length(), pos + pattern.length() + 20);
        QString context = text.mid(contextStart, contextEnd - contextStart);

        if (contextStart > 0) context = "..." + context;
        if (contextEnd < text.length()) context += "...";

        resultsTable->setItem(i, 1, new QTableWidgetItem(context));
    }

    highlightMatches(result.positions, pattern.length());
}

void MainWindow::highlightMatches(const QVector<int>& positions, int patternLength) {
    QTextCursor cursor = textEdit->textCursor();
    cursor.beginEditBlock();

    QTextCharFormat defaultFormat;
    defaultFormat.setBackground(Qt::white);
    cursor.select(QTextCursor::Document);
    cursor.setCharFormat(defaultFormat);

    QTextCharFormat highlightFormat;
    highlightFormat.setBackground(QColor("#ffeb3b"));
    highlightFormat.setFontWeight(QFont::Bold);

    for (int pos : positions) {
        cursor.setPosition(pos);
        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, patternLength);
        cursor.setCharFormat(highlightFormat);
    }

    cursor.endEditBlock();
}

void MainWindow::clearResults() {
    textEdit->clear();
    patternEdit->clear();
    resultsTable->setRowCount(0);
    timeLabel->setText("Tiempo: - segundos");
    matchesLabel->setText("Coincidencias: 0");
    comparisonsLabel->setText("Comparaciones: 0");
    fileSizeLabel->setText("Tamaño: 0 caracteres");
    dnaAnalysisText->setVisible(false);
}
