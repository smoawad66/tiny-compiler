#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QTableWidget>
#include <QPushButton>
#include <QListWidget>
#include <QStackedWidget>
#include <QWidget>
#include <vector>
#include <string>
#include "../include/Token.h"

using namespace std;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showTokenizeView();
    void showParseView();

private:
    // UI Components
    QTextEdit *codeEditor;
    QPushButton *tokenizeButton;
    QPushButton *parseButton;
    
    // Right panel components
    QStackedWidget *rightPanel;
    QWidget *tokenTablePage;
    QWidget *errorListPage;
    QTableWidget *tokenTable;
    QListWidget *errorList;
    
    // Methods
    void setupUI();
    void setupTokenTablePage();
    void setupErrorListPage();
    vector<Token> tokenize(const string& input);
    vector<string> parse(const string& input);
};

#endif