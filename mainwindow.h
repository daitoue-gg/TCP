#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QHostInfo>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QLabel * labSocket;
    QTcpSocket * m_socket;
    QString getLocalIP();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actConnect_triggered();

    void on_actDisconnect_triggered();

    void do_connected();
    void do_disConnect();
    void do_readyRead();
    void do_socketStateChanged(QAbstractSocket::SocketState socketState);

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
