#include "Logger.h"
#include "LoggerConfig.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>
#include <QMutexLocker>

#include <cstdio>


Q_LOGGING_CATEGORY(loggerLog, "logger")

Logger::Logger() : m_stream(&m_file)
{
    m_isReady = initializeFile();
}

Logger::~Logger()
{
    if (m_handlerInstalled) {
        qInstallMessageHandler(
            m_previousHandler
            );

        m_handlerInstalled = false;
        m_previousHandler = nullptr;
    }

    if (m_file.isOpen()) {
        m_stream.flush();
        m_file.flush();
        m_file.close();
    }
}

Logger *Logger::self()
{
    static Logger instance;
    return &instance;
}

bool Logger::isReady() const
{
    return m_isReady;
}

QString Logger::fileName() const
{
    return m_file.fileName();
}

QString Logger::logDirectory() const
{
    return m_logDirectory;
}

QString Logger::errorString() const
{
    return m_errorString;
}

bool Logger::initializeFile()
{
#if LOG_WRITE_TO_FILE
    const QString applicationDataPath =
        QStandardPaths::writableLocation(
            QStandardPaths::AppLocalDataLocation
            );

    if (applicationDataPath.isEmpty()) {
        m_errorString =
            QStringLiteral("QStandardPaths returned an empty path.");

        return false;
    }

    m_logDirectory =
        QDir(applicationDataPath).filePath(
            QStringLiteral("Logs")
            );

    const QString logFilePath =
        createUniqueFilePath(m_logDirectory);

    m_file.setFileName(logFilePath);

#if LOG_FORCE_INITIALIZATION_FAILURE
    m_errorString =
        QStringLiteral(
            "Forced logger initialization failure for testing."
            );

    return false;
#endif
    if (!QDir().mkpath(m_logDirectory)) {
        m_errorString =
            QStringLiteral("Unable to create log directory: %1")
                .arg(QDir::toNativeSeparators(m_logDirectory));

        return false;
    }

    if (!m_file.open(QIODevice::WriteOnly |
                     QIODevice::Text)) {
        m_errorString = m_file.errorString();
        return false;
    }

    m_stream.setCodec("UTF-8");

    return true;
#else
    return true;
#endif
}

QString Logger::createUniqueFilePath(
    const QString &directoryPath) const
{
    QString prefix =
        QCoreApplication::applicationName();

    if (prefix.isEmpty()) {
        prefix = QStringLiteral("EchoBoomMaster");
    }

    const QString timestamp =
        QDateTime::currentDateTime().toString(
            QStringLiteral("yyyy-MM-dd_HH-mm-ss")
            );

    const QString baseName =
        QStringLiteral("%1_%2")
            .arg(prefix, timestamp);

    QDir directory(directoryPath);

    QString candidate =
        directory.filePath(baseName + QStringLiteral(".log"));

    int suffix = 2;

    while (QFileInfo::exists(candidate)) {
        candidate =
            directory.filePath(
                QStringLiteral("%1_%2.log")
                    .arg(baseName)
                    .arg(suffix)
                );

        ++suffix;
    }

    return candidate;
}

void Logger::installHandler()
{
    Logger *logger = self();

    if (!logger->m_isReady ||
        logger->m_handlerInstalled) {
        return;
    }

    logger->m_previousHandler =
        qInstallMessageHandler(
            &Logger::messageHandler
            );

    logger->m_handlerInstalled = true;

    qCInfo(loggerLog)
        << "Qt message handler installed";
}

void Logger::uninstallHandler()
{
    Logger *logger = self();

    if (!logger->m_handlerInstalled) {
        return;
    }

    qInstallMessageHandler(
        logger->m_previousHandler
        );

    logger->m_previousHandler = nullptr;
    logger->m_handlerInstalled = false;
}

void Logger::messageHandler(
    QtMsgType type,
    const QMessageLogContext &context,
    const QString &message)
{
    Logger *logger = Logger::self();

    if (!logger->isReady()) {
        return;
    }

    logger->logMessage(
        type,
        context,
        message
        );
}

QString Logger::severityName(QtMsgType type)
{
    switch (type) {
    case QtDebugMsg:
        return QStringLiteral("DEBUG");

    case QtInfoMsg:
        return QStringLiteral("INFO");

    case QtWarningMsg:
        return QStringLiteral("WARNING");

    case QtCriticalMsg:
        return QStringLiteral("CRITICAL");

    case QtFatalMsg:
        return QStringLiteral("FATAL");
    }

    return QStringLiteral("UNKNOWN");
}

void Logger::logMessage(
    QtMsgType type,
    const QMessageLogContext &context,
    const QString &message)
{
    const QString category =
        context.category &&
                context.category[0] != '\0'
            ? QString::fromUtf8(context.category)
            : QStringLiteral("default");

    registerCategory(category);

    const QString line =
        QStringLiteral("[%1] [%2] %3")
            .arg(
                severityName(type),
                category,
                message
                );

    writeLine(line);
}

void Logger::writeLine(const QString &line)
{
    QMutexLocker locker(&m_mutex);

#if LOG_WRITE_TO_FILE
    if (m_file.isOpen()) {
        m_stream << line << '\n';

#if LOG_FLUSH_EVERY_MESSAGE
        m_stream.flush();
        m_file.flush();
#endif
    }
#endif

#if LOG_WRITE_TO_CONSOLE
    const QByteArray encoded = line.toUtf8();

    std::fprintf(
        stdout,
        "%s\n",
        encoded.constData()
        );

    std::fflush(stdout);
#endif
}

void Logger::configureFiltering()
{
#if LOG_ENABLE_HEAVY_DEBUG
    QLoggingCategory::setFilterRules(
        QStringLiteral("heavy.debug=true")
        );
#else
    QLoggingCategory::setFilterRules(
        QStringLiteral("heavy.debug=false")
        );
#endif
}

void Logger::registerCategory(const QString &category)
{
#if LOG_REGISTER_CATEGORIES
    bool isNewCategory = false;

    {
        QMutexLocker locker(&m_mutex);

        if (!m_registeredCategories.contains(category)) {
            m_registeredCategories.insert(category);
            isNewCategory = true;
        }
    }

    if (isNewCategory) {
        writeLine(
            QStringLiteral(
                "[INFO] [logger] "
                "*** Registered logging category: %1"
                ).arg(category)
            );
    }
#else
    Q_UNUSED(category)
#endif
}