#ifndef TRANSMITTER_H
#define TRANSMITTER_H

#include <QObject>
#include <QTimer>
#include <QSerialPort>
#include <iostream>
using namespace std;

#include "base.h"
#include "constants.h"
#include "outputbuffer.h"

class OutputBuffer;

#define PRE_0 -2
#define PRE_1 -1
#define PRE_2 0

class Transmitter : public QObject
{
    Q_OBJECT
public:
    explicit Transmitter(QString port, OutputBuffer* buffer, QObject *parent = 0);

private:
    QSerialPort _serialport;
    QTimer _timer;
    bool _state;
    OutputBuffer* _buffer;
    char readbuffer[120];
    int read_state;

public slots:
    void Start();
    void Stop();

private slots:
    void sendPacket();
    void serialRead();

};

#endif // TRANSMITTER_H
