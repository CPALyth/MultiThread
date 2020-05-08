#ifndef HTTPTEST_H
#define HTTPTEST_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDebug>

enum DownloadStatus
{
    StateHead,  // 获取头判断大小
    StateCheck, // 检查状态码.是否出错,是否重定向
    StateDownload   // 下载
};


class HttpTest : public QObject
{
    Q_OBJECT
public:
    explicit HttpTest(QObject *parent = 0);

signals:

public slots:
    void onLoadFinished();
    void onDownloadProgress(qint64 cur,qint64 total);

private:
    QNetworkAccessManager mgr;
    DownloadStatus state;
    int fileSize = 0;
};

#endif // HTTPTEST_H
