#include "mainwindow.h"
#include "functions.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <sstream>
using namespace std;
#define eb emplace_back

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) { setupUI(); }

MainWindow::~MainWindow() {}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    codeEditor = new QTextEdit(this);
    codeEditor->setFont(QFont("Courier", 10));
    codeEditor->setMinimumWidth(400);

    tokenTable = new QTableWidget(this);
    tokenTable->setColumnCount(2);
    tokenTable->setHorizontalHeaderLabels({"Lexeme", "Token Type"});
    tokenTable->setMinimumWidth(300);

    analyzeButton = new QPushButton("Analyze Code", this);

    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->addWidget(codeEditor);
    rightLayout->addWidget(analyzeButton);

    mainLayout->addWidget(tokenTable);
    mainLayout->addLayout(rightLayout);

    setCentralWidget(centralWidget);
    setWindowTitle("TINY Language Compiler");
    resize(800, 600);

    connect(analyzeButton, &QPushButton::clicked, this, &MainWindow::analyzeCode);
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

    tokenTable->setRowCount(tokens.size());
    for (size_t i = 0; i < tokens.size(); ++i)
    {
        tokenTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(tokens[i].first)));
        tokenTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(tokens[i].second)));
    }

    tokenTable->resizeColumnsToContents();
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
                i++;
                while(s[i] != '}' && i < l-1)t+=s[i++];
                v.eb(t, s[i] == '}' ? "Comment" : "Unterminated comment");
            }

            else if (s[i]=='"') {
                t='"', i++;
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
            else if (s[i] == ' ') continue;
            else v.eb(string(1, s[i]), "Unknown");

            t="";
            continue;
        }
    }

    return v;
}
