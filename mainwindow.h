#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHostInfo>
#include <QTcpServer>
#include <QTcpSocket>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private :
    QLabel * labListen; // 状态栏标签 监听状态
    QLabel * labSocketState; // 状态栏标签 socket状态

    QTcpServer * m_server; // TCP服务器
    QTcpSocket * m_socket = nullptr; // TCP通信所用shocket

    QString getLocalIP(); // 获取本机IP地址
public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private:
    Ui::MainWindow *ui;
private slots:

    void do_newConnect();

    void do_clientConnect();

    void do_clientDisconnect();

    void do_socketStateChanged(QAbstractSocket::SocketState socketState);

    void do_socketReadyread();

    void on_actList_triggered();
    void on_actStop_triggered();
    void on_pushButton_clicked();
};
#endif // MAINWINDOW_H
