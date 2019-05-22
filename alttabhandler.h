#ifndef ALTTABHANDLER_H
#define ALTTABHANDLER_H

#include <QObject>
#include <QSocketNotifier>

class AltTabHandler : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(AltTabHandler)
    Q_PROPERTY(bool altPressed READ altPressed WRITE setAltPressed NOTIFY altPressedChanged)
    Q_PROPERTY(bool tabPressed READ tabPressed WRITE setTabPressed NOTIFY tabPressedChanged)

public:
    AltTabHandler(QObject *parent = nullptr);
    ~AltTabHandler();

    bool altPressed() const;
    void setAltPressed(bool altPressed);

    bool tabPressed() const;
    void setTabPressed(bool tabPressed);

private:
    int keyboardFd;
    QSocketNotifier *socketNotifier;
    bool m_altPressed;
    bool m_tabPressed;
    u_int8_t altKeys[2];

private:
    int findKeyboardFd();
    QString getDeviceName(int fd);

private slots:
    void readKeyboardData();
    void detectKeyboard();

signals:
    void altPressedChanged();
    void tabPressedChanged();
};

#endif // ALTTABHANDLER_H
