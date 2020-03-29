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

	FLAGS_logtostderr = false;		//TRUE:��׼���,FALSE:�ļ����
	FLAGS_alsologtostderr = true;	//������־�ļ�֮���Ƿ���Ҫ��׼���
	FLAGS_colorlogtostderr = true;  //��׼�������ɫ
	FLAGS_logbufsecs = 0;			//���ÿ��Ի�����־�����������0ָʵʱ���
	FLAGS_max_log_size = 10;		//��־�ļ���С(��λ��MB)
	FLAGS_stop_logging_if_full_disk = true; //������ʱ�Ƿ��¼������
	google::InitGoogleLogging("ruiwei");
	google::SetLogDestination(google::GLOG_INFO, (logdir.absolutePath()+"/").toStdString().c_str());
	google::SetStderrLogging(google::GLOG_INFO);
	google::SetLogFilenameExtension("log_");
}


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
    MainWindow w;
    w.show();

	initLog();

	LOG(INFO) << "data acquisition system start.";

	Config config("./config/config.yaml");
    config.loadConfig();

	DatabaseConfig* dc = config.getDatabaseConfig();

	std::shared_ptr<SqlServerRepository> ssr = std::make_shared<SqlServerRepository>();
	if (!ssr->connect(dc->host, dc->dbname, dc->username, dc->password)) {
		return 0;
	}
	LOG(INFO) << "database connect success.";


    QThread thread;
    DataAnalyzer analyzer(ssr);
    QObject::connect(&w, &MainWindow::closeSignal, [&]() {
        thread.quit();
        thread.wait();
    });
    QObject::connect(&analyzer, &DataAnalyzer::pushDataSignal, &analyzer, &DataAnalyzer::handleData, Qt::QueuedConnection);

    analyzer.moveToThread(&thread);
    thread.start();

	DeviceConfig *deviceConfig = config.getDeviceConfig();
    std::vector<AdvatechCard*> advathechCards;
    ErrorCode        ret = Success;
	for (int i=0; i<deviceConfig->devices.size(); i++)
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

	return a.exec();
}
