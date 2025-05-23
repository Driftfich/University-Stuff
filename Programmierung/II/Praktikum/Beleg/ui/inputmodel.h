#ifndef _INPUTMODEL_H
#define _INPUTMODEL_H

#include <QStandardItemModel>

class InputModel : public QStandardItemModel {
    Q_OBJECT
    
    public:
        explicit InputModel(QObject *parent = nullptr);
        ~InputModel() override;
        
        void configureFromSource(QAbstractTableModel* sourceModel);

};


#endif