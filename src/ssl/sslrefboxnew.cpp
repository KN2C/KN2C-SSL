#include "sslrefboxnew.h"

SSLRefBoxNew::SSLRefBoxNew(QString ip, int port, TeamColorType color, float ball_min, WorldModel* wm, QObject *parent) :
    SSLReceiver(ip,port,parent),
    _cmdcounter(0),
    _color(color),
    _lastCMDCounter(0),
    _ball_min(ball_min),
    _wm(wm)
{
    wm->cmgs.init(color);
    connect(this, SIGNAL(newReceivedPacket(QByteArray,QString,int)), this, SLOT(readPendingPacket(QByteArray,QString,int)));
    qDebug() << "SSLRefBoxNew Initialization...";
    qDebug() << "IP: " << ip;
    qDebug() << "Port: " << port;
    qDebug() << "Color: " << (_color==COLOR_BLUE?"Blue":"Yellow");
    qDebug() << "Ball Min: " << _ball_min;
}

void SSLRefBoxNew::readPendingPacket(QByteArray data, QString ip, int port)
{
    // check for server ip (& port)
    if(ip=="" && port==0) return;

    SSL_Referee packet;
    bool ans=packet.ParseFromArray(data.data(), data.size());
    if(!ans) return;

    if(_color == COLOR_BLUE)
    {
        _wm->ref_goalie_our = packet.blue().goalie();
        _wm->ref_goalie_opp = packet.yellow().goalie();
    }
    else
    {
        _wm->ref_goalie_our = packet.yellow().goalie();
        _wm->ref_goalie_opp = packet.blue().goalie();
    }

    GameStatePacket pck;
    pck.cmd=packet.command();
    pck.cmd_counter=packet.command_counter();
    pck.goals_blue=packet.blue().score();
    pck.goals_yellow=packet.yellow().score();
    pck.time_remaining=packet.stage_time_left();

    // save last gs packet
    _wm->refgs = pck;

    // parse it
    parse(pck);
}

void SSLRefBoxNew::parse(GameStatePacket pck)
{
    bool ball_moved = _lastBallpos.loc.dist(_wm->ball.pos.loc)>_ball_min;
    _wm->cmgs.transition(pck.cmd, ball_moved);
    updategs(pck.cmd, ball_moved);

    if(pck.cmd_counter != _lastCMDCounter) // new cmd
    {
        // save ball position
        _lastBallpos = _wm->ball.pos;
        // update command counter
        _lastCMDCounter = pck.cmd_counter;
        // send signal
        emit newRefreeCommand();
    }
    else    // no new cmd
    {
    }
}

void SSLRefBoxNew::updategs(char cmd, bool)
{
    _wm->gs_last=_wm->gs;
    switch(cmd)
    {
    case 0:
        _wm->gs=STATE_Halt;
        break;
    case 1:
        _wm->gs=STATE_Stop;
        break;
    case 2:
        _wm->gs=STATE_Start;
        break;
    case 3:
        _wm->gs=STATE_ForceStart;
        break;
    case 5:
        if (_color)
            _wm->gs = STATE_Kick_off_Our;
        else
            _wm->gs = STATE_Kick_off_Opp;
        break;
    case 4:
        if (_color)
            _wm->gs = STATE_Kick_off_Opp;
        else
            _wm->gs = STATE_Kick_off_Our;
        break;
    case 7:
        if (_color)
            _wm->gs = STATE_Penalty_Our;
        else
            _wm->gs = STATE_Penalty_Opp;
        break;
    case 6:
        if (_color)
            _wm->gs = STATE_Penalty_Opp;
        else
            _wm->gs = STATE_Penalty_Our;
        break;
    case 9:
        if (_color)
            _wm->gs = STATE_Free_kick_Our;
        else
            _wm->gs = STATE_Free_kick_Opp;
        break;
    case 8:
        if (_color)
            _wm->gs = STATE_Free_kick_Opp;
        else
            _wm->gs = STATE_Free_kick_Our;
        break;
    case 11:
        if (_color)
            _wm->gs = STATE_Indirect_Free_kick_Our;
        else
            _wm->gs = STATE_Indirect_Free_kick_Opp;
        break;
    case 10:
        if (_color)
            _wm->gs = STATE_Indirect_Free_kick_Opp;
        else
            _wm->gs = STATE_Indirect_Free_kick_Our;
        break;

    case 13:
    case 12:
        _wm->gs = STATE_Halt;
        break;

    default:
        _wm->gs=STATE_Null;
        break;
    }
}

