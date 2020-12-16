#ifndef AIMLPARSER_H
#define AIMLPARSER_H

#include <QtXml/QDomNode>
#include <QMap>
#include <QList>
#include <QStringList>
#include <QRegExp>
#include <QObject>

#define MAX_LIST_LENGTH 50
#define MAX_RECURSION   50

class QTextStream;

struct Node;

struct Leaf
{
    Node *parent;
    QDomNode tmplate;
    QString topic;
    QString that;
    Leaf();
};

struct Node
{
    Node *parent;
    QString word;
    QList<Node*> childs;
    QList<Leaf*> leafs;
    Node();
    ~Node();
    void clear();
    void debug(QTextStream* logStream, uint indent = 0);
    bool match(QStringList::const_iterator, const QStringList&,
               const QString&, const QString&, QStringList &, QStringList &, Leaf *&);
};

class AIMLParser : public QObject
{
    Q_OBJECT

public:
    AIMLParser(bool displayTree = false);
    virtual ~AIMLParser();
    void clear();
    bool loadAimlFromString(const QString& data);
    bool loadAiml(const QString&);
    bool loadAIMLSet(const QString &);
    bool loadVars(const QString&, const bool&);
    bool saveVars(const QString&);
    QString executeCommand(const QString& commandStr);
    QString getResponse(QString, const bool & = false);
    QString logString;
    QTextStream* logStream;

    QStringList inputList;
    QList<QStringList> thatList;
    QMap<QString, QString> parameterValue;
    QMap<QString, QString> botVarValue;
    QList<QRegExp> subOld;
    QStringList subNew;
   // bool startServer(uint);
private:
    bool loadSubstitutions(const QString&);
    QString resolveNode(QDomNode*, const QStringList & = QStringList(),
                        const QStringList & = QStringList(), const QStringList & = QStringList());
    void parseCategory(QDomNode*);
    void normalizeString(QString &);    
private:

    Node root;
    int indent;    
    bool displayTree;
    quint16 blockSize;
    QString currentPath;
signals:
    //void aboutToLoadAIMLFile(const QString&);
private slots:

};

#endif
