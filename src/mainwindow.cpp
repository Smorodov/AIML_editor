#include <QtWidgets>
#include <QStatusBar>
#include <QToolBar>
#include "codeeditor.h"
#include "mainwindow.h"
#include "aimlparser.h"
#include "QTextCursor"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    parser=new AIMLParser();
    tabWidget = new QTabWidget();

    QToolBar* fileToolBar = addToolBar(tr("File"));

    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/images/addPage.png"));
    newAct = new QAction(newIcon, tr(u8"&Новый"));
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr(u8"Создать новый файл."));
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
    fileToolBar->addAction(newAct);

    const QIcon saveIcon = QIcon::fromTheme("document-new", QIcon(":/images/images/save.png"));
    saveAct = new QAction(saveIcon, tr(u8"&Coхранить..."));
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr(u8"Сохранить файл"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::saveFile);
    fileToolBar->addAction(saveAct);

    const QIcon openIcon = QIcon::fromTheme("document-new", QIcon(":/images/images/folder3.png"));
    openAct = new QAction(openIcon, tr(u8"&Открыть"), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr(u8"Открыть"));
    connect(openAct, &QAction::triggered, this, &MainWindow::openFile);
    fileToolBar->addAction(openAct);



    setupFileMenu();
    setupHelpMenu();
    setupEditor();

    statusBar()->showMessage(tr(u8"Строка:") + QString::number(1) + tr(u8" Столбец:") + QString::number(1));
    statusBar()->show();

    central = new QWidget();    
    botState = new QWidget();
    
    WhatTable = new QTableWidget();
    inputTable = new QTableWidget();
    paramTable = new QTableWidget();
    botTable = new QTableWidget();
    subTable = new QTableWidget();


    inputTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    WhatTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    paramTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    botTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    subTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);



    QHBoxLayout* botStateLayout = new QHBoxLayout();
    botState->setLayout(botStateLayout);
    QVBoxLayout* botStateLayout1 = new QVBoxLayout();
    QVBoxLayout* botStateLayout2 = new QVBoxLayout();
    QVBoxLayout* botStateLayout3 = new QVBoxLayout();
    QVBoxLayout* botStateLayout4 = new QVBoxLayout();
    QVBoxLayout* botStateLayout5 = new QVBoxLayout();
    botStateLayout->addLayout(botStateLayout1);
    botStateLayout->addLayout(botStateLayout2);
    botStateLayout->addLayout(botStateLayout3);
    botStateLayout->addLayout(botStateLayout4);
    botStateLayout->addLayout(botStateLayout5);
    
    QLabel* table1Label = new QLabel();
    QLabel* table2Label = new QLabel();
    QLabel* table3Label = new QLabel();
    QLabel* table4Label = new QLabel();
    QLabel* table5Label = new QLabel();

    table1Label->setText(u8"История ввода пользователя");
    table2Label->setText(u8"История ответов бота (<what>)");
    table3Label->setText(u8"Таблица запомненных данных <set>, <get>");
    table4Label->setText(u8"Таблица параметров ботп <bot>");
    table5Label->setText(u8"Таблицв замен <normalize>, <denormalize>");

    botStateLayout1->addWidget(table1Label);
    botStateLayout2->addWidget(table2Label);
    botStateLayout3->addWidget(table3Label);
    botStateLayout4->addWidget(table4Label);
    botStateLayout5->addWidget(table5Label);

    botStateLayout1->addWidget(inputTable);
    botStateLayout2->addWidget(WhatTable);
    botStateLayout3->addWidget(paramTable);
    botStateLayout4->addWidget(botTable);
    botStateLayout5->addWidget(subTable);


    tabWidget->addTab(central, u8"Редактор");
    tabWidget->addTab(botState, u8"Состояние бота");

    setCentralWidget(tabWidget);

    wtop = new QWidget();

    central_layout = new QVBoxLayout();

    chat_layout = new QVBoxLayout();
    log_layout = new QVBoxLayout();

    label1 = new QLabel();
    label1->setText(u8"Чат");
    label1->setFixedHeight(23);
    label2 = new QLabel();
    label2->setText(u8"Лог");
    label2->setFixedHeight(23);
    chat_log_layout = new QHBoxLayout();


    layout = new QHBoxLayout();
    left_panel_layout = new QVBoxLayout();

    chat_enter_button = new QPushButton();
    chat_enter_button->setFixedHeight(32);
    chat_enter_button->setText(u8"Ввод");
    lineedit = new QLineEdit();
    lineedit->setFixedHeight(32);

    chat_log_splitter = new QSplitter(Qt::Orientation::Horizontal);
    top_bottom_splitter = new QSplitter(Qt::Orientation::Vertical);

    central->setLayout(central_layout);

    chat = new QTextEdit();
    logger = new QTextEdit();

    init_code_button = new QPushButton();
    init_code_button->setFixedWidth(160);
    init_code_button->setText(u8"Начальный шаблон");
    left_panel_layout->addWidget(init_code_button);


    add_category_button = new QPushButton();
    add_category_button->setFixedWidth(160);
    add_category_button->setText(u8"Добавить категорию");
    left_panel_layout->addWidget(add_category_button);

    format_code_button = new QPushButton();
    format_code_button->setFixedWidth(160);
    format_code_button->setText(u8"Форматировать код");

    spacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    left_panel_layout->addSpacerItem(spacer);
    left_panel_layout->addWidget(format_code_button);

    layout->addLayout(left_panel_layout);
    layout->addWidget(editor);

    chat_log_layout->addWidget(chat_log_splitter);

    chat_layout->addWidget(label1);
    chat_layout->addWidget(chat);
    chat_layout->addWidget(lineedit);
    chat_layout->addWidget(chat_enter_button);

    wtop->setLayout(layout);
    top_bottom_splitter->addWidget(wtop);

    top_bottom_splitter->addWidget(chat_log_splitter);

    central_layout->addWidget(top_bottom_splitter);

    QWidget* w1 = new QWidget();
    QWidget* w2 = new QWidget();
    chat_log_splitter->addWidget(w1);
    chat_log_splitter->addWidget(w2);
    w1->setLayout(chat_layout);
    w2->setLayout(log_layout);


    log_layout->addWidget(label2);
    log_layout->addWidget(logger);



    connect(format_code_button, SIGNAL(clicked()), this, SLOT(onFormatCodeButtonClick()));
    connect(add_category_button, SIGNAL(clicked()), this, SLOT(onAddCategoryButtonClick()));
    connect(init_code_button, SIGNAL(clicked()), this, SLOT(onInitCodeButtonClick()));
    connect(chat_enter_button, SIGNAL(clicked()), this, SLOT(onChatEnterButtonClick()));
    connect(editor, SIGNAL(cursorPositionChanged()), this, SLOT(cursorPositionChanged()));

    setWindowTitle(tr(u8"Редактор AIML кода"));

    readSettings();
    updateStateTables();
}

