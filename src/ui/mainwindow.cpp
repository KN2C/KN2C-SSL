#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(Soccer *soccer, QWidget *parent) :
    QMainWindow(parent),
    timer(this),
    sc(soccer),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ui->txtLog->append(QSerialPort::);
    _render = new RenderArea(soccer);
    ui->gridRender->addWidget(_render);
    this->on_btnLoadVars_clicked();
    connect(&timer, SIGNAL(timeout()), this, SLOT(timer_timeout()));
    timer.start(100);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::timer_timeout()
{
    //qDebug() << "ui timer";
    ui->txtLog->setText(sc->log);

    ui->txtVisionSpeed->setText(QString::number(sc->sslvision->FPS()));
    ui->txtVisionSpeed_0->setText(QString::number(sc->sslvision->getFPS(0)));
    ui->txtVisionSpeed_1->setText(QString::number(sc->sslvision->getFPS(1)));
    ui->txtAIfps->setText(QString::number(sc->ai->fps.FPS()));
    if(sc->transmitter)
        ui->txtTrans->setText(QString::number(sc->transmitter->fps.FPS()));
    if(sc->grsim)
        ui->txtgrSim->setText(QString::number(sc->grsim->fps.FPS()));

    bool pause1;
    bool pause2;
    if(ui->rdoRefMain->isChecked())
    {
        pause1 = false;
        pause2 = true;
    }
    if(ui->rdoRefManual->isChecked())
    {
        pause1 = true;
        pause2 = false;
    }

    GameStatePacket refgs;
    GameStatePacket refgs2;
    if(sc->sslrefbox)
    {
        sc->sslrefbox->paused = pause1;
        ui->txtRefreeSpeed->setText("old : " + QString::number(sc->sslrefbox->FPS()));
        refgs = sc->sslrefbox->refgs;
    }
    if(sc->sslrefboxnew)
    {
        sc->sslrefboxnew->paused = pause1;
        ui->txtRefreeSpeed->setText("new : " + QString::number(sc->sslrefboxnew->FPS()));
        refgs = sc->sslrefboxnew->refgs;
    }
    if(sc->sslrefbox2)
    {
        sc->sslrefbox2->paused = pause2;
        ui->txtRefreeSpeed_2->setText("old : " + QString::number(sc->sslrefbox2->FPS()));
        refgs2 = sc->sslrefbox2->refgs;
    }
    if(sc->sslrefboxnew2)
    {
        sc->sslrefboxnew2->paused = pause2;
        ui->txtRefreeSpeed_2->setText("new : " + QString::number(sc->sslrefboxnew2->FPS()));
        refgs2 = sc->sslrefboxnew2->refgs;
    }

    ui->txtTime->setText(QString::number(sc->wm->time));
    ui->txtTimeBall->setText(QString::number(sc->wm->ball.time));

    QString str_refgs = QString("") + refgs.cmd +
            " : " +QString::number(refgs.cmd_counter) +
            " : " + QString::number(refgs.goals_blue) +
            " : " + QString::number(refgs.goals_yellow) +
            " : " + QString::number(refgs.time_remaining);
    ui->txtrefgs->setText(str_refgs);

    QString str_refgs2 = QString("") + refgs2.cmd +
            " : " +QString::number(refgs2.cmd_counter) +
            " : " + QString::number(refgs2.goals_blue) +
            " : " + QString::number(refgs2.goals_yellow) +
            " : " + QString::number(refgs2.time_remaining);
    ui->txtrefgs_2->setText(str_refgs2);

    ui->txtcmgs_1->setText(QString("gameOn : ") + (sc->wm->cmgs.gameOn()?"1":"0"));
    ui->txtcmgs_2->setText(QString("allowedNearBall : ") + (sc->wm->cmgs.allowedNearBall()?"1":"0"));
    ui->txtcmgs_3->setText(QString("canKickBall : ") + (sc->wm->cmgs.canKickBall()?"1":"0"));

    ui->txtcmgs_4->setText(QString("restart : ") + (sc->wm->cmgs.restart()?"1":"0"));
    ui->txtcmgs_5->setText(QString("ourRestart : ") + (sc->wm->cmgs.ourRestart()?"1":"0"));
    ui->txtcmgs_6->setText(QString("theirRestart : ") + (sc->wm->cmgs.theirRestart()?"1":"0"));

    ui->txtcmgs_7->setText(QString("kickoff : ") + (sc->wm->cmgs.kickoff()?"1":"0"));
    ui->txtcmgs_8->setText(QString("ourKickoff : ") + (sc->wm->cmgs.ourKickoff()?"1":"0"));
    ui->txtcmgs_9->setText(QString("theirKickoff : ") + (sc->wm->cmgs.theirKickoff()?"1":"0"));

    ui->txtcmgs_10->setText(QString("penaltyKick : ") + (sc->wm->cmgs.penaltyKick()?"1":"0"));
    ui->txtcmgs_11->setText(QString("ourPenaltyKick : ") + (sc->wm->cmgs.ourPenaltyKick()?"1":"0"));
    ui->txtcmgs_12->setText(QString("theirPenaltyKick : ") + (sc->wm->cmgs.theirPenaltyKick()?"1":"0"));

    ui->txtcmgs_13->setText(QString("directKick : ") + (sc->wm->cmgs.directKick()?"1":"0"));
    ui->txtcmgs_14->setText(QString("ourDirectKick : ") + (sc->wm->cmgs.ourDirectKick()?"1":"0"));
    ui->txtcmgs_15->setText(QString("theirDirectKick : ") + (sc->wm->cmgs.theirDirectKick()?"1":"0"));

    ui->txtcmgs_16->setText(QString("indirectKick : ") + (sc->wm->cmgs.indirectKick()?"1":"0"));
    ui->txtcmgs_17->setText(QString("ourIndirectKick : ") + (sc->wm->cmgs.ourIndirectKick()?"1":"0"));
    ui->txtcmgs_18->setText(QString("theirIndirectKick : ") + (sc->wm->cmgs.theirIndirectKick()?"1":"0"));

    ui->txtcmgs_19->setText(QString("freeKick : ") + (sc->wm->cmgs.freeKick()?"1":"0"));
    ui->txtcmgs_20->setText(QString("ourFreeKick : ") + (sc->wm->cmgs.ourFreeKick()?"1":"0"));
    ui->txtcmgs_21->setText(QString("theiFreeKick : ") + (sc->wm->cmgs.theiFreeKick()?"1":"0"));

    ui->txtcmgs_22->setText(QString("canMove : ") + (sc->wm->cmgs.canMove()?"1":"0"));

    QString ball  = QString::number(sc->wm->ball.pos.loc.x,'f',2) + " , " + QString::number(sc->wm->ball.pos.loc.y,'f',2);
    QString ballv = QString::number(sc->wm->ball.vel.loc.x,'f',2) + " , " + QString::number(sc->wm->ball.vel.loc.y,'f',2);
    ball = "( " + ball + " ) , ( " + ballv + " ) ";

    // WM
    ui->txtWM->setText("");
    ui->txtWM->append("time : " + QString::number(sc->wm->time));
    ui->txtWM->append("ball : " + ball + " [" + QString::number(sc->wm->ball.isValid?1:0) + "] ");

    ui->txtWM->append("");
    for(int i=0; i<PLAYERS_MAX_NUM; ++i)
    {
        QString r = QString::number(sc->wm->ourRobot[i].pos.loc.x,'f',2) + " , " + QString::number(sc->wm->ourRobot[i].pos.loc.y,'f',2) + " : " + QString::number(sc->wm->ourRobot[i].pos.dir,'f',2);
        QString s = QString::number(sc->wm->ourRobot[i].vel.loc.x,'f',2) + " , " + QString::number(sc->wm->ourRobot[i].vel.loc.y,'f',2) + " : " + QString::number(sc->wm->ourRobot[i].vel.dir,'f',2);
        QString id = QString::number(i);
        if(id.length()<2) id = "0" + id;
        ui->txtWM->append("our[" + id + "]  : ( " + r + " ) , ( " + s + " ) (" + QString::number(sc->wm->ourRobot[i].vel.loc.length(),'f',2) + ") [" + QString::number(sc->wm->ourRobot[i].isValid) + "] ");
    }

    ui->txtWM->append("");
    for(int i=0; i<PLAYERS_MAX_NUM; ++i)
    {
        QString r = QString::number(sc->wm->oppRobot[i].pos.loc.x,'f',2) + " , " + QString::number(sc->wm->oppRobot[i].pos.loc.y,'f',2) + " : " + QString::number(sc->wm->oppRobot[i].pos.dir,'f',2);
        QString s = QString::number(sc->wm->oppRobot[i].vel.loc.x,'f',2) + " , " + QString::number(sc->wm->oppRobot[i].vel.loc.y,'f',2) + " : " + QString::number(sc->wm->oppRobot[i].vel.dir,'f',2);
        QString id = QString::number(i);
        if(id.length()<2) id = "0" + id;
        ui->txtWM->append("opp[" + id + "] :  ( " + r + " ) , ( " + s + " ) (" + QString::number(sc->wm->oppRobot[i].vel.loc.length(),'f',2) + ") [" + QString::number(sc->wm->oppRobot[i].isValid) + "] ");
    }
    ui->txtWM->append("");
    ui->txtWM->append("ref_goalie_our : " + QString::number(sc->wm->ref_goalie_our));
    ui->txtWM->append("ref_goalie_opp : " + QString::number(sc->wm->ref_goalie_opp));
    ui->txtWM->append("");

    //AI
    if(sc->ai->getCurrentPlay())
    {
        ui->txtPlay->setText(sc->ai->getCurrentPlay()->getName());
        ui->txtPlay_2->setText(sc->ai->getCurrentPlay()->getName());
    }
    else
    {
        ui->txtPlay->setText("NULL");
        ui->txtPlay_2->setText("NULL");
    }
    if(sc->ai->getCurrentTactic(0))
        ui->txtTactic_0 ->setText(sc->ai->getCurrentTactic(0) ->getName());
    else ui->txtTactic_0 ->setText("NULL");
    if(sc->ai->getCurrentTactic(1))
        ui->txtTactic_1 ->setText(sc->ai->getCurrentTactic(1) ->getName());
    else ui->txtTactic_1 ->setText("NULL");
    if(sc->ai->getCurrentTactic(2))
        ui->txtTactic_2 ->setText(sc->ai->getCurrentTactic(2) ->getName());
    else ui->txtTactic_2 ->setText("NULL");
    if(sc->ai->getCurrentTactic(3))
        ui->txtTactic_3 ->setText(sc->ai->getCurrentTactic(3) ->getName());
    else ui->txtTactic_3 ->setText("NULL");
    if(sc->ai->getCurrentTactic(4))
        ui->txtTactic_4 ->setText(sc->ai->getCurrentTactic(4) ->getName());
    else ui->txtTactic_4 ->setText("NULL");
    if(sc->ai->getCurrentTactic(5))
        ui->txtTactic_5 ->setText(sc->ai->getCurrentTactic(5) ->getName());
    else ui->txtTactic_5 ->setText("NULL");
    if(sc->ai->getCurrentTactic(6))
        ui->txtTactic_6 ->setText(sc->ai->getCurrentTactic(6) ->getName());
    else ui->txtTactic_6 ->setText("NULL");
    if(sc->ai->getCurrentTactic(7))
        ui->txtTactic_7 ->setText(sc->ai->getCurrentTactic(7) ->getName());
    else ui->txtTactic_7 ->setText("NULL");
    if(sc->ai->getCurrentTactic(8))
        ui->txtTactic_8 ->setText(sc->ai->getCurrentTactic(8) ->getName());
    else ui->txtTactic_8 ->setText("NULL");
    if(sc->ai->getCurrentTactic(9))
        ui->txtTactic_9 ->setText(sc->ai->getCurrentTactic(9) ->getName());
    else ui->txtTactic_9 ->setText("NULL");
    if(sc->ai->getCurrentTactic(10))
        ui->txtTactic_10->setText(sc->ai->getCurrentTactic(10)->getName());
    else ui->txtTactic_10 ->setText("NULL");
    if(sc->ai->getCurrentTactic(11))
        ui->txtTactic_11->setText(sc->ai->getCurrentTactic(11)->getName());
    else ui->txtTactic_11 ->setText("NULL");

    ui->txtAgent_0->setText(QString::number(sc->wm->ourRobot[0].Role) + " : " + QString::number(sc->wm->ourRobot[0].Status));
    ui->txtAgent_1->setText(QString::number(sc->wm->ourRobot[1].Role) + " : " + QString::number(sc->wm->ourRobot[1].Status));
    ui->txtAgent_2->setText(QString::number(sc->wm->ourRobot[2].Role) + " : " + QString::number(sc->wm->ourRobot[2].Status));
    ui->txtAgent_3->setText(QString::number(sc->wm->ourRobot[3].Role) + " : " + QString::number(sc->wm->ourRobot[3].Status));
    ui->txtAgent_4->setText(QString::number(sc->wm->ourRobot[4].Role) + " : " + QString::number(sc->wm->ourRobot[4].Status));
    ui->txtAgent_5->setText(QString::number(sc->wm->ourRobot[5].Role) + " : " + QString::number(sc->wm->ourRobot[5].Status));
    ui->txtAgent_6->setText(QString::number(sc->wm->ourRobot[6].Role) + " : " + QString::number(sc->wm->ourRobot[6].Status));
    ui->txtAgent_7->setText(QString::number(sc->wm->ourRobot[7].Role) + " : " + QString::number(sc->wm->ourRobot[7].Status));
    ui->txtAgent_8->setText(QString::number(sc->wm->ourRobot[8].Role) + " : " + QString::number(sc->wm->ourRobot[8].Status));
    ui->txtAgent_9->setText(QString::number(sc->wm->ourRobot[9].Role) + " : " + QString::number(sc->wm->ourRobot[9].Status));
    ui->txtAgent_10->setText(QString::number(sc->wm->ourRobot[10].Role) + " : " + QString::number(sc->wm->ourRobot[10].Status));
    ui->txtAgent_11->setText(QString::number(sc->wm->ourRobot[11].Role) + " : " + QString::number(sc->wm->ourRobot[11].Status));

    sc->wm->var[0] = ui->spnvar0->value();
    ui->txtvar0->setText(QString::number(sc->wm->var[0]));
    sc->wm->var[1] = ui->spnvar1->value();
    ui->txtvar1->setText(QString::number(sc->wm->var[1]));
    sc->wm->var[2] = ui->spnvar2->value();
    ui->txtvar2->setText(QString::number(sc->wm->var[2]));
    sc->wm->var[3] = ui->spnvar3->value();
    ui->txtvar3->setText(QString::number(sc->wm->var[3]));
    sc->wm->var[4] = ui->spnvar4->value();
    ui->txtvar4->setText(QString::number(sc->wm->var[4]));
    sc->wm->var[5] = ui->spnvar5->value();
    ui->txtvar5->setText(QString::number(sc->wm->var[5]));
    sc->wm->var[6] = ui->spnvar6->value();
    ui->txtvar6->setText(QString::number(sc->wm->var[6]));
    sc->wm->var[7] = ui->spnvar7->value();
    ui->txtvar7->setText(QString::number(sc->wm->var[7]));
    sc->wm->var[8] = ui->spnvar8->value();
    ui->txtvar8->setText(QString::number(sc->wm->var[8]));
    sc->wm->var[9] = ui->spnvar9->value();
    ui->txtvar9->setText(QString::number(sc->wm->var[9]));
}

