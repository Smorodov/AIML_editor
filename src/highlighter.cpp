#include "highlighter.h"
#include <QTextCharFormat>
#include <QBrush>
#include <QColor>


Highlighter::Highlighter(QTextDocument* parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    edgeTagFormat.setForeground(QBrush(QColor("#32a9dd")));
    insideTagFormat.setForeground(QColor("#eea9dd"));
    insideTagFormat.setFontWeight(QFont::Bold);
    openTag = QRegExp("<");
    closeTag = QRegExp(">");

    tagsFormat.setForeground(QColor("#bbee44"));
    tagsFormat.setFontWeight(QFont::Bold);



    QStringList keywords;
    keywords << "aiml"
        << "bot"
        << "br"
        << "category"
        << "condition"
        << "date"
        << "denormalize"
        << "eval"
        << "explode"
        << "first"
        << "formal"
        << "gender"
        << "get"
        << "id"
        << "img"
        << "input"
        << "interval"
        << "learn"
        << "learnf"
        << "li"
        << "loop"
        << "lowercase"
        << "map"
        << "normalize"
        << "oob"
        << "pattern"
        << "person"
        << "person2"
        << "program"
        << "random"
        << "request"
        << "response"
        << "rest"
        << "sentence"
        << "set"
        << "size"
        << "sr"
        << "srai"
        << "sraix"
        << "star"
        << "system"
        << "template"
        << "that"
        << "thatstar"
        << "think"
        << "topic"
        << "topicstar"
        << "uppercase";

    QStringList keywordPatterns;
    for (auto keyword : keywords)
    {
        keywordPatterns << "<\\b"+keyword+"\\b";
    }

    for (const QString& pattern : keywordPatterns)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = tagsFormat;
        startTagRules.append(rule);
    }

    QStringList keywordPatterns_end;
    for (auto keyword : keywords)
    {
        keywordPatterns_end << "</\\b" + keyword + "\\b";
    }  

    for (const QString& pattern : keywordPatterns_end)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = tagsFormat;
        endTagRules.append(rule);
    }

    multiLineCommentFormat.setForeground(Qt::darkGray);
    commentStartExpression = QRegExp("<!--");
    commentEndExpression = QRegExp("-->");

    quotationFormat.setForeground(Qt::darkGreen);
    quotes = QRegExp("\"");
}

void Highlighter::highlightBlock(const QString& text)
{
    setCurrentBlockState(Highlighter::None);

    // TAG
    int startIndex = 0;
    // If you're not within a tag,
    if (previousBlockState() != Highlighter::Tag && previousBlockState() != Highlighter::Quote)
    {
        // So we try to find the beginning of the next tag
        startIndex = openTag.indexIn(text);
    }

    // Taking the state of the previous text block
    int subPreviousTag = previousBlockState();
    while (startIndex >= 0)
    {
        // We are looking for an end-tag
        int endIndex = closeTag.indexIn(text, startIndex);

        int tagLength;
        // If the end tag is not found, then we set the block state
        if (endIndex == -1)
        {
            setCurrentBlockState(Highlighter::Tag);
            tagLength = text.length() - startIndex;
        }
        else
        {
            tagLength = endIndex - startIndex + closeTag.matchedLength();
        }

        // Set the formatting for a tag
        if (subPreviousTag != Highlighter::Tag)
        {
            // since the beginning of the tag to the end, if the previous status is not equal Tag
            setFormat(startIndex, 1, edgeTagFormat);
            setFormat(startIndex + 1, tagLength - 1, insideTagFormat);
        }
        else
        {
            // If you're already inside the tag from the start block
            // and before the end tag
            setFormat(startIndex, tagLength, insideTagFormat);
            subPreviousTag = Highlighter::None;
        }
        // Format the symbol of the end tag
        setFormat(endIndex, 1, edgeTagFormat);

        /// QUOTES ///////////////////////////////////////
        int startQuoteIndex = 0;
        // If you are not in quotation marks with the previous block
        if (previousBlockState() != Highlighter::Quote)
        {
            // So we try to find the beginning of the quotes
            startQuoteIndex = quotes.indexIn(text, startIndex);
        }

        // Highlights all quotes within the tag
        while (startQuoteIndex >= 0 && ((startQuoteIndex < endIndex) || (endIndex == -1)))
        {
            int endQuoteIndex = quotes.indexIn(text, startQuoteIndex + 1);
            int quoteLength;
            if (endQuoteIndex == -1)
            {
                // If a closing quotation mark is found, set the state for the block Quote
                setCurrentBlockState(Highlighter::Quote);
                quoteLength = text.length() - startQuoteIndex;
            }
            else
            {
                quoteLength = endQuoteIndex - startQuoteIndex + quotes.matchedLength();
            }

            if ((endIndex > endQuoteIndex) || endIndex == -1)
            {
                setFormat(startQuoteIndex, quoteLength, quotationFormat);
                startQuoteIndex = quotes.indexIn(text, startQuoteIndex + quoteLength);
            }
            else
            {
                break;
            }
        }
        //////////////////////////////////////////////////
        // Again, look for the beginning of the tag
        startIndex = openTag.indexIn(text, startIndex + tagLength);
    }

    // EDGES OF TAGS
    // Processing of color tags themselves, that is, highlight words div, p, strong etc.
    for (const HighlightingRule& rule : startTagRules)
    {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0)
        {
            int length = expression.matchedLength();
            setFormat(index + 1, length - 1, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }

    for (const HighlightingRule& rule : endTagRules)
    {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0)
        {
            int length = expression.matchedLength();
            setFormat(index + 1, 1, edgeTagFormat);
            setFormat(index + 2, length - 2, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }

    // COMMENT
    int startCommentIndex = 0;
    // If the tag is not a previous state commentary
    if (previousBlockState() != Highlighter::Comment)
    {
        // then we try to find the beginning of a comment
        startCommentIndex = commentStartExpression.indexIn(text);
    }

    // If a comment is found
    while (startCommentIndex >= 0)
    {
        // We are looking for the end of the comment
        int endCommentIndex = commentEndExpression.indexIn(text, startCommentIndex);
        int commentLength;

        // If the end is not found
        if (endCommentIndex == -1)
        {
            // That set the state Comment
            // The principle is similar to that for conventional tags
            setCurrentBlockState(Highlighter::Comment);
            commentLength = text.length() - startCommentIndex;
        }
        else
        {
            commentLength = endCommentIndex - startCommentIndex
                + commentEndExpression.matchedLength();
        }

        setFormat(startCommentIndex, commentLength, multiLineCommentFormat);
        startCommentIndex = commentStartExpression.indexIn(text, startCommentIndex + commentLength);
    }
}