// -------------------------------------------------------------
//
// -------------------------------------------------------------
void MainWindow::cursorPositionChanged()
{
    // положение в тексте
    QTextCursor textCursor = editor->cursorForPosition(editor->cursorRect().center());
    // положение на экране
    int line = 0;
    int col = 0;
    line = textCursor.blockNumber();
    col = textCursor.columnNumber();
    statusBar()->showMessage(tr(u8"Строка:") + QString::number(line + 1) + tr(u8" Столбец:") + QString::number(col + 1));
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Syntax Highlighter"),
        tr("<p>The <b>Syntax Highlighter</b> example shows how " \
            "to perform simple syntax highlighting by subclassing " \
            "the QSyntaxHighlighter class and describing " \
            "highlighting rules using regular expressions.</p>"));
}

void MainWindow::newFile()
{
    editor->clear();
}

void MainWindow::openFile(void)
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", "AIML (*.aiml)");

    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (file.open(QFile::ReadOnly | QFile::Text))
        {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            editor->setPlainText(stream.readAll());
            file.flush();
            file.close();
        }
    }
}

void MainWindow::saveFile(void)
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", "AIML (*.aiml)");

    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (file.open(QFile::WriteOnly | QFile::Text))
        {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            stream << editor->toPlainText();
            file.flush();
            file.close();
        }
    }
}

//! [1]
void MainWindow::setupEditor()
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(14);

    editor = new CodeEditor;
    editor->setFont(font);

    highlighter = new Highlighter(editor->document());
}

void MainWindow::setupFileMenu()
{
    QMenu* fileMenu = new QMenu(tr(u8"&Файл"), this);
    menuBar()->addMenu(fileMenu);
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(tr(u8"В&ыход"), qApp, &QApplication::quit, QKeySequence::Quit);
}

