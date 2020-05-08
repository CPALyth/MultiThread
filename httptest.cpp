#include "httptest.h"

HttpTest::HttpTest(QObject *parent) : QObject(parent)
{
    state = StateHead;
    QNetworkRequest request;
    request.setUrl(QUrl("http://download.qt.io/official_releases/qtcreator/4.5/4.5.2/qt-creator-opensource-src-4.5.2.tar.xz"));
    QNetworkReply* reply = mgr.head(request);   // POST请求获取网络标头
    connect(reply,SIGNAL(finished()),
            this,SLOT(onLoadFinished()));
}

void HttpTest::onLoadFinished()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(this->sender());
    if(reply == NULL)
        return;

    if(state == StateHead)
    {
        fileSize = reply->rawHeader("Content-Length").toInt() / 1024 / 1024;
        qDebug() << "file size:" << fileSize << " MB";

        QNetworkRequest request;
        request.setUrl(QUrl("http://download.qt.io/official_releases/qtcreator/4.5/4.5.2/qt-creator-opensource-src-4.5.2.tar.xz"));
        QNetworkReply* replyNew = mgr.get(request);

        connect(replyNew,SIGNAL(finished()),
                this,SLOT(onLoadFinished()));
        state = StateCheck;
    }
    else if(state == StateCheck)
    {
        qDebug() << "状态码:" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(); // 输出状态码
        QUrl urlNew = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();

        // 分段请求
        QNetworkRequest request;
        request.setUrl(urlNew);

        int sectionOne = fileSize / 4;


        QString qstrSectionOne = QString::number(sectionOne);
        QString sectionOneRange = QString("bytes=0-%1").arg(qstrSectionOne);

        request.setRawHeader("Range",sectionOneRange.toLocal8Bit());
        qDebug() << "request range:" << sectionOneRange;

        QNetworkReply* replyNew = mgr.get(request);
        connect(replyNew,SIGNAL(finished()),
                this,SLOT(onLoadFinished()));

        connect(replyNew,SIGNAL(downloadProgress(qint64,qint64)),
                this,SLOT(onDownloadProgress(qint64,qint64)));

        state = StateDownload;
    }
    else if(state == StateDownload)
    {
        qDebug() << "finished.";
    }
}

void HttpTest::onDownloadProgress(qint64 cur, qint64 total)
{
    qDebug() << "down:" << cur << "/" << total;
}
