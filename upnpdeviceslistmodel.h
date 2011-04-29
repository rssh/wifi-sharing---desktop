#ifndef UPNPDEVICESLISTMODEL_H
#define UPNPDEVICESLISTMODEL_H

#include <QListView>
#include <QAbstractListModel>
#include <QListWidget>


#include <HUpnpCore/HDeviceInfo>
#include <HUpnpCore/HClientDevice>
#include <HUpnpCore/HServiceInfo>



class UpnpDeviceRecord
{
public:

     UpnpDeviceRecord(const Herqq::Upnp::HClientDevice* device, int level=0);

     UpnpDeviceRecord(const UpnpDeviceRecord&);

     UpnpDeviceRecord& operator=(const UpnpDeviceRecord&);

     const Herqq::Upnp::HDeviceInfo&  deviceInfo() const
       { return deviceInfo_; }

     const QList<Herqq::Upnp::HServiceInfo> services() const
       { return services_; }

     bool isMobileDevice() const
       { return isMobileDevice_; }

     int  levelOfEmbedding() const
       { return level_; }


private:

    Herqq::Upnp::HDeviceInfo  deviceInfo_;
    QList<Herqq::Upnp::HServiceInfo> services_;
    bool                      isMobileDevice_;
    int                       level_;

public:

  static const QString const_MobileDevice;

};



class UpnpDevicesListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit UpnpDevicesListModel(QObject *parent = 0);

    int rowCount(const QModelIndex &  = QModelIndex()) const
      { return items.count(); }

    QVariant data(const QModelIndex& index, int role) const;

    const UpnpDeviceRecord& operator[](int i) const
                    { return items[i]; }

    const UpnpDeviceRecord& at(int i) const
                    { return items[i]; }

    bool  getOnlyMobile()
             { return onlyMobile; }

    void  setOnlyMobile(bool value)
             { onlyMobile=value; }

signals:

public slots:


    bool addIfApply(const Herqq::Upnp::HClientDevice* device);

    bool remove(const Herqq::Upnp::HClientDevice* device);

    void clear();


private:

   QList<UpnpDeviceRecord>::const_iterator findSameItem(const Herqq::Upnp::HDeviceInfo& item);


   static void appendEmbedded(QList<UpnpDeviceRecord>& records, const Herqq::Upnp::HClientDevice* device, int level);


private:

    QList<UpnpDeviceRecord>  items;
    bool                     onlyMobile;


};

#endif // UPNPDEVICESLISTMODEL_H
