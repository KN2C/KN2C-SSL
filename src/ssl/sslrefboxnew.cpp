#include "sslrefboxnew.h"

SSLRefBoxNew::SSLRefBoxNew(QString ip, int port, TeamColorType color, float ball_min, WorldModel* wm, QObject *parent) :
    SSLRefBox(ip, port, color, ball_min, wm, parent)
{
    qDebug() << "SSLRefBoxNew Initialization...";
}

void SSLRefBoxNew::readPendingPacket(QByteArray data, QString ip, int port)
{
    // check for server ip (& port)
    if(ip=="" && port==0) return;

    SSL_Referee packet;
    bool ans=packet.ParseFromArray(data.data(), data.size());
    if(!ans) return;

    if(!paused)
    {
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
    }

    GameStatePacket pck;

    pck.cmd_counter=packet.command_counter();
    pck.goals_blue=packet.blue().score();
    pck.goals_yellow=packet.yellow().score();
    pck.time_remaining=packet.packet_timestamp()/1000000;

    switch (packet.command())
    {
    case 0:
        pck.cmd = COMM_HALT;
        break;
    case 1:
        pck.cmd = COMM_STOP;
        break;
    case 2:
        pck.cmd = COMM_READY;
        break;
    case 3:
        pck.cmd = COMM_START;
        break;
    case 4:
        pck.cmd = COMM_KICKOFF_YELLOW;
        break;
    case 5:
        pck.cmd = COMM_KICKOFF_BLUE;
        break;
    case 6:
        pck.cmd = COMM_PENALTY_YELLOW;
        break;
    case 7:
        pck.cmd = COMM_PENALTY_BLUE;
        break;
    case 8:
        pck.cmd = COMM_DIRECT_YELLOW;
        break;
    case 9:
        pck.cmd = COMM_DIRECT_BLUE;
        break;
    case 10:
        pck.cmd = COMM_INDIRECT_YELLOW;
        break;
    case 11:
        pck.cmd = COMM_INDIRECT_BLUE;
        break;
    case 12:
        pck.cmd = COMM_TIMEOUT_YELLOW;
        break;
    case 13:
        pck.cmd = COMM_TIMEOUT_BLUE;
        break;
    case 14:
        pck.cmd = COMM_GOAL_YELLOW;
        break;
    case 15:
        pck.cmd = COMM_GOAL_BLUE;
        break;

    default:
        break;
    }

    // parse it
    parse(pck);
}


