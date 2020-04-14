#pragma once

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QByteArray>
#include <mutex>
#include <queue>
#include "QThread"

template<class T>
class thread_safe_queue
{
private:
    mutable std::mutex mut;
    std::queue<std::shared_ptr<T>> data_queue;
    std::condition_variable data_con;
public:
    thread_safe_queue() {}
    thread_safe_queue(thread_safe_queue const& other)
    {
        std::lock_guard<std::mutex> lk(other.mut);
        data_queue = other.data_queue;
    }
    void push(T tValue)
    {
        std::shared_ptr<T> data(std::make_shared<T>(std::move(tValue)));
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(data);
        data_con.notify_one();
    }
    void wait_and_pop(T& tValue)
    {
        std::unique_lock<std::mutex> lk(mut);
        data_con.wait(lk, [this] {return !data_queue.empty(); });
        tValue = std::move(*data_queue.front());
        data_queue.pop();
    }
    std::shared_ptr<T>wait_and_pop()
    {
        std::unique_lock<std::mutex> lk(mut);
        data_con.wait(lk, [this] {return !data_queue.empty(); });
        std::shared_ptr<T> ret(std::make_shared<T>(data_queue.front()));
        data_queue.pop();
        return ret;
    }
    bool try_pop(T& tValue)
    {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty())
            return false;
        tValue = std::move(*data_queue.front());
        data_queue.pop();
        return true;
    }

    std::shared_ptr<T> try_pop()
    {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty())
            return std::shared_ptr<T>();
        std::shared_ptr<T> ret(std::make_shared(data_queue.front()));
        data_queue.pop();
        return ret;
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.empty();
    }

    bool count() const
    {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.size();
    }
};

template<typename T> class ringbuffer {
public:
    /**
      * create a ringbuffer with space for up to size elements.
      */
    explicit ringbuffer(size_t size)
        : size(size)
        , begin(0)
        , end(0)
        , wrap(false)
    {
        buffer = new T[size];
    }

    /**
      * copy constructor
      */
    ringbuffer(const ringbuffer<T> & rb)
    {
        this(rb.size);
        begin = rb.begin;
        end = rb.end;
        memcpy(buffer, rb.buffer, sizeof(T) * size);
    }

    /**
      * destructor
      */
    ~ringbuffer()
    {
        delete[] buffer;
    }

    size_t write(const T * data, size_t n)
    {
        n = std::min(n, getFree());

        if (n == 0) {
            return n;
        }

        const size_t first_chunk = std::min(n, size - end);
        memcpy(buffer + end, data, first_chunk * sizeof(T));
        end = (end + first_chunk) % size;

        if (first_chunk < n) {
            const size_t second_chunk = n - first_chunk;
            memcpy(buffer + end, data + first_chunk, second_chunk * sizeof(T));
            end = (end + second_chunk) % size;
        }

        if (begin == end) {
            wrap = true;
        }

        return n;
    }

    size_t read(T * dest, size_t n)
    {
        n = std::min(n, getOccupied());

        if (n == 0) {
            return n;
        }

        if (wrap) {
            wrap = false;
        }

        const size_t first_chunk = std::min(n, size - begin);
        memcpy(dest, buffer + begin, first_chunk * sizeof(T));
        begin = (begin + first_chunk) % size;

        if (first_chunk < n) {
            const size_t second_chunk = n - first_chunk;
            memcpy(dest + first_chunk, buffer + begin, second_chunk * sizeof(T));
            begin = (begin + second_chunk) % size;
        }
        return n;
    }

    size_t getOccupied() {
        if (end == begin) {
            return wrap ? size : 0;
        }
        else if (end > begin) {
            return end - begin;
        }
        else {
            return size + end - begin;
        }
    }

    size_t getFree() {
        return size - getOccupied();
    }
private:
    T * buffer;
    size_t size;
    size_t begin;
    size_t end;
    bool wrap;
};

enum MsgType
{
    msg_unknown = 0,
    msg_start,              //采集开始
    msg_stop,               //采集停止
    msg_startSendData,      //开始发送实时数据
    msg_stopSendData,       //停止发送实时数据
    msg_deviceData          //实时发送的数据
};

enum DeviceType
{
    dt_unknown = 0,
    dt_advatechCard,
    dt_serialPort
};

struct BaseMsg
{
    MsgType type;
    unsigned int dataSize;    //数据包长度，包含包头：BaseMsg
};

struct DeviceDataMsg : BaseMsg
{
    unsigned int deviceId;
    unsigned int deviceType;
    unsigned char* data;

    ~DeviceDataMsg()
    {
        delete[] data;
        data = nullptr;
    }
};


class TcpServer : public QThread
{
    Q_OBJECT

public:
    ~TcpServer();
    TcpServer(const TcpServer&) = delete;
    TcpServer& operator=(const TcpServer&) = delete;
    static TcpServer& getInstance() {
        static TcpServer instance;
        return instance;
    }

    void pushSendMsg(DeviceDataMsg* msg);

signals:
    void startReadData();
    void stopReadData();
    void startSendData();
    void stopSendData();

private slots:
    void displayError(QAbstractSocket::SocketError socketError);
    void NewConnectionSlot();
    void disconnectedSlot();
    void ReadData();



private:
    TcpServer(QObject* parent = nullptr);
    void run();

    void recvMsgHandle();


    QTcpServer* _tcpServer = { nullptr };
    QTcpSocket* _clientSocket = {nullptr};

    thread_safe_queue<DeviceDataMsg*> _msgQueue;

#define MAX_SIZE 1000
    ringbuffer<char> _recvBuffer = { ringbuffer<char>(1000) };
    

};
