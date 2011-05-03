#include "upnpdeviceslistmodel.h"


#include <HUpnpCore/HUdn>
#include <HUpnpCore/HClientDevice>
#include <HUpnpCore/HClientService>
#include <HUpnpCore/HServiceInfo>
#include <HUpnpCore/HServiceId>

#include <iostream>

UpnpDeviceRecord::UpnpDeviceRecord(const Herqq::Upnp::HClientDevice* device, int level)
{
   deviceInfo_ = device->info();
   services_ = QList<Herqq::Upnp::HServiceInfo>();
   isMobileDevice_=false;
   level_=level;
   Herqq::Upnp::HClientServices services = device->services();
   for(Herqq::Upnp::HClientServices::iterator it=services.begin(); it!=services.end(); ++it) {
       Herqq::Upnp::HClientService* service = *it;
       const Herqq::Upnp::HServiceInfo& serviceInfo = service->info();
       services_.push_back(serviceInfo);
       if (serviceInfo.serviceId().suffix()==const_MobileDevice) {
           isMobileDevice_=true;
       }
   }
}

UpnpDeviceRecord::UpnpDeviceRecord(const UpnpDeviceRecord& x)
{
  deviceInfo_=x.deviceInfo_;
  services_ = QList<Herqq::Upnp::HServiceInfo>();
  services_.append(x.services_);
  isMobileDevice_ = x.isMobileDevice_;
  level_=x.level_;
}


UpnpDeviceRecord& UpnpDeviceRecord::operator=(const UpnpDeviceRecord& x)
{
    deviceInfo_=x.deviceInfo_;
    services_ = QList<Herqq::Upnp::HServiceInfo>();
    services_.append(x.services_);
    isMobileDevice_ = x.isMobileDevice_;
    level_=x.level_;
    return *this;
}



const QString UpnpDeviceRecord::const_MobileDevice = QString("MobileDevice");


UpnpDevicesListModel::UpnpDevicesListModel(QObject *parent) :
    QAbstractListModel(parent)
{
     onlyMobile=false;
}


QVariant UpnpDevicesListModel::data(const QModelIndex& index, int role) const
{
  if (!index.isValid()) {
      return QVariant();
  }
  const UpnpDeviceRecord& item = items[index.row()];
  switch(role) {
     case Qt::DisplayRole:
                return QString(item.deviceInfo().friendlyName());
     case Qt::DecorationRole:
                /* TODO: set-up icon */
                return QVariant();
  default:
      return QVariant();
  }
}


bool UpnpDevicesListModel::addIfApply(const Herqq::Upnp::HClientDevice* device)
{
  QList<UpnpDeviceRecord>::const_iterator it = findSameItem(device->info());
  if (it==items.end()) {
      UpnpDeviceRecord r = UpnpDeviceRecord(device);
      if (!onlyMobile || r.isMobileDevice()) {
        QList<UpnpDeviceRecord> embedded;
        appendEmbedded(embedded,device,0);
        beginInsertRows(QModelIndex(),items.size(),items.size()+embedded.size());
        items.push_back(UpnpDeviceRecord(device));
        items.append(embedded);
        endInsertRows();
        return true;
      } else {
        return false;
      }
  } else {
      //
      return false;
  }
}



void UpnpDevicesListModel::appendEmbedded(QList<UpnpDeviceRecord>& records, const Herqq::Upnp::HClientDevice* device, int level)
{
    Herqq::Upnp::HClientDevices devices = device->embeddedDevices();
    for(Herqq::Upnp::HClientDevices::const_iterator it=devices.begin(); it<devices.end();++it) {
        Herqq::Upnp::HClientDevice* childDevice = *it;
        //std::cerr << "child: " << childDevice->info().friendlyName().toUtf8().data() << std::endl;
        records.push_back(UpnpDeviceRecord(childDevice,level+1));
        appendEmbedded(records,childDevice,level+1);
    }
}


bool UpnpDevicesListModel::remove(const Herqq::Upnp::HClientDevice* device)
{
  QList<UpnpDeviceRecord>::const_iterator it = findSameItem(device->info());
  if (it==items.end()) {
      return false;
  } else {
      int index = it - items.begin();
      QList<UpnpDeviceRecord> embedded;
      appendEmbedded(embedded,device,0);
      beginRemoveRows(QModelIndex(),index,index+embedded.size());
      items.removeAt(index);
      for(int i=0; i<embedded.size();++i) {
          items.removeAt(index);
      }
      endRemoveRows();
      return false;
  }
}


void UpnpDevicesListModel::clear()
{
    beginResetModel();
    items.clear();
    endResetModel();
}

QList<UpnpDeviceRecord>::const_iterator UpnpDevicesListModel::findSameItem(const Herqq::Upnp::HDeviceInfo& item)
{
  QList<UpnpDeviceRecord>::const_iterator it = items.begin();
  for(;it!=items.end();++it) {
      if (it->deviceInfo().udn()==item.udn()) {
          return it;
      }
  }
  return items.end();
}
