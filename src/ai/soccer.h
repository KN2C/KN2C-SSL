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
#include "sslvision.h"
#include "wpacket.h"
#include "grsim.h"
#include "outputbuffer.h"
#include "ai.h"

class Soccer : public QObject
{
    Q_OBJECT
public:
    explicit Soccer(QObject *parent = 0);

//private:
    GameModeType gamemode;
    WorldModel* wm;
    SSLVision* sslvision;
    SSLRefBox* sslrefbox;
    OutputBuffer* outputbuffer;
    Transmitter* transmitter;
    grSim* grsim;
    AI *ai;

};

#endif // SOCCER_H
