#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QFont>
#include <QHeaderView>
#include <QShortcut>
#include <QKeySequence>
#include <QListWidget>
#include <QStackedWidget>
#include <QLabel>
#include <sstream>
#include "../include/Parser.h"
#include "../include/Lexer.h"
#include "../include/Token.h"
#include "../include/MainWindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) { setupUI(); }

MainWindow::~MainWindow() {}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(15);

    setStyleSheet("background-color: #1a1b26;");


    codeEditor = new QTextEdit(this);
    codeEditor->setFont(QFont("Consolas", 14));
    codeEditor->setStyleSheet(
        "QTextEdit {"
        "  background-color: #24283b;"
        "  color: #c0caf5;"
        "  border: 2px solid #414868;"
        "  border-radius: 10px;"
        "  font-size: 25px;"
        "  padding: 15px;"
        "  line-height: 1.4;"
        "}"
    );
    codeEditor->setPlaceholderText("Enter your TINY code here...");
    codeEditor->setTabStopDistance(QFontMetricsF(codeEditor->font()).horizontalAdvance("    "));


    tokenizeButton = new QPushButton("Tokenize", this);
    parseButton = new QPushButton("Parse", this);
    
    QString buttonStyle = 
        "QPushButton {"
        "  background: #5e81cc;"
        "  color: black;"
        "  border: none;"
        "  border-radius: 8px;"
        "  font-size: 20px;"
        "  font-weight: bold;"
        "  padding: 12px 20px;"
        "  margin: 2px;"
        "}"
        "QPushButton:hover {"
        "  background: #81a1c1"
        "}"
        "QPushButton:pressed {"
        "  background: #4c566a;"
        "}";

    tokenizeButton->setStyleSheet(buttonStyle);
    parseButton->setStyleSheet(buttonStyle);
    tokenizeButton->setFixedHeight(50);
    parseButton->setFixedHeight(50);
    tokenizeButton->setCursor(Qt::PointingHandCursor);
    parseButton->setCursor(Qt::PointingHandCursor);

    tokenizeButton->setToolTip("Tokenize Code (Ctrl+T)");
    parseButton->setToolTip("Parse Code (Ctrl+P)");

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(tokenizeButton);
    buttonLayout->addWidget(parseButton);
    buttonLayout->setSpacing(10);

    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->addWidget(codeEditor);
    leftLayout->addSpacing(15);
    leftLayout->addLayout(buttonLayout);

    rightPanel = new QStackedWidget(this);    

    setupTokenTablePage();
    setupErrorListPage();
    
    rightPanel->addWidget(tokenTablePage);
    rightPanel->addWidget(errorListPage);
    rightPanel->setCurrentIndex(0);

    mainLayout->addLayout(leftLayout, 5);
    mainLayout->addWidget(rightPanel, 5);

    setCentralWidget(centralWidget);
    setWindowTitle("TINY Language Compiler");
    resize(1200, 800);

    connect(tokenizeButton, &QPushButton::clicked, this, &MainWindow::showTokenizeView);
    connect(parseButton, &QPushButton::clicked, this, &MainWindow::showParseView);

    QShortcut *tokenizeShortcut = new QShortcut(QKeySequence("Ctrl+T"), this);
    QShortcut *parseShortcut = new QShortcut(QKeySequence("Ctrl+P"), this);
    connect(tokenizeShortcut, &QShortcut::activated, this, &MainWindow::showTokenizeView);
    connect(parseShortcut, &QShortcut::activated, this, &MainWindow::showParseView);
}

void MainWindow::setupTokenTablePage()
{
    tokenTablePage = new QWidget();
    QVBoxLayout *tokenLayout = new QVBoxLayout(tokenTablePage);
    
    QLabel *tokenTitle = new QLabel("Tokens");
    tokenTitle->setStyleSheet(
        "QLabel {"
        "  color: black;"
        "  font-size: 22px;"
        "  font-weight: bold;"
        "  padding: 10px;"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #7aa2f7, stop:1 #bb9af7);"
        "  border-radius: 8px;"
        "  margin-bottom: 10px;"
        "}"
    );
    tokenTitle->setAlignment(Qt::AlignCenter);


    tokenTable = new QTableWidget();
    tokenTable->setColumnCount(2);
    tokenTable->setHorizontalHeaderLabels({"Lexeme", "Token Type"});
    tokenTable->horizontalHeader()->setStretchLastSection(true);
    tokenTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tokenTable->verticalHeader()->setVisible(false);
    tokenTable->setShowGrid(true);
    tokenTable->setGridStyle(Qt::DotLine);
    tokenTable->setFont(QFont("Consolas", 12));
    tokenTable->setStyleSheet(
        "QTableWidget {"
        "  background-color: #24283b;"
        "  color: #c0caf5;"
        "  border: 2px solid #414868;"
        "  border-radius: 10px;"
        "  font-size: 13px;"
        "  gridline-color: #414868;"
        "  selection-background-color: #7aa2f7;"
        "}"
        "QHeaderView::section {"
        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #565f89, stop:1 #414868);"
        "  color: #c0caf5;"
        "  padding: 12px;"
        "  font-size: 14px;"
        "  font-weight: bold;"
        "  border: 1px solid #414868;"
        "}"
        "QTableWidget::item {"
        "  padding: 10px;"
        "  border-bottom: 1px solid #414868;"
        "}"
        "QTableWidget::item:selected {"
        "  background-color: #7aa2f7;"
        "  color: #1a1b26;"
        "}"
    );

    tokenLayout->addWidget(tokenTitle);
    tokenLayout->addWidget(tokenTable);
}

