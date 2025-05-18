// Abstract Qt table model for the personman class

#ifndef _PERSONMODEL_H
#define _PERSONMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QVariant>
#include <QModelIndex>
#include "personman.h"

class PersonTableModel : public QAbstractTableModel {
    Q_OBJECT

    PersonMan* personMan;

    public:
        enum ColumnIdentity {
            Id,
            FirstName,
            LastName,
            ExtensionName,
            BirthDate,
            Gender,
            Location,
            Email,
            Phone,
            Note,
            MaxColumnIdentity
        };

        Q_ENUM(ColumnIdentity)

        explicit PersonTableModel(PersonMan* personMan, QObject *parent = nullptr);
        ~PersonTableModel() override;

        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int columnCount(const QModelIndex &parent = QModelIndex()) const override;

        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
        Qt::ItemFlags flags(const QModelIndex &index) const override;

        void setDisplayedColumns(const QVector<ColumnIdentity>& displayedColumns);
        QMap<ColumnIdentity, QString> getAllColumnNames() const;
        QVector<ColumnIdentity> getDisplayedColumns() const;
    
        public slots:
            void refreshData();







    


};

#endif