#ifndef SOCCER_H
#define SOCCER_H

#include <QtCore>
#include <QObject>
#include <QtCore>
#include <QDebug>
#include <QTimer>
#include <QMap>

#include "settings.h"
#include "transmitter.h"
#include "worldmodel.h"
#include "sslrefbox.h"
#include "sslrefboxnew.h"
#include "sslvision.h"
#include "wpacket.h"
#include "grsim.h"
#include "outputbuffer.h"
#include "ai.h"
#include "mapsearchnode.h"

class Soccer : public QObject
{
    Q_OBJECT
public:
    explicit Soccer(QObject *parent = 0);

//private:
    QString log;
    GameModeType gamemode;

    WorldModel* wm;
    SSLVision* sslvision;

    SSLRefBox* sslrefbox;
    SSLRefBoxNew* sslrefboxnew;

    SSLRefBox* sslrefbox2;
    SSLRefBoxNew* sslrefboxnew2;

    OutputBuffer* outputbuffer;
    Transmitter* transmitter;
    grSim* grsim;
    AI *ai;

};

#endif // SOCCER_H