void MainWindow::setupErrorListPage()
{
    errorListPage = new QWidget();
    QVBoxLayout *errorLayout = new QVBoxLayout(errorListPage);
    

    QLabel *errorTitle = new QLabel("Syntax Errors");
    errorTitle->setStyleSheet(
        "QLabel {"
        "  color: black;"
        "  font-size: 22px;"
        "  font-weight: bold;"
        "  padding: 10px;"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #f7768e, stop:1 #ff9e64);"
        "  border-radius: 8px;"
        "  margin-bottom: 10px;"
        "}"
    );
    errorTitle->setAlignment(Qt::AlignCenter);

    errorList = new QListWidget();
    errorList->setFont(QFont("Consolas", 12));
    errorList->setStyleSheet(
        "QListWidget {"
        "  background-color: #24283b;"
        "  color: #c0caf5;"
        "  border: 2px solid #414868;"
        "  border-radius: 10px;"
        "  font-size: 13px;"
        "  padding: 10px;"
        "}"
        "QListWidget::item {"
        "  padding: 12px;"
        "  margin: 2px 0px;"
        "  background-color: #1a1b26;"
        "  border: 1px solid #414868;"
        "  border-radius: 6px;"
        "}"
        "QListWidget::item:hover {"
        "  background-color: #414868;"
        "  border-color: #7aa2f7;"
        "}"
        "QListWidget::item:selected {"
        "  background-color: #7aa2f7;"
        "  color: #1a1b26;"
        "  border-color: #bb9af7;"
        "}"
    );

    errorLayout->addWidget(errorTitle);
    errorLayout->addWidget(errorList);
}

void MainWindow::showTokenizeView()
{
    QString code = codeEditor->toPlainText();
    if (code.isEmpty()) {
        return;
    }
    
    string codeStr = code.toStdString();
    vector<Token> tokens = tokenize(codeStr);

    tokenTable->clearContents();
    tokenTable->setRowCount(tokens.size());

    for (int i = 0; i < tokens.size(); ++i)
    {
        auto *lexItem = new QTableWidgetItem(QString::fromStdString(tokens[i].lexeme));
        auto *typeItem = new QTableWidgetItem(QString::fromStdString(tokens[i].type));

        QFont itemFont("Consolas", 13);
        lexItem->setFont(itemFont);
        typeItem->setFont(itemFont);

        lexItem->setTextAlignment(Qt::AlignCenter);
        typeItem->setTextAlignment(Qt::AlignCenter);

        tokenTable->setItem(i, 0, lexItem);
        tokenTable->setItem(i, 1, typeItem);
    }

    for (int i = 0; i < tokenTable->rowCount(); i++)
    {
        tokenTable->setRowHeight(i, 40);
    }

    rightPanel->setCurrentIndex(0);
    

    tokenizeButton->setStyleSheet(tokenizeButton->styleSheet());
    parseButton->setStyleSheet(parseButton->styleSheet());
}

void MainWindow::showParseView()
{
    QString code = codeEditor->toPlainText();
    if (code.isEmpty()) {
        return;
    }
    
    string codeStr = code.toStdString();
    vector<string> errors = parse(codeStr);

    errorList->clear();
    
    if (errors.empty()) {
        QListWidgetItem *noErrorItem = new QListWidgetItem("✅ No syntax errors found!");
        noErrorItem->setTextColor(QColor("#9ece6a"));
        noErrorItem->setBackground(QBrush(QColor("#1a1b26")));
        noErrorItem->setFlags(noErrorItem->flags() & ~Qt::ItemIsSelectable);
        noErrorItem->setTextAlignment(Qt::AlignCenter);
        QFont successFont("Consolas", 14);
        successFont.setBold(true);
        noErrorItem->setFont(successFont);
        errorList->addItem(noErrorItem);
        
    } else {
        for (int i = 0; i < errors.size(); ++i) {
            QString errorText = QString("[Error %1] %2").arg(i + 1).arg(QString::fromStdString(errors[i]));
            QListWidgetItem *errorItem = new QListWidgetItem(errorText);
            QFont errorFont("Consolas", 12);
            errorItem->setTextColor(QColor("#f7768e"));
            errorItem->setFont(errorFont);
            errorList->addItem(errorItem);
        }
    }

    rightPanel->setCurrentIndex(1);

    parseButton->setStyleSheet(parseButton->styleSheet());
    tokenizeButton->setStyleSheet(tokenizeButton->styleSheet());
}

vector<Token> MainWindow::tokenize(const string &s)
{
    Lexer lexer(s);
    return lexer.tokenize();
}

vector<string> MainWindow::parse(const string& s) 
{
    Lexer lexer(s);
    auto tokens = lexer.tokenize();
    
    Parser parser(tokens);
    return parser.parse();
}