void MainWindow::setupHelpMenu()
{
    QMenu* helpMenu = new QMenu(tr("&Help"), this);
    menuBar()->addMenu(helpMenu);
    helpMenu->addAction(tr("&About"), this, &MainWindow::about);
    helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
}
// -------------------------------------------------------------
//
// -------------------------------------------------------------
void MainWindow::onAddCategoryButtonClick()
{
    QTextCursor textCursor = editor->cursorForPosition(editor->cursorRect().center());
    QString textToAdd =
        u8"<category>\n"
        u8"<pattern></pattern>\n"
        u8"<template></template>\n"
        u8"</category>"
        ;
    textCursor.insertText(textToAdd);
}
// -------------------------------------------------------------
//
// -------------------------------------------------------------
void MainWindow::onInitCodeButtonClick()
{
    QString textToAdd =
        u8"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        u8"<aiml version=\"1.0\">\n"
        u8"<meta name=\"author\" content=\"Andrey\"/>\n"
        u8"<meta name=\"language\" content=\"Russian\"/>\n"
        u8"<category>\n"
        u8"<pattern> * </pattern>\n"
        u8"<template>У меня нет на это ответа...</template>\n"
        u8"</category>\n"
        u8"</aiml>\n"
        ;
    editor->appendPlainText(textToAdd);
}
// -------------------------------------------------------------
//
// -------------------------------------------------------------
void MainWindow::onFormatCodeButtonClick()
{
    QSet<QString> levelChangingTags;
    levelChangingTags << "aiml" << "category" << "template" << "pattern";

    QString txt = editor->toPlainText();
    editor->textToSingleLine(txt);
    std::vector<tagInfo> tags = editor->getTagMap(txt);

    int err_tag_ind = 0;
    bool isValid = editor->valid_paren(tags, err_tag_ind);
    if (!isValid)
    {
        return;
    }

    QRegularExpression commentExpression = QRegularExpression(QStringLiteral("(<!--)([^(-->)])*(-->)"));
    // extract comments
    QRegularExpressionMatchIterator commentMatchIterator = commentExpression.globalMatch(txt);
    while (commentMatchIterator.hasNext())
    {
        QRegularExpressionMatch match = commentMatchIterator.next();
        int comment_start = match.capturedStart();
        int comment_end = comment_start + match.capturedLength();
        qDebug() << "comment: " << comment_start << " to " << comment_end;
        tags.push_back(tagInfo(" ", match.capturedStart(), match.capturedStart() + match.capturedLength(), -1));
    }
    // sort using a custom function object
    struct
    {
        bool operator()(tagInfo a, tagInfo b) const
        {
            return a.tagBegin < b.tagBegin;
        }
    } customLess;
    std::sort(tags.begin(), tags.end(), customLess);


    int len = txt.length();
    int offset = 0;
    for (auto tag : tags)
    {
        if (tag.tagType == tagInfo::open && levelChangingTags.contains(tag.tagName))
        {
            txt.insert(tag.tagEnd + offset, '\n');
            offset++;
            txt.insert(tag.tagBegin + offset - 1, '\n');
            offset++;
        }
        if (tag.tagType == tagInfo::close && levelChangingTags.contains(tag.tagName))
        {
            txt.insert(tag.tagBegin + offset - 1, '\n');
            offset++;
            txt.insert(tag.tagEnd + offset, '\n');
            offset++;
        }
        if (tag.tagType == -1)
        {
            txt.insert(tag.tagBegin + offset - 1, '\n');
            offset++;
            txt.insert(tag.tagEnd + offset, '\n');
            offset++;
        }
    }
    if (txt.length() > 0)
    {
        if (txt.at(0) == '\n')
        {
            txt = txt.remove(0, 1);
        }
    }

    if (txt.length() > 0)
    {
        if (txt.at(txt.length() - 1) == '\n')
        {
            txt = txt.remove(txt.length() - 1, 1);
        }
    }
    txt = txt.replace(" \n", "\n");
    txt = txt.replace("\n ", "\n");
    txt = txt.replace("\n\n", "\n");
    txt = txt.replace("\n\n", "\n");

    tags.clear();
    tags = editor->getTagMap(txt);
    int n_added = 0;
    int level = 0;


    for (int i = 0; i < tags.size(); ++i)
    {
        if (tags[i].tagType == tagInfo::open && levelChangingTags.contains(tags[i].tagName))
        {
            tags[i].tagLevel = level;
            level++;
        }
        if (tags[i].tagType == tagInfo::close && levelChangingTags.contains(tags[i].tagName))
        {
            level--;
            tags[i].tagLevel = level;
        }
    }
    QString txt2;
    int linestart = 0;
    int ind = 0;
    int lvl = 0;
    int prev_lvl = 0;

    for (int pos = 0; pos < txt.length(); ++pos)
    {
        QChar c = txt.at(pos);
        bool isInTag = false;
        for (auto tag : tags)
        {
            if (tag.tagType != tagInfo::mono && tag.tagType != -1 && tag.tagBegin <= pos && tag.tagEnd >= pos && levelChangingTags.contains(tag.tagName))
            {
                isInTag = true;
                prev_lvl = lvl;
                lvl = tag.tagLevel;
            }
        }

        if (c == '\n')
        {
            if (isInTag)
            {
                for (int s = 0; s < lvl; ++s)
                {
                    txt2.insert(linestart + 1, "    ");
                    ind += 4;
                }
            }
            else
            {
                for (int s = 0; s < lvl; ++s)
                {
                    txt2.insert(linestart + 1, "      ");
                    ind += 6;
                }
            }
            linestart = ind - prev_lvl + lvl;
        }
        txt2.append(c);
        ind++;

    }

    editor->setPlainText(txt2);
}


