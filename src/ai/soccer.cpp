#include "soccer.h"

Soccer::Soccer(QObject *parent) :
    QObject(parent)
{
    qDebug() << "Soccer Initialization...";
    Settings s("settings.ini");

    QString gm = s.Get("Game","Mode");
    log += "Mode : " + gm + "\n\n";

    QString rip = s.Get(gm, "RefIP");
    log += "RefIP : " + rip + "\n";

    int rport = s.Get(gm, "RefPort").toInt();
    log += "RefPort : " + QString::number(rport) + "\n";

    int rportn = s.Get(gm, "RefPortNew").toInt();
    log += "RefPortNew : " + QString::number(rportn) + "\n";

    float ball_dist = s.Get("RefereeConfig", "RefereeBall").toFloat();
    log += "RefereeBall : " + QString::number(ball_dist) + "\n\n";

    QString vip = s.Get(gm, "VisionIP");
    log += "VisionIP : " + rip + "\n";

    int vport = s.Get(gm, "VisionPort").toInt();
    log += "VisionPort : " + QString::number(vport) + "\n\n";

    QString scolor = s.Get("Team","Color");
    log += "Color : " + scolor + "\n\n";
    TeamColorType tcolor = (scolor=="Blue")?COLOR_BLUE:COLOR_YELLOW;

    QString sside = s.Get("Team","Side");
    log += "Side : " + sside + "\n\n";
    TeamSideType tside = (sside=="Right")?SIDE_RIGHT:SIDE_LEFT;

    CameraConfigType tcam = (CameraConfigType)s.Get("VisionConfig","UsingCameras").toInt();
    log += "UsingCameras : " + QString::number(tcam) + "\n\n";

    QString simip = s.Get("grSim", "CommandIP");
    log += "grSim CommandIP : " + simip + "\n";

    int simport = s.Get("grSim", "CommandPort").toInt();
    log += "grSim CommandPort : " + QString::number(simport) + "\n\n";

    QString serialport = s.Get("Transmitter","SerialPort");
    log += "SerialPort : " + serialport + "\n\n";


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

    int refusenew = s.Get(gm, "RefUseNew").toInt();
    if(refusenew == 1)
    {
        sslrefboxnew = new SSLRefBoxNew(rip, rportn, tcolor, ball_dist, wm);
        sslrefboxnew->Start();
    }
    else
    {
        sslrefbox = new SSLRefBox(rip, rport, tcolor, ball_dist, wm);
        sslrefbox->Start();
    }

    // manual Referee
    sslrefbox2 = 0;
    sslrefboxnew2 = 0;

    QString mren = s.Get("ManualRef", "Enable");
    QString mrip = s.Get("ManualRef", "RefIP");
    int mrport = s.Get("ManualRef", "RefPort").toInt();
    int mrportn = s.Get("ManualRef", "RefPortNew").toInt();
    int mrefusenew = s.Get("ManualRef", "RefUseNew").toInt();

    if(mren.toInt() == 1)
    {
        log += "Manual RefIP : " + mrip + "\n";
        log += "Manual RefPort : " + QString::number(mrport) + "\n";
        log += "Manual RefPortNew : " + QString::number(mrportn) + "\n";

        if(mrefusenew == 1)
        {
            sslrefboxnew2 = new SSLRefBoxNew(mrip, mrportn, tcolor, ball_dist, wm);
            sslrefboxnew2->paused = true;
            sslrefboxnew2->Start();
        }
        else
        {
            sslrefbox2 = new SSLRefBox(mrip, mrport, tcolor, ball_dist, wm);
            sslrefbox2->paused = true;
            sslrefbox2->Start();
        }
    }
    else
    {
        log += "ManualRef Disabled \n";
    }

    // grSim
    grsim = 0;
    if(gamemode==MODE_SIMULATION)
    {
        grsim = new grSim(simip, simport, tcolor, outputbuffer);
        grsim->Start();
        wm->isSim = true;
    }

    // serial port transmitter
    transmitter = 0;
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
