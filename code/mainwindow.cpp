#include "mainwindow.h"
#include "functions.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QFont>
#include <QHeaderView>
#include <QShortcut>
#include <QKeySequence>
#include <sstream>
#define eb emplace_back
using namespace std;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) { setupUI(); }

MainWindow::~MainWindow() {}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(15);

    setStyleSheet("background-color: #24283B");

    codeEditor = new QTextEdit(this);
    codeEditor->setFont(QFont("Consolas"));
    codeEditor->setStyleSheet(
        "QTextEdit {"
        "  background-color: #3b4252;"
        "  color: #d8dee9;"
        "  border-radius: 8px;"
        "  font-size: 20px;"
        "  padding-top: 15px;"
        "  padding-left: 10px;"
        "}");
    codeEditor->setPlaceholderText("Enter your TINY code here...");

    analyzeButton = new QPushButton("Analyze Code", this);
    analyzeButton->setToolTip("Analyze Code (Ctrl+Enter)");
    analyzeButton->setFont(QFont("Arial", 11, QFont::Bold));
    analyzeButton->setCursor(Qt::PointingHandCursor);
    analyzeButton->setFixedHeight(35);
    analyzeButton->setStyleSheet(
        "QPushButton {"
        "  background-color:rgba(32, 56, 100, 0.91);"
        "  color: white;"
        "  border-radius: 6px;"
        "  font-size: 13px;"
        "  padding: 6px 12px;"
        "}"
        "QPushButton:hover {"
        "  background-color:rgba(73, 110, 196, 0.85);"
        "}"
        "QPushButton:pressed {"
        "  background-color: #4c566a;"
        "}");

    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->addWidget(codeEditor);
    leftLayout->addSpacing(10);
    leftLayout->addWidget(analyzeButton);

    tokenTable = new QTableWidget(this);
    tokenTable->setColumnCount(2);
    tokenTable->setHorizontalHeaderLabels({"Lexeme", "Token Type"});
    tokenTable->horizontalHeader()->setStretchLastSection(true);
    tokenTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tokenTable->verticalHeader()->setVisible(false);
    tokenTable->setShowGrid(false);
    tokenTable->setFont(QFont("Consolas", 14));
    tokenTable->setStyleSheet(
        "QTableWidget {"
        "  background-color: #2e3440;"
        "  color: #e5e9f0;"
        "  border-radius: 8px;"
        "  font-size: 14px;"
        "}"
        "QHeaderView::section {"
        "  background-color: #434c5e;"
        "  color: #eceff4;"
        "  padding: 8px;"
        "  font-size: 14px;"
        "}"
        "QTableWidget::item {"
        "  padding: 8px;"
        "  color: #e5e9f0;"
        "  selection-background-color: #5e81ac;"
        "  selection-color: #eceff4;"
        "}");

    mainLayout->addLayout(leftLayout, 6);
    mainLayout->addWidget(tokenTable, 4);

    setCentralWidget(centralWidget);
    setWindowTitle("TINY Language Compiler");
    resize(1000, 700);

    connect(analyzeButton, &QPushButton::clicked, this, &MainWindow::analyzeCode);

    QShortcut *analyzeShortcut = new QShortcut(QKeySequence("Ctrl+Return"), this);
    connect(analyzeShortcut, &QShortcut::activated, this, &MainWindow::analyzeCode);
}

void MainWindow::analyzeCode()
{
    QString code = codeEditor->toPlainText();
    string codeStr = code.toStdString();
    vector<pair<string, string>> tokens;

    istringstream stream(codeStr);
    string line;

    while (getline(stream, line))
    {
        line += "\\";
        auto lineTokens = tokenize(line);
        tokens.insert(tokens.end(), lineTokens.begin(), lineTokens.end());
    }

    tokenTable->clearContents();
    tokenTable->setRowCount(tokens.size());

    for (size_t i = 0; i < tokens.size(); ++i)
    {
        auto *lexItem = new QTableWidgetItem(QString::fromStdString(tokens[i].first));
        auto *typeItem = new QTableWidgetItem(QString::fromStdString(tokens[i].second));

        QFont lexemeFont("Consolas", 14);
        lexItem->setFont(lexemeFont);

        QFont typeFont("Consolas", 14);
        typeItem->setFont(typeFont);

        lexItem->setTextAlignment(Qt::AlignCenter);
        typeItem->setTextAlignment(Qt::AlignCenter);

        tokenTable->setItem(i, 0, lexItem);
        tokenTable->setItem(i, 1, typeItem);
    }

    tokenTable->horizontalHeader()->setStretchLastSection(true);
    tokenTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    for (int i = 0; i < tokenTable->rowCount(); i++)
    {
        tokenTable->setRowHeight(i, 34);
    }
}

vector<pair<string, string>> MainWindow::tokenize(const string &s)
{
    vector<pair<string, string>> v;
    string t;

    int l = s.length();

    for (int i = 0; i < l-1 ; i++)
    {
        if(isDigit(s[i])) {
            while(isDigit(s[i]) && i < l-1) t+=s[i++];
            v.eb(t, "Number"), t="", i--;
        }

        else if(isAlpha(s[i])) {
            while(isAlphaNum(s[i]) && i < l-1) t+=s[i++];
            v.eb(t, isReserved(t) ? "Reserved word" : "Identifier"), t="", i--;
        }

        else {
            if(s[i] == '{') {
                t="{", i++;
                while(s[i] != '}' && i < l-1)t+=s[i++];
                s[i] == '}' ? v.eb(t+"}", "Comment") : v.eb(t, "Unterminated Comment");
            }

            else if (s[i]=='"') {
                t="\"", i++;
                while(s[i]!='"' && i < l-1)t+=s[i++];
                s[i] == '"' ? v.eb(t+"\"", "String") : v.eb(t, "Unterminated string");
            }

            else if(s[i]==':' && s[i+1]=='=') {
                v.eb(":=", "Assignment operator");
                i++;
            }

            else if(isArithmetic(s[i])) v.eb(string(1, s[i]), "Arithmetic operator");
            else if(isComparison(s[i])) v.eb(string(1, s[i]), "Comparison operator");
            else if(isParenthesis(s[i])) v.eb(string(1, s[i]), "Parenthesis");
            else if(s[i]==';') v.eb(";", "Semicolon");
            else if(s[i]==',') v.eb(",", "Comma");
            else if (s[i] == ' ') continue;
            else v.eb(string(1, s[i]), "Unknown");

            t="";
            continue;
        }
    }

    return v;
}