void MainWindow::updateStateTables(void)
{
    // Таблица ответов бота
    QStringList what_columns;
    what_columns << u8"that";
    WhatTable->setColumnCount(what_columns.size());
    WhatTable->setHorizontalHeaderLabels(what_columns);

    int maxCols = 0;
    for (auto l : parser->thatList)
    {
        if (l.size() > maxCols)
        {
            maxCols = l.size();
        }
    }
    int rowNum = parser->thatList.size();
    WhatTable->setColumnCount(maxCols);
    WhatTable->setRowCount(rowNum);
    WhatTable->verticalHeader()->hide();

    for (int i = 0; i < WhatTable->rowCount(); i++)
    {
        for (int j = 0; j < WhatTable->columnCount(); j++)
        {
            QTableWidgetItem* pCell = WhatTable->item(i, j);
            if (!pCell)
            {
                pCell = new QTableWidgetItem;
                WhatTable->setItem(i, j, pCell);
            }
            if (pCell)
            {
                if (parser->thatList[i].size() > j)
                {
                    pCell->setText(parser->thatList[i][j]);
                }
            }
        }
    }


    // Таблица ввода пользователя
    QStringList input_columns;
    input_columns << u8"input";
    inputTable->setColumnCount(input_columns.size());
    inputTable->setHorizontalHeaderLabels(input_columns);

    rowNum = parser->inputList.size();
    inputTable->setColumnCount(1);
    inputTable->setRowCount(rowNum);
    inputTable->verticalHeader()->hide();

    for (int i = 0; i < inputTable->rowCount(); i++)
    {
        for (int j = 0; j < inputTable->columnCount(); j++)
        {
            QTableWidgetItem* pCell = inputTable->item(i, j);
            if (!pCell)
            {
                pCell = new QTableWidgetItem;
                inputTable->setItem(i, j, pCell);
            }
            if (pCell)
            {
                pCell->setText(parser->inputList[i]);
            }
        }
    }


    QStringList param_columns;
    param_columns << u8"Название" << u8"Значение";
    paramTable->setColumnCount(param_columns.size());
    paramTable->setHorizontalHeaderLabels(param_columns);
    rowNum = parser->parameterValue.toStdMap().size();
    paramTable->setColumnCount(2);
    paramTable->setRowCount(rowNum);
    paramTable->verticalHeader()->hide();

    int i = 0;

    for (auto m : parser->parameterValue.toStdMap())
    {
        QString key = m.first;
        QString value = m.second;

        for (int j = 0; j < paramTable->columnCount(); j++)
        {
            QTableWidgetItem* pCell = paramTable->item(i, j);
            if (!pCell)
            {
                pCell = new QTableWidgetItem;
                paramTable->setItem(i, j, pCell);
            }
            if (pCell)
            {
                if (j == 0)
                {
                    pCell->setText(key);
                }
                if (j == 1)
                {
                    pCell->setText(value);
                }
            }
        }
        ++i;
    }

    QStringList bot_columns;
    bot_columns << u8"Название" << u8"Значение";
    botTable->setColumnCount(bot_columns.size());
    botTable->setHorizontalHeaderLabels(bot_columns);

    rowNum = parser->botVarValue.toStdMap().size();
    botTable->setColumnCount(2);
    botTable->setRowCount(rowNum);
    botTable->verticalHeader()->hide();
    i = 0;
    for (auto m : parser->botVarValue.toStdMap())
    {
        QString key = m.first;
        QString value = m.second;

        for (int j = 0; j < botTable->columnCount(); j++)
        {
            QTableWidgetItem* pCell = botTable->item(i, j);
            if (!pCell)
            {
                pCell = new QTableWidgetItem;
                botTable->setItem(i, j, pCell);
            }
            if (pCell)
            {
                if (j == 0)
                {
                    pCell->setText(key);
                }
                if (j == 1)
                {
                    pCell->setText(value);
                }
            }
        }
        ++i;
    }

    // Таблица подстановаок
    QStringList sub_columns;
    sub_columns << u8"Было" << u8"Стало";
    subTable->setColumnCount(sub_columns.size());
    subTable->setHorizontalHeaderLabels(sub_columns);

    rowNum = parser->subOld.size();
    subTable->setColumnCount(2);
    subTable->setRowCount(rowNum);
    subTable->verticalHeader()->hide();

    for (int i = 0; i < subTable->rowCount(); i++)
    {
        for (int j = 0; j < subTable->columnCount(); j++)
        {
            QTableWidgetItem* pCell = subTable->item(i, j);
            if (!pCell)
            {
                pCell = new QTableWidgetItem;
                subTable->setItem(i, j, pCell);
            }
            if (pCell)
            {
                if (j == 0)
                {
                    pCell->setText(parser->subOld[i].pattern());
                }
                if (j == 1)
                {
                    pCell->setText(parser->subNew[i]);
                }
            }
        }
    }
}


