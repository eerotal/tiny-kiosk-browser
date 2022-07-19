#include <QCoreApplication>
#include <QApplication>
#include <QCommandLineParser>

#include "Core/Inc/browser.hpp"

int main(int argc, char *argv[]) {
    QCoreApplication::setApplicationVersion(VERSION);
    QApplication app(argc, argv);

    /*
     * Setup command line parsing.
     */

    QCommandLineParser parser;

    parser.setApplicationDescription("Kiosk Browser");
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addPositionalArgument("url", "Destination URL.");

    QCommandLineOption userOption(
        QStringList() << "u" << "username",
        "Specify a <username> to use for HTTP Basic Auth.",
        "username"
    );
    parser.addOption(userOption);

    QCommandLineOption passwordOption(
        QStringList() << "p" << "password",
        "Specify a <password> to use for HTTP Basic Auth.",
        "passsword"
    );
    parser.addOption(passwordOption);

    parser.process(app);

    /*
     * Initialize the browser.
     */

    TinyKioskBrowser browserWidget;
    QStringList posArgs = parser.positionalArguments();

    // Set destination URL.
    if (!posArgs.isEmpty()) {
        QUrl url(posArgs.at(0));

        qDebug() << "Navigating to" << url.toString() << "(possible username/password redacted for security)";

        if (url.scheme().isEmpty()) {
            qWarning() << "URL scheme not provided. Falling back to https://.";
            url.setScheme("https");
        }

        // Set HTTP Basic Auth credentials if provided.
        if (parser.isSet(userOption) != parser.isSet(passwordOption)) {
            qCritical("Please provide both a username and a password for HTTP Basic Auth.");
            return 1;
        } else if (parser.isSet(userOption) && parser.isSet(passwordOption)){
            url.setUserName(parser.value(userOption));
            url.setPassword(parser.value(passwordOption));
        }

        browserWidget.navigate(url);
    } else {
        browserWidget.showIntroPage();
    }

    browserWidget.show();

    return app.exec();
}
