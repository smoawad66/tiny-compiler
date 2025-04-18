#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QTableWidget>
#include <QPushButton>
#include <vector>
#include <string>
using namespace std;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void analyzeCode();

private:
    QTextEdit *codeEditor;
    QTableWidget *tokenTable;
    QPushButton *analyzeButton;
    void setupUI();
    vector<pair<string, string>> tokenize(const string& input);
};

#endif
