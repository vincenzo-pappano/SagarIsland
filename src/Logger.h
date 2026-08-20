#ifndef LOGGER_H
#define LOGGER_H

#include <QFile>
#include <QMutex>
#include <QSet>
#include <QString>
#include <QTextStream>
#include <QtGlobal>

#include <QLoggingCategory>

//Q_DECLARE_LOGGING_CATEGORY(loggerLog)

class Logger final
{
public:
    static Logger *self();

    static void installHandler();
    static void uninstallHandler();
    static void configureFiltering();

    bool isReady() const;
    QString fileName() const;
    QString logDirectory() const;
    QString errorString() const;

private:
    Logger();
    ~Logger();

    bool initializeFile();
    QString createUniqueFilePath(const QString &directoryPath) const;
    void registerCategory(const QString &category);
    void writeLine(const QString &line);

    static void messageHandler(
        QtMsgType type,
        const QMessageLogContext &context,
        const QString &message
        );

    static QString severityName(QtMsgType type);
    void logMessage(
        QtMsgType type,
        const QMessageLogContext &context,
        const QString &message
        );
    Q_DISABLE_COPY(Logger)

private:
    QFile m_file;
    QTextStream m_stream;
    QMutex m_mutex;
    QSet<QString> m_registeredCategories;

    QString m_logDirectory;
    QString m_errorString;

    QtMessageHandler m_previousHandler = nullptr;

    bool m_isReady = false;
    bool m_handlerInstalled = false;
};

#endif // LOGGER_H
