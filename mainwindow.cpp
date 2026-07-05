#include "mainwindow.h"
#include "ui_mainwindow.h"

QString MainWindow::getLocalIP()
{
    // 获取本机IP地址
    QString hostName = QHostInfo::localHostName();
    QHostInfo hostInfo = QHostInfo::fromName(hostName);
    QList<QHostAddress> addLists = hostInfo.addresses();
    if (addLists.isEmpty()) return "";
    foreach(QHostAddress aAdd,addLists)
    {
        if (aAdd.protocol() == QAbstractSocket::IPv4Protocol)
        return aAdd.toString();
    }
    return "";
}

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 状态栏设置
    labListen = new QLabel("监听状态：");
    labListen->setMinimumWidth(150);
    labSocketState = new QLabel("Socket状态：");
    labSocketState->setMinimumWidth(150);
    ui->statusbar->addWidget(labListen);
    ui->statusbar->addWidget(labSocketState);

    // 设置窗口标题为IP地址
    QString str = getLocalIP();
    this->setWindowTitle("本机IP地址：" + str);
    ui->comboBox->addItem(str);

    // 创建服务器
    m_server = new QTcpServer(this);
    connect(m_server,&QTcpServer::newConnection,this,&MainWindow::do_newConnect);

}

MainWindow::~MainWindow()
{
    if (m_socket != nullptr)
    {
        if (m_socket->state() == QAbstractSocket::ConnectedState)
        {
            m_socket->disconnectFromHost();
        }
    }
    if (m_server->isListening())
    {
        m_server->close();
    }
    delete ui;
}

void MainWindow::do_newConnect()
{
    // 当有客户端接入是，TcpServer会发射newConnection信号
    m_socket = m_server->nextPendingConnection(); // 创建shocket

    connect(m_socket,&QTcpSocket::connected,this,&MainWindow::do_clientConnect);
    do_clientConnect(); // 更新状态
    connect(m_socket,&QTcpSocket::disconnected,this,&MainWindow::do_clientDisconnect);
    connect(m_socket,&QTcpSocket::stateChanged,this,&MainWindow::do_socketStateChanged);
    do_socketStateChanged(m_socket->state());  // 更新状态
    connect(m_socket,&QTcpSocket::readyRead,this,&MainWindow::do_socketReadyread);

}

void MainWindow::do_clientConnect()
{
    // 客户端接入时
    qDebug("m_socket的connect信号发射");
    ui->plainTextEdit->appendPlainText("**client socket connected");
    ui->plainTextEdit->appendPlainText("peer address：" + m_socket->peerAddress().toString()); // 返回与之连接的地址
    ui->plainTextEdit->appendPlainText("peer port:" + QString::number(m_socket->peerPort())); // 返回与之连接的端口
}

void MainWindow::do_clientDisconnect()
{
    // 客户端断开连接
    ui->plainTextEdit->appendPlainText("**client disconnected");
    m_socket->deleteLater();
}

void MainWindow::do_socketStateChanged(QAbstractSocket::SocketState socketState)
{
    // scoket状态变化时
    switch(socketState)
    {
    case QAbstractSocket::UnconnectedState:
        labSocketState->setText("socket状态：UnconnectedState");
        break;
    case QAbstractSocket::HostLookupState:
        labSocketState->setText("socket状态：HostLookupState");
        break;
    case QAbstractSocket::ConnectingState:
        labSocketState->setText("socket状态：ConnectingState");
        break;
    case QAbstractSocket::ConnectedState:
        labSocketState->setText("socket状态：ConnectedState");
        break;
    case QAbstractSocket::BoundState:
        labSocketState->setText("socket状态：BoundState");
        break;
    case QAbstractSocket::ClosingState:
        labSocketState->setText("socket状态：ClosingState");
        break;
    case QAbstractSocket::ListeningState:
        labSocketState->setText("socket状态：ListeningState");
    }
}

void MainWindow::do_socketReadyread()
{
    while(m_socket->canReadLine())
    {
        ui->plainTextEdit->appendPlainText("[in]" + m_socket->readLine());
    }
}



void MainWindow::on_actList_triggered()
{
    // 开始监听按钮
    QString IP = ui->comboBox->currentText();
    quint16 port = ui->spinBox->value();
    QHostAddress address(IP);
    m_server->listen(address,port);

    ui->actStop->setEnabled(true);
    ui->actList->setEnabled(false);

    ui->plainTextEdit->appendPlainText("开始监听");
    ui->plainTextEdit->appendPlainText("***服务器地址：" + IP);
    ui->plainTextEdit->appendPlainText("***服务区端口：" + QString::number(port));

    labListen->setText("监听状态：正在监听");
    labSocketState->setText("Socket状态：未连接");

}


void MainWindow::on_actStop_triggered()
{
    // 停止监听按钮
    if (m_server->isListening())
    {
        if (m_socket != nullptr)
            if (m_socket->state() == QAbstractSocket::ConnectedState)
                m_socket->disconnectFromHost();
        m_server->close();
        ui->actList->setEnabled(true);
        ui->actStop->setEnabled(false);
        labListen->setText("监听状态：已停止监听");

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

