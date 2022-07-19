#include <QBoxLayout>

#include "Core/Inc/browser.hpp"

static const char* kIntroPageHtml =
    "<html lang=\"en\">"
    "    <head></head>"
    "    <body style=\"text-align: center\">"
    "        <h1>Tiny Kiosk Browser</h1>"
    "        <p>Please specify a URL to load.</p>"
    "    </body>"
    "</html>";

TinyKioskBrowser::TinyKioskBrowser(QWidget* parent) : QWidget(parent) {
    // Setup web view.
    webView = new QWebEngineView();
    webView->setUrl(QUrl(QStringLiteral("")));

    // Setup layout.
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(webView);
    layout->setMargin(0);

    setLayout(layout);
    setWindowTitle("Tiny Kiosk Browser");
}

TinyKioskBrowser::~TinyKioskBrowser() {
    delete webView;
}

void TinyKioskBrowser::navigate(const QUrl& url) {
    webView->setUrl(url);
}

void TinyKioskBrowser::showIntroPage(void) {
    webView->setHtml(kIntroPageHtml);
}
