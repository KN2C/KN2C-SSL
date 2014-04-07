#ifndef SSLREFBOXNEW_H
#define SSLREFBOXNEW_H

#include "commands.h"   //from sslrefbox
#include "game_state.h" //from sslrefbox client
#include "ref_protocol.h"   //referee:protocol

#include "sslrefbox/messages/referee.pb.h"

#include "sslreceiver.h"
#include "base.h"
#include "worldmodel.h"

class SSLRefBoxNew : public SSLReceiver
{
    Q_OBJECT

public:
    explicit SSLRefBoxNew(QString ip, int port, TeamColorType color, float ball_min, WorldModel* wm, QObject *parent = 0);

private:
    int _cmdcounter;
    TeamColorType _color;
    unsigned char _lastCMDCounter;
    float _ball_min;
    WorldModel* _wm;
    Position _lastBallpos;
    void parse(GameStatePacket pck);
    void updategs(char cmd, bool ball_moved);

signals:
    void newRefreeCommand();

public slots:


private slots:
    void readPendingPacket(QByteArray data, QString ip, int port);

};

#endif // SSLREFBOXNEW_H
