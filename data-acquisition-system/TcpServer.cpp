#include "TcpServer.h"
#include "glog/logging.h"


TcpServer::TcpServer(QObject *parent)
{
    _tcpServer = new QTcpServer(this);

    connect(_tcpServer, &QTcpServer::newConnection, this, &TcpServer::NewConnectionSlot);
    connect(_tcpServer, &QTcpServer::acceptError, this, &TcpServer::displayError);

    if (!_tcpServer->listen(QHostAddress::Any, 10086))
    {
        LOG(ERROR) << "m_pTcpServer->listen() error";
    }

    this->start();
}

void TcpServer::pushSendMsg(DeviceDataMsg* msg)
{
    if (_msgQueue.count() > 10)
    {
        delete msg;
        return;
    }
    _msgQueue.push(msg);
}

TcpServer::~TcpServer()
{
    if (_tcpServer)
    {
        _tcpServer->close();
        _tcpServer->deleteLater();
    }

    this->terminate();
}

void TcpServer::displayError(QAbstractSocket::SocketError socketError)
{
    LOG(ERROR) << "accapt client error.";
}

void TcpServer::NewConnectionSlot()
{
    _clientSocket = _tcpServer->nextPendingConnection();
    connect(_clientSocket, &QTcpSocket::readyRead, this, &TcpServer::ReadData);
    connect(_clientSocket, &QTcpSocket::disconnected, this, &TcpServer::disconnectedSlot);
}

void TcpServer::disconnectedSlot()
{
    if (_clientSocket->state() == QAbstractSocket::UnconnectedState)
    {
        _clientSocket->deleteLater();
    }
}

void TcpServer::ReadData()
{
    char* buf = new char[MAX_SIZE];
    int num = _clientSocket->read((char*)buf, MAX_SIZE);
    _recvBuffer.write(buf, num);
    delete buf;
    buf = nullptr;
    recvMsgHandle();
}

void TcpServer::run()
{
    for (;;)
    {
        DeviceDataMsg *msg;
        _msgQueue.wait_and_pop(msg);
        _clientSocket->write((char*)msg, sizeof(DeviceDataMsg)-4);
        _clientSocket->flush();
        _clientSocket->write((char*)msg->data, msg->dataSize - sizeof(DeviceDataMsg) + 4);
        _clientSocket->flush();
        delete msg;
        msg = nullptr;
    }
}

void TcpServer::recvMsgHandle()
{
    while (_recvBuffer.getOccupied() >= sizeof(BaseMsg))
    {
        BaseMsg msg;
        _recvBuffer.read((char*)&msg, sizeof(BaseMsg));
        switch (msg.type)
        {
        case MsgType::msg_start:
            emit startReadData();
            break;
        case MsgType::msg_stop:
            emit stopReadData();
            break;
        case MsgType::msg_startSendData:
            emit startSendData();
            break;
        case MsgType::msg_stopSendData:
            emit stopSendData();
            break;
        default:
            LOG(WARNING) << "tcp server recive unknown msg:" << msg.type << " size:" << msg.dataSize;
            break;
        }
    }
}
