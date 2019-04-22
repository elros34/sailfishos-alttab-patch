#include "alttabhandler.h"
#include <QDebug>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/input.h>
#include <stdint.h>

AltTabHandler::AltTabHandler(QObject *parent):
    QObject(parent),
    keyboardFd(-1),
    m_altPressed(false),
    m_tabPressed(false)
{
    keyboardFd = findKeyboardFd();
    if (keyboardFd > 0) {
        QString deviceName = getDeviceName(keyboardFd);
        if (deviceName == "keypad_8960") { // Photon Q
            altKeys[0] = KEY_LEFTCTRL; // OK key
            altKeys[1] = -1;
        } else {
            altKeys[0] = KEY_LEFTALT;
            altKeys[1] = KEY_RIGHTALT;
        }
        socketNotifier = new QSocketNotifier(keyboardFd, QSocketNotifier::Read, this);
        connect(socketNotifier, SIGNAL(activated(int)), this, SLOT(readKeyboardData()));
    }
}

AltTabHandler::~AltTabHandler()
{
    socketNotifier->setEnabled(false);
    if (keyboardFd > 0)
        close(keyboardFd);
}

bool AltTabHandler::altPressed() const
{
    return m_altPressed;
}

void AltTabHandler::setAltPressed(bool altPressed)
{
    if (altPressed == m_altPressed)
        return;
    m_altPressed = altPressed;
    emit altPressedChanged();
}

bool AltTabHandler::tabPressed() const
{
    return m_tabPressed;
}

void AltTabHandler::setTabPressed(bool tabPressed)
{
    if (tabPressed == m_tabPressed)
        return;
    m_tabPressed = tabPressed;
    emit tabPressedChanged();
}

int AltTabHandler::findKeyboardFd()
{
    uint8_t evtypeMask;
    int fd = -1;
    for (int i = 0; i < 30; ++i) {
        fd = open("/dev/input/event" + QByteArray::number(i), O_RDONLY | O_NONBLOCK);
        if (fd < 0)
            break;

        if (ioctl(fd, EVIOCGBIT(0, sizeof(evtypeMask)), &evtypeMask) < 0 ) {
            close(fd);
            continue;
        }

        if (evtypeMask & (1 << EV_KEY)) {
            // every keyboard must have KEY_A = 30
            int searchKey = KEY_A;
            uint8_t keysMask[searchKey / 8 + 1];
            if (ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(keysMask)), keysMask)) {
                if (keysMask[searchKey / 8] & (1 << (searchKey % 8))) {
                    qDebug("Found keyboard at: /dev/input/event%d", i);
                    return fd;
                }
            }
        }
        close(fd);
    }
    return -1;
}

QString AltTabHandler::getDeviceName(int fd)
{
    char name[256];
    if (ioctl(fd, EVIOCGNAME(sizeof(name)), name) < 0) {
        qDebug() << "Cannot read device name";
        return QString();
    } else {
        return QString(name);
    }
}

void AltTabHandler::readKeyboardData()
{
    struct input_event inputEvent;

    if (read(keyboardFd, &inputEvent, sizeof(inputEvent)) > 0) {
        if (inputEvent.type == EV_KEY) {
            if (inputEvent.code == altKeys[0] || inputEvent.code == altKeys[1]) {
                setAltPressed(inputEvent.value);
            } else if (inputEvent.code == KEY_TAB) {
                setTabPressed(inputEvent.value);
            }
        }
    }
}

