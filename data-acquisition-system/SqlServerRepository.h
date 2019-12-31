#pragma once

#include <QObject>

class SqlServerRepository : public QObject
{
	Q_OBJECT

public:
	SqlServerRepository(QObject *parent);
	~SqlServerRepository();
};
