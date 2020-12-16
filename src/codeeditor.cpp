#include "codeeditor.h"
#include <QPainter>
#include <QTextBlock>
#include <QRegExp>
#include <stack>
#include <QRegularExpression>
// -----------------------------
//
// -----------------------------
int start = 0, end = 0;
bool needClean = false;

// -------------------------------------------------------------
//
// -------------------------------------------------------------
std::vector<tagInfo> CodeEditor::getTagMap(QString text)
{
    QRegularExpression tagOpenExpression = QRegularExpression(QStringLiteral("(<[^\\/?!])((\\.)|[^>])*([^\/]>)"));
    QRegularExpression tagCloseExpression = QRegularExpression(QStringLiteral("(<\\/)((\\.)|[^>])*([^?\/]>)"));
    QRegularExpression tagMonoExpression = QRegularExpression(QStringLiteral("(<[^\/\?])((\\.)|[^\\>])*(\/>)"));

    QRegularExpression firstWordExpression = QRegularExpression(QStringLiteral("^([\/\\w\\-]+)"));

    QRegularExpressionMatchIterator openMatchIterator = tagOpenExpression.globalMatch(text);
    QRegularExpressionMatchIterator closeMatchIterator = tagCloseExpression.globalMatch(text);
    QRegularExpressionMatchIterator monoMatchIterator = tagMonoExpression.globalMatch(text);

    std::vector<tagInfo> allTags;


    while (openMatchIterator.hasNext())
    {
        QRegularExpressionMatch match = openMatchIterator.next();
        QStringRef subString(&text, match.capturedStart() + 1, match.capturedLength() - 2);
        QString str = subString.toString();
        QRegularExpressionMatchIterator firstWordMatchIterator = firstWordExpression.globalMatch(str);
        QRegularExpressionMatch match1 = firstWordMatchIterator.next();
        QStringRef subString1(&str, match1.capturedStart(), match1.capturedLength());
        allTags.push_back(tagInfo(subString1.toString(), match.capturedStart(), match.capturedStart() + match.capturedLength(), tagInfo::open));
    }


    while (closeMatchIterator.hasNext())
    {
        QRegularExpressionMatch match = closeMatchIterator.next();
        QStringRef subString(&text, match.capturedStart() + 1, match.capturedLength() - 2);
        QString str = subString.toString();
        QRegularExpressionMatchIterator firstWordMatchIterator = firstWordExpression.globalMatch(str);
        QRegularExpressionMatch match1 = firstWordMatchIterator.next();
        QStringRef subString1(&str, match1.capturedStart(), match1.capturedLength());
        allTags.push_back(tagInfo(subString1.toString().remove('/'), match.capturedStart(), match.capturedStart() + match.capturedLength(), tagInfo::close));
    }

    while (monoMatchIterator.hasNext())
    {
        QRegularExpressionMatch match = monoMatchIterator.next();
        QStringRef subString(&text, match.capturedStart() + 1, match.capturedLength() - 2);
        QString str = subString.toString();
        QRegularExpressionMatchIterator firstWordMatchIterator = firstWordExpression.globalMatch(str);
        QRegularExpressionMatch match1 = firstWordMatchIterator.next();
        QStringRef subString1(&str, match1.capturedStart(), match1.capturedLength());
        allTags.push_back(tagInfo(subString1.toString().remove('/'), match.capturedStart(), match.capturedStart() + match.capturedLength(), tagInfo::mono));
    }


    // sort using a custom function object
    struct
    {
        bool operator()(tagInfo a, tagInfo b) const
        {
            return a.tagBegin < b.tagBegin;
        }
    } customLess;
    std::sort(allTags.begin(), allTags.end(), customLess);

    return allTags;
}
// -------------------------------------------------------------
//
// -------------------------------------------------------------
void CodeEditor::textToSingleLine(QString& txt)
{
    txt = txt.replace('\n', ' ');
    txt = txt.replace(">", "> ");
    txt = txt.replace("<", " <");
    txt = txt.simplified();
}
// -------------------------------------------------------------
//
// -------------------------------------------------------------
bool CodeEditor::valid_paren(std::vector<tagInfo> input, int& ind)
{
    // Declaraing a stack.
    std::stack <tagInfo> s;
    int length = input.size();
    tagInfo top;
    ind = 0;
    // Iterating over the entire string.
    for (int i = 0; i < length; i++)
    {
        if (input[i].tagType == -1 || input[i].tagType == 2)
        {
            continue;
        }
        //  If the input string contains an opening parenthesis,
        //  push in on to the stack.
        if (input[i].tagType == tagInfo::open)
        {
            s.push(input[i]);
            ind = i;
            continue;
        }
        else
        { // In the case of valid parentheses, the stack cannot be 
         // be empty if a closing parenthesis is encountered.
            if (s.empty())
            {
                //qDebug() << input << " contains invalid parentheses.";
                return false;
            }
            else
            {
                top = s.top();
                // If the input string contains a closing bracket,
                // then pop the corresponding opening parenthesis if
                // present.
                if (input[i].tagType == tagInfo::close &&
                    input[i].tagName == top.tagName &&
                    top.tagType == tagInfo::open)
                {
                    s.pop();
                }
                else
                {
                    // The opening and closing parentheses are of
                    // different types. This implies an invaled sequence
                    //qDebug() << input << " contains invalid parentheses.";
                    return false;
                }
            }
        }
    }
    //  Checking the status of the stack to determine the
    //  validity of the string.
    if (s.empty())
    {
        //qDebug() << input << " contains valid parentheses.";
        return true;
    }
    else
    {
        //qDebug() << input << " contains invalid parentheses.";
        return false;
    }
}
// -----------------------------
// Конструктор
// -----------------------------
CodeEditor::CodeEditor(QWidget* parent) : QPlainTextEdit(parent)
{
    lineNumbersMargin = 16;
    lineNumberArea = new LineNumberArea(this);

    connect(this, &CodeEditor::blockCountChanged, this, &CodeEditor::updateLineNumberAreaWidth);
    connect(this, &CodeEditor::updateRequest, this, &CodeEditor::updateLineNumberArea);
    connect(this, &CodeEditor::cursorPositionChanged, this, &CodeEditor::highlightCurrentLine);

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}
// -----------------------------
//
// -----------------------------
int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10)
    {
        max /= 10;
        ++digits;
    }

    int space = lineNumbersMargin + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

    return space;
}

// -----------------------------
//
// -----------------------------
void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

// -----------------------------
//
// -----------------------------
void CodeEditor::updateLineNumberArea(const QRect& rect, int dy)
{
    if (dy)
    {
        lineNumberArea->scroll(0, dy);
    }
    else
    {
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
    }

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

// -----------------------------
//
// -----------------------------
void CodeEditor::resizeEvent(QResizeEvent* e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

// -----------------------------
//
// -----------------------------
void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly())
    {
        QTextEdit::ExtraSelection selection;
        QColor lineColor = QColor(0x3E,0x44,0x51);
        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }
    setExtraSelections(extraSelections);
}

// -----------------------------
//
// -----------------------------
void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent* event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), QColor(0x28,0x2C,0x34));
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());

    while (block.isValid() && top <= event->rect().bottom())
    {
        if (block.isVisible() && bottom >= event->rect().top())
        {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::white);
            painter.drawText(0, top, lineNumberArea->width()-lineNumbersMargin, fontMetrics().height(),Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}
