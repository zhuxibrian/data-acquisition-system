#include "MainWindow.h"
#include <QtWidgets/QApplication>

#include "QDateTime"
#include "QDir"

#define GOOGLE_GLOG_DLL_DECL
#define GLOG_NO_ABBREVIATED_SEVERITIES
#include "glog/logging.h"

#ifdef _DEBUG
#pragma comment(lib,"../lib/libglog_staticd.lib")
#else
#pragma comment(lib,"../lib/libglog_static.lib")
#endif // DEBUG

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

	QDir dir;
	dir.setFilter(QDir::Files);
	QFileInfoList fileList = dir.entryInfoList();
	int fileCount = fileList.count();
	for (int i = 0; i < fileCount; i++)
	{
		QFileInfo fileInfo = fileList[i];
		QDateTime lastModify = fileInfo.lastModified();
		if (lastModify.secsTo(current) > LOGOUTTIME)
		{
			dir.remove(fileInfo.absoluteFilePath());
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
	MainWindow w;
	w.show();
	return a.exec();
}
