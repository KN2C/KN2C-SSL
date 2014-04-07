#include "soccer.h"

Soccer::Soccer(QObject *parent) :
    QObject(parent)
{
    qDebug() << "Soccer Initialization...";
    Settings s("settings.ini");

    QString gm = s.Get("Game","Mode");
    log += "Mode : " + gm + "\n";

    QString rip = s.Get(gm, "RefIP");
    log += "RefIP : " + rip + "\n";

    int rport = s.Get(gm, "RefPort").toInt();
    log += "RefPort : " + QString::number(rport) + "\n";

    int rportn = s.Get(gm, "RefPortNew").toInt();
    log += "RefPortNew : " + QString::number(rportn) + "\n";

    QString vip = s.Get(gm, "VisionIP");
    log += "VisionIP : " + rip + "\n";

    int vport = s.Get(gm, "VisionPort").toInt();
    log += "VisionPort : " + QString::number(vport) + "\n";

    QString scolor = s.Get("Team","Color");
    log += "Color : " + scolor + "\n";
    TeamColorType tcolor = (scolor=="Blue")?COLOR_BLUE:COLOR_YELLOW;

    QString sside = s.Get("Team","Side");
    log += "Side : " + sside + "\n";
    TeamSideType tside = (sside=="Right")?SIDE_RIGHT:SIDE_LEFT;

    CameraConfigType tcam = (CameraConfigType)s.Get("VisionConfig","UsingCameras").toInt();
    log += "UsingCameras : " + QString::number(tcam) + "\n";

    QString simip = s.Get("grSim", "CommandIP");
    log += "grSim CommandIP : " + simip + "\n";

    int simport = s.Get("grSim", "CommandPort").toInt();
    log += "grSim CommandPort : " + QString::number(simport) + "\n";

    QString serialport = s.Get("Transmitter","SerialPort");
    log += "SerialPort : " + serialport + "\n";

    float ball_dist = s.Get("RefereeConfig", "RefereeBall").toFloat();
    log += "RefereeBall : " + QString::number(ball_dist) + "\n";

    // Game mode
    gamemode = (gm=="Simulation")?MODE_SIMULATION:MODE_REAL;
    qDebug() << "Game mode: " << gm << "\n";

    // output buffer
    outputbuffer = new OutputBuffer;

    // Vison & referee
    wm = new WorldModel(outputbuffer);
    MapSearchNode::wm = wm;
    sslvision = new SSLVision(vip, vport, tcolor, tside, tcam, wm);
    sslvision->Start();

    sslrefbox = 0;
    sslrefboxnew = 0;

    if(0)
    {
        sslrefbox = new SSLRefBox(rip, rport, tcolor, ball_dist, wm);
        sslrefbox->Start();
    }
    else
    {
        sslrefboxnew = new SSLRefBoxNew(rip, rportn, tcolor, ball_dist, wm);
        sslrefboxnew->Start();
    }

    // grSim
    if(gamemode==MODE_SIMULATION)
    {
        grsim = new grSim(simip, simport, tcolor, outputbuffer);
        grsim->Start();
        wm->isSim = true;
    }

    // serial port transmitter
    if(gamemode==MODE_REAL)
    {
        transmitter = new Transmitter(serialport, outputbuffer, wm);
        transmitter->Start();
        wm->isSim = false;
    }

    // AI
    ai = new AI(wm, outputbuffer, this);
    ai->Start();
}
