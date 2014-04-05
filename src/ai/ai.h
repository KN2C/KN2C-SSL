#ifndef AI_H
#define AI_H

#include <QtCore>
#include <QObject>
#include "constants.h"
#include "outputbuffer.h"
#include "play.h"

class AI : public QObject
{
    Q_OBJECT
public:
    explicit AI(WorldModel* worldmodel, OutputBuffer* outputbuffer, QObject *parent = 0);

public slots:
    void Start();
    void Stop();

private:
    WorldModel *wm;
    OutputBuffer *outputbuffer;
    QTimer timer;

private slots:
    void timer_timeout();

private:
    QList<Play*> plays;

};

#endif // AI_H
