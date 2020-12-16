#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>

QT_BEGIN_NAMESPACE
class QTextDocument;
class QTextCharFormat;
QT_END_NAMESPACE

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    explicit Highlighter(QTextDocument* parent = 0);

protected:
    void highlightBlock(const QString& text) Q_DECL_OVERRIDE;

private:
    // Status highlighting, which is a text box at the time of its closure
    enum States
    {
        None,       // Without highlighting
        Tag,        // Подсветка внутри тега
        Comment,    // Внутри комментария
        Quote       // Внутри кавычек, которые внутри тега
    };

    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> startTagRules;    // Formatting rules for opening tag
    QVector<HighlightingRule> endTagRules;      // Formatting rules for closing tags

    QRegExp openTag;                            // opening tag symbol - "<"
    QRegExp closeTag;                           // closing symbol tag - ">"
    QTextCharFormat edgeTagFormat;              // character formatting of openTag and closeTag
    QTextCharFormat insideTagFormat;            // Formatting text inside the tag

    QRegExp commentStartExpression;             // Regular expression of start comment
    QRegExp commentEndExpression;               // Redular expression of end comment
    QTextCharFormat multiLineCommentFormat;     // Format text inside a comment

    QRegExp quotes;                             // Regular Expression for text in quotes inside the tag
    QTextCharFormat quotationFormat;            // Formatting text in quotes inside the tag
    QTextCharFormat tagsFormat;                 // Formatting tags themselves
};

#endif // HTMLHIGHLIGHTER_H