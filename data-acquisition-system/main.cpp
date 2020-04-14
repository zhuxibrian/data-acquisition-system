#include "MainWindow.h"
#include <QtWidgets/QApplication>

#include "QDateTime"
#include "QDir"
#include <vector>
#include <QThread>
#include <QMessageBox>

#define GOOGLE_GLOG_DLL_DECL
//#define GLOG_NO_ABBREVIATED_SEVERITIES
#include "glog/logging.h"

#ifdef _DEBUG
#pragma comment(lib,"../lib/libglog_staticd.lib")
#else
#pragma comment(lib,"../lib/libglog_static.lib")
#endif // DEBUG

#include "Config.h"
#include "SqlServerRepository.h"
#include "AdvatechCard.h"
#include "TcpServer.h"

#define LOGOUTTIME (60*60*24*10)
void initLog()
{
	QDateTime current = QDateTime::currentDateTime();
	QDir logdir(QDir::tempPath() + "/DasLog/");
	if (!logdir.exists())
	{
		logdir.mkpath(logdir.absolutePath());
		return;
	}

	
	logdir.setFilter(QDir::Files);
	QFileInfoList fileList = logdir.entryInfoList();
	int fileCount = fileList.count();
	for (int i = 0; i < fileCount; i++)
	{
		QFileInfo fileInfo = fileList[i];
		QDateTime lastModify = fileInfo.lastModified();
		if (lastModify.secsTo(current) > LOGOUTTIME)
		{
			logdir.remove(fileInfo.absoluteFilePath());
		}
	}

	FLAGS_logtostderr = false;		//TRUE:标准输出,FALSE:文件输出
	FLAGS_alsologtostderr = true;	//除了日志文件之外是否需要标准输出
	FLAGS_colorlogtostderr = true;  //标准输出带颜色
	FLAGS_logbufsecs = 0;			//设置可以缓冲日志的最大秒数，0指实时输出
	FLAGS_max_log_size = 10;		//日志文件大小(单位：MB)
	FLAGS_stop_logging_if_full_disk = true; //磁盘满时是否记录到磁盘
	google::InitGoogleLogging("ruiwei");
	google::SetLogDestination(google::GLOG_INFO, (logdir.absolutePath()+"/").toStdString().c_str());
	google::SetStderrLogging(google::GLOG_INFO);
	google::SetLogFilenameExtension("log_");
}


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	initLog();

	LOG(INFO) << "data acquisition system start.";

	Config config("./config/config.yaml");
    config.loadConfig();

    QObject::connect(&TcpServer::getInstance(), &TcpServer::startReadData, [=]() {
        LOG(INFO) << "startReadData signal recv";
    });

    QObject::connect(&TcpServer::getInstance(), &TcpServer::stopReadData, [=]() {
        LOG(INFO) << "stopReadData signal recv";
    });

    QObject::connect(&TcpServer::getInstance(), &TcpServer::startSendData, []() {
        for (int j=0; j<5; j++)
        {
            double *data = new double[1000];
            srand((unsigned)time(NULL));
            for (int i = 0; i < 1000; i++)
            {
                data[i] = (double)(rand() / (double)RAND_MAX);
            }

            DeviceDataMsg* msg = new DeviceDataMsg();
            msg->data = new unsigned char[sizeof(double) * 1000];
            memcpy(msg->data, data, sizeof(double) * 1000);
            msg->type = MsgType::msg_deviceData;
            msg->dataSize = sizeof(DeviceDataMsg) + sizeof(double) * 1000 - 4;
            msg->deviceType = DeviceType::dt_advatechCard;
            msg->deviceId = 1;

            delete[] data;
            data = nullptr;
            TcpServer::getInstance().pushSendMsg(msg);

            Sleep(3000);
        }
        LOG(INFO) << "startSendData signal recv";
    });

    QObject::connect(&TcpServer::getInstance(), &TcpServer::stopSendData, [=]() {
        LOG(INFO) << "stopSendData signal recv";
    });

    int res = a.exec();

    return res;

    
    

    /*DatabaseConfig* dc = config.getDatabaseConfig();

    std::shared_ptr<SqlServerRepository> ssr = std::make_shared<SqlServerRepository>();
    if (!ssr->connect(dc->host, dc->dbname, dc->username, dc->password)) {
        return 0;
    }
    LOG(INFO) << "database connect success.";


    QThread thread;
    DataAnalyzer analyzer(ssr);
    QObject::connect(&analyzer, &DataAnalyzer::pushDataSignal, &analyzer, &DataAnalyzer::handleData, Qt::QueuedConnection);

    analyzer.moveToThread(&thread);
    thread.start();

    TcpServer tcpServer(nullptr);

    DeviceConfig *deviceConfig = config.getDeviceConfig();
    std::vector<AdvatechCard*> advathechCards;
    ErrorCode        ret = Success;
    for (int i = 0; i < deviceConfig->devices.size(); i++)
    {
        AdvatechCard* card = new AdvatechCard;
        ret = card->init(deviceConfig->devices[i], &analyzer);
        advathechCards.push_back(card);
        if (ret != Success && ret >= 0xE0000000)
        {
            QMessageBox::about(NULL, "Error", "advatech card init failed!");
            break;
        }
    }

    if (ret == Success)
    {
        for (const auto& c : advathechCards)
        {
            c->start();
        }
    }

    MainWindow w;
    w.show();

    int res = a.exec();

    thread.quit();
    thread.wait();
    return res;*/
}
