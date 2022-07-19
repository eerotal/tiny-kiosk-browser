#ifndef MAIN_H
#define MAIN_H

#include <QWidget>
#include <QWebEngineView>

class TinyKioskBrowser : public QWidget {
    Q_OBJECT

public:
    TinyKioskBrowser(QWidget* parent = nullptr);
    ~TinyKioskBrowser();

    void navigate(const QUrl& url);
    void showIntroPage(void);

private:
    QWebEngineView* webView;
};

#endif