void MainWindow::on_btnSaveVars_clicked()
{
    Settings s("vars.ini");
    s.SetValue("vars" , "var0", ui->txtvar0->text());
    s.SetValue("vars" , "var1", ui->txtvar1->text());
    s.SetValue("vars" , "var2", ui->txtvar2->text());
    s.SetValue("vars" , "var3", ui->txtvar3->text());
    s.SetValue("vars" , "var4", ui->txtvar4->text());
    s.SetValue("vars" , "var5", ui->txtvar5->text());
    s.SetValue("vars" , "var6", ui->txtvar6->text());
    s.SetValue("vars" , "var7", ui->txtvar7->text());
    s.SetValue("vars" , "var8", ui->txtvar8->text());
    s.SetValue("vars" , "var9", ui->txtvar9->text());

}

void MainWindow::on_btnLoadVars_clicked()
{
    Settings s("vars.ini");

    QString var0 = s.Get("vars", "var0");
    ui->txtvar0->setText(var0);
    ui->spnvar0->setValue(var0.toInt());

    QString var1 = s.Get("vars", "var1");
    ui->txtvar1->setText(var1);
    ui->spnvar1->setValue(var1.toInt());

    QString var2 = s.Get("vars", "var2");
    ui->txtvar2->setText(var2);
    ui->spnvar2->setValue(var2.toInt());

    QString var3 = s.Get("vars", "var3");
    ui->txtvar3->setText(var3);
    ui->spnvar3->setValue(var3.toInt());

    QString var4 = s.Get("vars", "var4");
    ui->txtvar4->setText(var4);
    ui->spnvar4->setValue(var4.toInt());

    QString var5 = s.Get("vars", "var5");
    ui->txtvar5->setText(var5);
    ui->spnvar5->setValue(var5.toInt());

    QString var6 = s.Get("vars", "var6");
    ui->txtvar6->setText(var6);
    ui->spnvar6->setValue(var6.toInt());

    QString var7 = s.Get("vars", "var7");
    ui->txtvar7->setText(var7);
    ui->spnvar7->setValue(var7.toInt());

    QString var8 = s.Get("vars", "var8");
    ui->txtvar8->setText(var8);
    ui->spnvar8->setValue(var8.toInt());

    QString var9 = s.Get("vars", "var9");
    ui->txtvar9->setText(var9);
    ui->spnvar9->setValue(var9.toInt());

}
