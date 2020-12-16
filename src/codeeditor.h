#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
QT_END_NAMESPACE

class LineNumberArea;
class tagInfo
{
public:
    enum { open, close, mono };
    QString tagName;
    int tagBegin;
    int tagEnd;
    int tagLevel;

    int tagType;

    tagInfo()
    {
        this->tagName = "";
        this->tagBegin = 0;
        this->tagEnd = 0;
        this->tagType = 0;
        tagLevel = 0;
    }

    tagInfo(QString tagName, int tagBegin, int tagEnd, int tagType)
    {
        this->tagName = tagName;
        this->tagBegin = tagBegin;
        this->tagEnd = tagEnd;
        this->tagType = tagType;
        tagLevel = 0;
    }

    ~tagInfo()
    {

    }
};
// -----------------------------
//
// -----------------------------
class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    CodeEditor(QWidget* parent = nullptr);
    void textToSingleLine(QString& txt);
    std::vector<tagInfo> getTagMap(QString text);
    bool valid_paren(std::vector<tagInfo> input, int& ind);

    void lineNumberAreaPaintEvent(QPaintEvent* event);
    int lineNumberAreaWidth();
    int lineNumbersMargin;
protected:
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect& rect, int dy);

private:
    QWidget* lineNumberArea;
};

// -----------------------------
//
// -----------------------------
class LineNumberArea : public QWidget
{
public:
    LineNumberArea(CodeEditor* editor) : QWidget(editor), codeEditor(editor)
    {
    }

    QSize sizeHint() const override
    {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent* event) override
    {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    CodeEditor* codeEditor;
};
#endif
