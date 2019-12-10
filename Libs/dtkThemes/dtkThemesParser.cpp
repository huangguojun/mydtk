// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "dtkThemesParser.h"

#include <QtGui>

// // /////////////////////////////////////////////////////////////////////////////
// // Helper functions
// // /////////////////////////////////////////////////////////////////////////////

bool dtkThemesParserSorter(const QString& s1, const QString& s2)
{
    return s1.length() > s2.length();
}

// ///////////////////////////////////////////////////////////////////
// dtkThemesParserPrivate
// ///////////////////////////////////////////////////////////////////

class dtkThemesParserPrivate
{
public:
    QHash<QString, QString> values;
};

// ///////////////////////////////////////////////////////////////////
// dtkThemesParser
// ///////////////////////////////////////////////////////////////////

dtkThemesParser::dtkThemesParser(QObject *parent) : QObject(parent)
{
    d = new dtkThemesParserPrivate;
}

dtkThemesParser::dtkThemesParser(const QString& theme, QObject *parent) : QObject(parent)
{
    d = new dtkThemesParserPrivate;

    this->parse(QString(":dtk-themes/mixins/dtkTheme%1.qless").arg(theme));
}

dtkThemesParser::~dtkThemesParser(void)
{
    delete d;
}

QString dtkThemesParser::parse(const QString& path)
{
    auto eval = [=] (const QString& source, bool compound = false) -> QString {

       QString declaration;
       QString definition = source;

       if (compound) {

           QStringList statement = source.split(":");

           declaration = statement.takeFirst();
           declaration += ":";
           definition = statement.join(":");
       }

       QStringList keys = d->values.keys(); qSort(keys.begin(), keys.end(), dtkThemesParserSorter);

       for(QString key : keys) {
           if(d->values.contains(key))
               definition.replace(key, d->values[key]);
           else
               qDebug() << "AH VOILA!!!" << key;
       }

       return declaration + definition;
    };

    QString source;
    QString destination;

    QFile stylesheet(path);

    if (!stylesheet.open(QIODevice::ReadOnly))
        return QString();

    QTextStream stream(&stylesheet);

    QString line;
    QString style;

    QRegularExpression s_expression("^;; (?<key>@([a-z])+(\\d)*(\\-)*([a-z])*):\\s*(?<value>#(.)+).*$");
    QRegularExpression c_expression("^;; (?<key>@([a-z])+(\\d)*(\\-)*([a-z])*):\\s*(?<value>@([a-z])+(\\d)*(\\-)*([a-z])*).*$");

    while(stream.readLineInto(&line)) {

        QRegularExpressionMatch s_match = s_expression.match(line);
        QRegularExpressionMatch c_match = c_expression.match(line);

        if (c_match.hasMatch())
            s_match = s_expression.match(eval(line, true));

        if (s_match.hasMatch()) {
            d->values.insert(s_match.captured("key"), s_match.captured("value"));

            // qDebug() << Q_FUNC_INFO << s_match.captured("key") << s_match.captured("value");

            if (s_match.captured("key") == "@hl")
                d->values.insert("@hlalt", QColor(d->values["@hl"]).darker(125).name());
        }

        if (line.startsWith("=yield")) {

            QFile c_less(":dtk-themes/style/common.qless");

            if (!c_less.open(QIODevice::ReadOnly))
                return QString();

            QTextStream c_stream(&c_less);

            QString c_line;

            while(c_stream.readLineInto(&c_line)) {

                if (c_line.startsWith("=yield")) {

                    QFile s_less(":dtk-themes/style/" + c_line.remove("=yield").trimmed());

                    if (!s_less.open(QIODevice::ReadOnly)) {
                        qCritical() << Q_FUNC_INFO << "Cannot open file" << s_less.fileName();
                        return QString();
                    }

                    QTextStream s_stream(&s_less);

                    style.append(s_stream.readAll());
                    style.append("\n");
                }
            }
        }
    }

    stream.seek(0);

    destination = stylesheet.readAll();

    stylesheet.close();

    destination.replace("=yield", style);

    destination = eval(destination);

    QTextStream o_stream(&destination);

    // QRegularExpression t_expression("^.*(?<operator>(α)).*$");
    QRegularExpression o_expression("^.*(?<operator>(alpha))\\((?<value>(#([a-z]|[A-Z]|[0-9])+)),\\s*(?<operand>([a-x]|[0-9])+).*\\).*$");

    QHash<QString, QString> substitutions;

    while(o_stream.readLineInto(&line)) {

        // QRegularExpressionMatch t_match = t_expression.match(line);
        QRegularExpressionMatch o_match = o_expression.match(line);

        // if(t_match.hasMatch()) {
        //     qDebug() << Q_FUNC_INFO << "Got an OP for:" << path << "in:" << line;
        // }

        if(o_match.hasMatch()) {

            // qDebug() << Q_FUNC_INFO << "Strogin an OP substitution for:" << path << "in:" << line;

            QString s = QString("%1(%2, %3)").arg(o_match.captured("operator")).arg(o_match.captured("value")).arg(o_match.captured("operand"));
            QString d = QString("#%1%2").arg(o_match.captured("operand")).arg(o_match.captured("value").remove("#"));

            substitutions[s] = d;
        }
    }

    for(QString s : substitutions.keys())
        destination.replace(s, substitutions[s]);

    return destination;
}

const QColor dtkThemesParser::color(const QString& key) const
{
    return QColor(d->values[key]);
}

const QHash<QString, QString>& dtkThemesParser::values(void) const
{
    return d->values;
}

//
// dtkThemesParser.cpp ends here
