#ifndef SSLREFBOXNEW_H
#define SSLREFBOXNEW_H

#include "sslrefbox.h"
#include "sslrefbox/messages/referee.pb.h"

class SSLRefBoxNew : public SSLRefBox
{
    Q_OBJECT

public:
    explicit SSLRefBoxNew(QString ip, int port, TeamColorType color, float ball_min, WorldModel* wm, QObject *parent = 0);

private slots:
    virtual void readPendingPacket(QByteArray data, QString ip, int port);

};

#endif // SSLREFBOXNEW_H
