#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 状态栏标签
    labSocket = new QLabel("Socket状态：未连接");
    labSocket->setMinimumWidth(150);
    ui->statusbar->addWidget(labSocket);

    QString IP = getLocalIP();
    this->setWindowTitle("本机IP地址：" + IP);
    m_socket = new QTcpSocket(this);

    connect(m_socket,&QTcpSocket::connected,this,&MainWindow::do_connected);
    connect(m_socket,&QTcpSocket::disconnected,this,&MainWindow::do_disConnect);
    connect(m_socket,&QTcpSocket::stateChanged,this,&MainWindow::do_socketStateChanged);
    connect(m_socket,&QTcpSocket::readyRead,this,&MainWindow::do_readyRead);
}

MainWindow::~MainWindow()
{
    delete ui;
}


QString MainWindow::getLocalIP()
{
    // 获取本机IP地址
    QString hostName = QHostInfo::localHostName();
    QHostInfo hostInfo = QHostInfo::fromName(hostName);
    QList<QHostAddress> hostList = hostInfo.addresses();
    foreach(QHostAddress add,hostList)
    {
        if (add.protocol() == QAbstractSocket::IPv4Protocol)
        {
            return add.toString();
        }
    }
    return "";
}


void MainWindow::on_actConnect_triggered()
{
    //连接服务器按钮
    QString IP = ui->comboBox->currentText();
    QHostAddress add(IP);
    int port = ui->spinBox->value();
    m_socket->connectToHost(add,port);

}


void MainWindow::on_actDisconnect_triggered()
{
    // 断开连接按钮
    if (m_socket->state() == QAbstractSocket::ConnectedState)
    {
        m_socket->disconnectFromHost();
    }
}

void MainWindow::do_connected()
{
    // 成功连接后执行
    labSocket->setText("Scoket状态：已连接");
    ui->plainTextEdit->appendPlainText("***已连接到服务器");
    ui->plainTextEdit->appendPlainText("***peer address:" + m_socket->peerAddress().toString());
    ui->plainTextEdit->appendPlainText("peer port:" + QString::number(m_socket->peerPort()));
    ui->actConnect->setEnabled(false);
    ui->actDisconnect->setEnabled(true);

}

void MainWindow::do_disConnect()
{
    // 断开连接后执行
    labSocket->setText("Socket状态：未连接");
    ui->plainTextEdit->appendPlainText("已断开连接");
    ui->actConnect->setEnabled(true);
    ui->actDisconnect->setEnabled(false);
}

void MainWindow::do_readyRead()
{
    while(m_socket->canReadLine())
    {
        ui->plainTextEdit->appendPlainText(m_socket->readLine());
    }

}


void MainWindow::do_socketStateChanged(QAbstractSocket::SocketState socketState)
{
    // scoket状态变化时
    switch(socketState)
    {
    case QAbstractSocket::UnconnectedState:
        labSocket->setText("socket状态：UnconnectedState");
        break;
    case QAbstractSocket::HostLookupState:
        labSocket->setText("socket状态：HostLookupState");
        break;
    case QAbstractSocket::ConnectingState:
        labSocket->setText("socket状态：ConnectingState");
        break;
    case QAbstractSocket::ConnectedState:
        labSocket->setText("socket状态：ConnectedState");
        break;
    case QAbstractSocket::BoundState:
        labSocket->setText("socket状态：BoundState");
        break;
    case QAbstractSocket::ClosingState:
        labSocket->setText("socket状态：ClosingState");
        break;
    case QAbstractSocket::ListeningState:
        labSocket->setText("socket状态：ListeningState");
    }
}


void MainWindow::on_pushButton_clicked()
{
    // 发送消息按钮
    QString msg = ui->lineEdit->text();
    ui->plainTextEdit->appendPlainText("[out]" + msg);
    ui->lineEdit->clear();
    ui->lineEdit->setFocus();
    QByteArray str = msg.toUtf8();
    str.append('\n');
    m_socket->write(str);
}