void MainWindow::onChatEnterButtonClick()
{

    parser->loadAimlFromString(editor->toPlainText());
    QString chatInput = lineedit->text();
    if (!chatInput.isEmpty())
    {
        chat->append(u8"Вы : " + chatInput);
        chat->append(u8"Бот : " + parser->getResponse(lineedit->text()));
        QString logTxt;
        logTxt=parser->logStream->readAll();
        parser->logStream->flush();
        logger->append(logTxt);
        updateStateTables();
    }
}

void MainWindow::writeSettings()
{
    QSettings settings("Smorodov soft", "AIML_editor");

    settings.beginGroup("top_bottom_splitter");
    settings.setValue("pos", top_bottom_splitter->saveState());
    settings.endGroup();

    settings.beginGroup("chat_log_splitter");
    settings.setValue("pos", top_bottom_splitter->saveState());
    settings.endGroup();

    settings.beginGroup("MainWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.endGroup();
}

void MainWindow::readSettings()
{
    QSettings settings("Smorodov soft", "AIML_editor");

    settings.beginGroup("top_bottom_splitter");
    top_bottom_splitter->restoreState(settings.value("pos").toByteArray());
    top_bottom_splitter->setOrientation(Qt::Vertical);
    settings.endGroup();

    settings.beginGroup("chat_log_splitter");
    chat_log_splitter->restoreState(settings.value("pos").toByteArray());
    chat_log_splitter->setOrientation(Qt::Horizontal);
    //settings.setValue("pos", chat_log_splitter->pos());
    settings.endGroup();

    settings.beginGroup("MainWindow");
    resize(settings.value("size", QSize(400, 400)).toSize());
    move(settings.value("pos", QPoint(200, 200)).toPoint());
    settings.endGroup();
}

void MainWindow::closeEvent(QCloseEvent* event)
{

    QMessageBox::StandardButton resBtn = QMessageBox::question(this, "AIML_Editor",
        tr(u8"Уверены?\n"),
        QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
        QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes)
    {
        event->ignore();
    }
    else
    {
        delete parser;
        //optional check if the user really want to quit
        // and/or if the user want to save settings        
        writeSettings();
        event->accept();
    }
}