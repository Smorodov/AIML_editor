#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "highlighter.h"
#include "codeeditor.h"
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMainWindow>
#include <QSpacerItem>
#include <QLabel>
#include <QLineEdit>
#include <qsplitter.h>
#include <QAction>
#include <QTabWidget>
#include <QTableWidget>
#include "aimlparser.h"

QT_BEGIN_NAMESPACE
class QTextEdit;
QT_END_NAMESPACE

//! [0]
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    AIMLParser* parser;
    QTabWidget* tabWidget;
    QPushButton* format_code_button;
    QPushButton* init_code_button;
    QPushButton* add_category_button;
    QWidget* central;
    QWidget* botState;
    QTableWidget* WhatTable;
    QTableWidget* inputTable;
    QTableWidget* paramTable;
    QTableWidget* botTable;

    QTableWidget* subTable;

    QVBoxLayout* central_layout;
    QVBoxLayout* chat_layout;
    QVBoxLayout* log_layout;
    QHBoxLayout* chat_log_layout;
    QHBoxLayout* layout;
    QVBoxLayout* left_panel_layout;
    QSpacerItem* spacer;
    QTextEdit* logger;
    QTextEdit* chat;
    QLabel* label1;
    QLabel* label2;

    QPushButton* chat_enter_button;
    QLineEdit* lineedit;

    QSplitter* chat_log_splitter;
    QSplitter* top_bottom_splitter;
    QWidget* wtop;

public slots:
    void about();
    void newFile(void);
    void openFile(void);
    void saveFile(void);
    void onFormatCodeButtonClick();
    void onInitCodeButtonClick();
    void onAddCategoryButtonClick();
    void onChatEnterButtonClick();
    void cursorPositionChanged();
    void closeEvent(QCloseEvent* event);
private:
    void setupEditor();
    void setupFileMenu();
    void setupHelpMenu();
    void readSettings();
    void writeSettings();
    void updateStateTables(void);
    CodeEditor *editor;
    Highlighter *highlighter;
    QAction* newAct;
    QAction* openAct;
    QAction* saveAct;
};
//! [0]

#endif // MAINWINDOW_H
